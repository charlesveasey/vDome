#include "command.h"
namespace vd {

extern int maxHistory;

Command::~Command() {}

CommandHistory::CommandHistory() : index(0) {
    lastCommand = 0;
}

CommandHistory::~CommandHistory() {}

void CommandHistory::execute(Command* command) {
    if (history.size() >= maxHistory)
        history.erase(history.begin());
    if (history.size() > 1 && index < history.size()-1)
        history.erase(history.begin() + index, history.end());
    history.push_back(command);
    command->execute();
    index = history.size()-1;
    lastCommand = 0;
}

void CommandHistory::undo() {
    if (lastCommand == 1) {
        if (index > 0)
            index--;
    }
    history[index]->undo();
    lastCommand = 1;
}

void CommandHistory::redo() {
    if (lastCommand != 1) {
        if (index < history.size()-1)
            index++;
    }
    history[index]->redo();
    lastCommand = 2;
}

int CommandHistory::getIndex() {
    return index;
}

int CommandHistory::getLastCommand() {
    return lastCommand;
}

int CommandHistory::getSize() {
    return history.size();
}

}
