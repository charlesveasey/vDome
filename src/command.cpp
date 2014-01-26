#include "command.h"

Command::~Command() {}


CommandHistory::CommandHistory() : index(0) {
	maxHistory = 100;
}

CommandHistory::~CommandHistory() {
}

void CommandHistory::execute(Command* command) {
    if (history.size() >= maxHistory) {
        history.erase(history.begin());
    }    
    if (history.size() > 1 && index < history.size()-1)
        history.erase(history.begin() + index+1, history.end());
    
    history.push_back(command);
    command->execute();
    index = history.size()-1;
    //std::cout <<"-----"<< index << endl;
}

void CommandHistory::undo() {
    if (index > 0) {
        index--;
        history[index]->undo();
        //std::cout << index << endl;
    }
}

void CommandHistory::redo() {    
    if (index + 1 < history.size()) {
        index++;
        history[index]->execute();
        //std::cout << index << endl;
    }
}