#include <memory>
#include "Model.h"
#include "View.h"
#include "Command.h"

// Controller using Command pattern
class Controller
{
    std::shared_ptr<Model> model_;
    std::vector<std::shared_ptr<View>> views_;
    std::queue<std::unique_ptr<Command>> commandQueue_;

public:
    Controller(std::shared_ptr<Model> model) : model_(model) {}

    void addView(std::shared_ptr<View> view)
    {
        views_.push_back(view);
        model_->attach(view);
    }

    void executeCommand(std::unique_ptr<Command> cmd)
    {
        commandQueue_.push(std::move(cmd));
        processCommands();
    }

private:
    void processCommands()
    {
        while (!commandQueue_.empty())
        {
            auto cmd = std::move(commandQueue_.front());
            commandQueue_.pop();
            cmd->execute(*model_);
        }
    }
};
