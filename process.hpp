#ifndef PROCESS
#define PROCESS

#include <vector>
#include <iostream>
#include <deque>

class Process
{

private:
    int PID;
    int Parent_pid = 0;
    int process_counter = 1;
    //PCB information PID,P-States

public:
    Process(int PID_Num);
    Process(int PID_num, int parentPid);
    Process() {}
    ~Process() {}
    bool check_parent(); //checks PID of parent process from child
    std::vector<int> children;
    int getParentPID();
    void setParent(int PID); //setter for linking the parent to child
    void setPID(int PID);
    const int getPID();
    void childrenVeiw();

    Process fork(int parentPID); //Spawns child process
    Process newProcess();
};

Process::Process(int PID_num, int parentPid) : PID(PID_num), Parent_pid(parentPid), process_counter(1) {}
Process::Process(int PID_Num) : PID(PID_Num), Parent_pid(0), process_counter(1) {}

Process Process::fork(int parentPID)
{

    Process childProcess;
    childProcess.setParent(parentPID);
 

    return childProcess;
}

const int Process::getPID()
{
    return PID;
}

void Process::setPID(const int pid)
{

    PID = pid;
}

void Process::setParent(const int pid_parent)
{

    Parent_pid = pid_parent;
}

int Process::getParentPID()
{

    return Parent_pid;
}

void Process::childrenVeiw()
{

    for (unsigned int i = 0; i < children.size(); i++)
    {
        if (children[i] != -1)
        {
            std::cout << "[";
            std::cout << children[i] << "]";
            if (i != children.size() - 1)
                std::cout << " -> ";
        }
    }
}

Process Process::newProcess()
{

    Process newInstance;
    ++process_counter;
    newInstance.setPID(process_counter);

    return newInstance;
}

#endif