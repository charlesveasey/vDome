#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Command {
public:
    virtual ~Command();
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class CommandHistory {
public :
    CommandHistory();
    ~CommandHistory();
    void execute(Command* command);
    void undo();
    void redo();
    int getIndex();
    int getLastCommand();
    int getSize();
    
private :
    vector<Command*> history;
    int index;
    int maxHistory;
    int lastCommand; // 0 = exec, 1 = undo, 2 = redo
};

