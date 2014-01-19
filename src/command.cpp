#include "command.h"

Command::~Command() {}


CommandHistory::CommandHistory() : index(-1) {
	maxHistory = 50;
}

CommandHistory::~CommandHistory() {
}

void CommandHistory::execute(Command* command) {
    int size = history.size();

    if (size >= maxHistory) {
        delete history[0];
        history.erase(history.begin());
    }
    for (int i = size-1; i>index; i--) {
        delete history[i];
        history.erase(history.begin()+i);
    }
    
    history.push_back(command);
    command->execute();
    index = history.size()-1;
}

void CommandHistory::undo() {    
    if (index >= 0) {
        history[index]->undo();
        index--;
    }
}

void CommandHistory::redo() {    
    if (index + 1 < history.size()) {
        index++;
        history[index]->execute();
    }
}