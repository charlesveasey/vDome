#include "command.h"

Command::~Command() {}


CommandHistory::CommandHistory() : index(0) {
	maxHistory = 100;
}

CommandHistory::~CommandHistory() {
}

void CommandHistory::execute(Command* command) {
    if (history.size() >= maxHistory)
        history.erase(history.begin());
    if (history.size() > 1 && index < history.size()-1)
        history.erase(history.begin() + index+1, history.end());

    history.push_back(command);
    command->execute();
    index = history.size()-1;
}

void CommandHistory::undo() {
    if (index < history.size())
        history[index]->undo();
    if (index > 0)
        index--;
}

void CommandHistory::redo() {    
    if (index + 1 < history.size()) {
        index++;
        history[index]->redo();
    }
}

int CommandHistory::getIndex() {
    return index;
}