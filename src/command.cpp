#include "command.h"

Command::~Command() {}


CommandHistory::CommandHistory() : index(-1) {
	maxHistory = 50;
}

CommandHistory::~CommandHistory() {
}

void CommandHistory::execute(Command* command) {
    cout << "CommandManager: execute" << endl;

    int size = history.size();
    cout << size << endl;

    if (size >= maxHistory) {
        delete history[0];
        history.erase(history.begin());
    }
    
    cout << size << endl;

    for (int i = size-1; i>index; i--) {
        delete history[i];
        history.erase(history.begin()+i);
    }
    cout << size << endl;

    history.push_back(command);
    command->execute();
    index = history.size()-1;

    cout << index << endl;
}

void CommandHistory::undo() {
    cout << "CommandManager: undo" << endl;
    
    if (index >= 0) {
        history[index]->undo();
        index--;
    }
    
    cout << index << endl;    
}

void CommandHistory::redo() {
    cout << "CommandManager: redo" << endl;
    
    if (index + 1 < history.size()) {
        index++;
        history[index]->execute();
    }
    cout << index << endl;
}

