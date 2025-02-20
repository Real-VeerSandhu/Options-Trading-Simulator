#include <array>
#include <bitset>
#include <cstddef>
#include <new>
#include <type_traits>

template <typename T, size_t BlockSize = 4096>
class MemoryPool
{
    static_assert(BlockSize >= sizeof(T), "BlockSize must be larger than object size");
    static_assert(std::is_trivially_destructible_v<T>, "T must be trivially destructible");

    // Cache line alignment for optimal performance
    alignas(64) struct Block
    {
        std::array<std::byte, BlockSize> storage;
        std::bitset<BlockSize / sizeof(T)> used;
        Block *next;
    };

    Block *head_ = nullptr;
    static constexpr size_t ObjectsPerBlock = BlockSize / sizeof(T);

public:
    // Custom allocator that implements the allocator concept
    template <typename U>
    struct Allocator
    {
        using value_type = U;

        Allocator(MemoryPool &pool) noexcept : pool_(pool) {}

        template <typename V>
        Allocator(const Allocator<V> &other) noexcept : pool_(other.pool_) {}

        [[nodiscard]] U *allocate(size_t n)
        {
            if (n == 1)
                return static_cast<U *>(pool_.allocate());
            return static_cast<U *>(::operator new(n * sizeof(U)));
        }

        void deallocate(U *p, size_t n) noexcept
        {
            if (n == 1)
                pool_.deallocate(p);
            else
                ::operator delete(p);
        }

    private:
        MemoryPool &pool_;
    };

    [[nodiscard]] void *allocate()
    {
        if (!head_ || head_->used.all())
        {
            auto newBlock = new Block();
            newBlock->next = head_;
            head_ = newBlock;
        }

        // Find first available slot using bit manipulation
        auto index = head_->used._Find_first();
        head_->used.set(index);

        return &head_->storage[index * sizeof(T)];
    }

    void deallocate(void *ptr) noexcept
    {
        for (auto block = head_; block; block = block->next)
        {
            auto start = reinterpret_cast<std::byte *>(&block->storage[0]);
            auto end = start + BlockSize;

            if (start <= ptr && ptr < end)
            {
                size_t index = (static_cast<std::byte *>(ptr) - start) / sizeof(T);
                block->used.reset(index);
                return;
            }
        }
    }

    // RAII-style scope guard for automatic memory management
    class ScopeGuard
    {
        MemoryPool &pool_;
        void *ptr_;

    public:
        ScopeGuard(MemoryPool &pool, void *ptr) : pool_(pool), ptr_(ptr) {}
        ~ScopeGuard() { pool_.deallocate(ptr_); }
    };

    // Statistics for monitoring
    struct Stats
    {
        size_t total_blocks;
        size_t total_objects;
        size_t used_objects;
    };

    [[nodiscard]] Stats getStats() const noexcept
    {
        Stats stats{0, 0, 0};
        for (auto block = head_; block; block = block->next)
        {
            ++stats.total_blocks;
            stats.total_objects += ObjectsPerBlock;
            stats.used_objects += block->used.count();
        }
        return stats;
    }

    ~MemoryPool()
    {
        while (head_)
        {
            auto next = head_->next;
            delete head_;
            head_ = next;
        }
    }
};