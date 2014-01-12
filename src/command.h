#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Command {
public:
    virtual ~Command();
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class CommandHistory {
public :
    CommandHistory();
    ~CommandHistory();
    void execute(Command* command);
    void undo();
    void redo();
private :
    vector<Command*> history;
    int index;
    int maxHistory = 50;
};

