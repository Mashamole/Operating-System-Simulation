#ifndef SIMULATION
#define SIMULATION

#include <iostream>
#include <string>
#include <stdlib.h>
#include <deque>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <cmath>
#include "process.hpp"

using std::cin;
using std::cout;
using std::endl;

struct HardDriveData
{

     Process p;
     std::string filename;
     int hd_space = 0;
     HardDriveData(Process _p, std::string fname, int HD_num) : p(_p), filename(fname), hd_space(HD_num){};
     HardDriveData() : p(0), filename(""), hd_space(-1) {}
     ~HardDriveData() {}
     bool checkIfEmpty()
     {
          if (filename.empty())
          {
               return true;
          }
          return false;
     }
     void display()
     {
          cout << " PID: " << p.getPID() << " "
               << "filename: " << filename << endl;
     }
     Process getProcess()
     {
          return p;
     }
};

struct memData
{
     //   private:
     int timestamp = 0;
     int page_num = 0;
     int p_size = 0;
     int prc_pid = 0;
     //   public:
     memData() {}
     memData(int pg_size)
     {
          p_size = pg_size;
     }
     ~memData() {}
};

class simulation
{
private:
     int Ram_space(long int SizeOfRam);

public:
     simulation() {}
     ~simulation() {}
     void startProgram();
     void StartQuery(Process &main_process, std::deque<Process> &readyQueue, std::deque<Process> &waitQueue, std::vector<HardDriveData> &hard_drive, std::deque<HardDriveData> &hd_wait, std::deque<memData> &ram);
     void DisplayValidCmds();
     int contains(std::deque<Process> &wq, int pid);
     void updateWaitQueue(std::deque<Process> &wq, Process &removed_process);
     Process ReturnParentProcess(std::deque<Process> &rq, std::deque<Process> &wq);
     void updateChildren(std::deque<Process> &cprocess, int _pid);
     bool ValidateInput(std::string &userIn);
     void updateHardDriveWait(std::deque<HardDriveData> &hdw, int hdSpace);
     void DefaultHd_wait(std::deque<HardDriveData> &hd_w);
     void DefaulHD(std::vector<HardDriveData> &hd);
     void DefaultRAM(std::deque<memData> &rm, int pg_size);
     void DefaultRAM_EXIT(std::deque<memData> &rm, int pid_check);
     void CascadeTermination(std::deque<Process> &rq, std::deque<memData> &rm, std::vector<HardDriveData> &hd, std::deque<HardDriveData> &hd_wait);
     void HardDriveCascadeTermination(std::vector<HardDriveData> &hd, int pid);
     void HardDriveWaitCascadeTermination(std::deque<HardDriveData> &hd, int pid);
};

void simulation::HardDriveWaitCascadeTermination(std::deque<HardDriveData> &hdw, int pid)
{

     HardDriveData defaultSet;

     for (unsigned int i = 0; i < hdw.size(); ++i)
     {
          if (hdw[i].p.getPID() == pid)
          {
               hdw[i] = defaultSet;
          }
     }
}

void simulation::HardDriveCascadeTermination(std::vector<HardDriveData> &hd, int pid)
{

     HardDriveData defaultSet;

     for (unsigned int i = 0; i < hd.size(); ++i)
     {
          if (hd[i].p.getPID() == pid)
          {
               hd[i] = defaultSet;
          }
     }
}

void simulation::CascadeTermination(std::deque<Process> &rq, std::deque<memData> &rm, std::vector<HardDriveData> &hd, std::deque<HardDriveData> &hd_wait)
{

     std::deque<Process>::iterator itr = rq.begin();

     for (; itr - 1 != rq.end(); ++itr)
     {

          for (unsigned int i = 0; i < rq[0].children.size(); ++i)
          {
               HardDriveCascadeTermination(hd, rq[0].children[i]);
               HardDriveWaitCascadeTermination(hd_wait, rq[0].children[i]);
               if (rq[0].children[i] == (*itr).getPID())
               {
                    DefaultRAM_EXIT(rm, (*itr).getPID());
                    rq.erase(itr);
               }
          }
     }
}

void simulation::DefaultRAM_EXIT(std::deque<memData> &rm, int pid_check)
{

     memData default_ram(rm[0].p_size);

     for (unsigned int i = 0; i < rm.size(); ++i)
     {
          if (rm[i].prc_pid == pid_check)
          {
               rm[i] = default_ram;
          }
     }
}

void simulation::DefaultRAM(std::deque<memData> &rm, int pg_size)
{

     memData default_ram(pg_size);

     for (unsigned int i = 0; i < rm.size(); ++i)
     {
          rm[i] = default_ram;
     }
}

void DefaultHd(std::deque<HardDriveData> &hd)
{

     HardDriveData set_default;
     for (unsigned int i = 0; i < hd.size(); ++i)
     {

          hd[i] = set_default;
     }
}

void DefaultHD_wait(std::deque<HardDriveData> &hd_w)
{

     HardDriveData set_default;
     for (unsigned int i = 0; i < hd_w.size(); ++i)
     {

          hd_w[i] = set_default;
     }
}

void simulation::updateHardDriveWait(std::deque<HardDriveData> &hdw, int hdSpace)
{
     std::deque<HardDriveData>::iterator itr = hdw.begin();

     for (; itr - 1 != hdw.end(); ++itr)
     {
          if ((*itr).hd_space == hdSpace)
          {
               hdw.erase(itr);
          }
     }
}

void simulation::updateWaitQueue(std::deque<Process> &wq, Process &removed_process)
{

     std::deque<Process>::iterator itr = wq.begin();

     for (; itr - 1 != wq.end(); ++itr)
     {
          if ((*itr).getPID() == removed_process.getPID())
          {
               wq.erase(itr);
          }
     }
}

Process simulation::ReturnParentProcess(std::deque<Process> &rq, std::deque<Process> &wq)
{

     for (unsigned int i = 0; i < wq.size(); ++i)
     {

          if (wq[i].getPID() == rq[0].getParentPID())
          {
               auto parent = wq[i];
               return parent;
          }
     }

     return rq[0];
}

void simulation::updateChildren(std::deque<Process> &cprocess, int _pid)
{
     unsigned int i = 0;
     std::vector<int>::iterator itr = cprocess[i].children.begin();

     for (; i < cprocess.size(); i++)
     {
          for (; itr - 1 != cprocess[i].children.end(); ++itr)
          {
               if (*itr == _pid)
               {
                    cprocess[i].children.erase(itr);
               }
          }
     }
}

int simulation::contains(std::deque<Process> &wq, int pid)
{
     for (unsigned int i = 0; i < wq.size(); ++i)
     {
          for (unsigned int j = 0; i < wq[i].children.size(); ++j)
          {
               if (wq[i].children[j] == pid)
               {
                    return pid;
               }
          }
     }
     return -1;
}

bool simulation::ValidateInput(std::string &userIn)
{
     if (userIn.empty())
     {
          return false;
     }

     for (unsigned int i = 0; i < userIn.size(); ++i)
     {

          if (!std::isdigit(userIn[i]))
          {
               return false;
          }
     }
     return true;
}

void simulation::startProgram()
{

     std::string ramSize, pageSize, hardDisk;

     std::cout << "\t\tWelcome to my OS simulation" << std::endl;
     std::cout << "\t\t Press [Enter] to continue" << std::endl;
     std::cin.get();
     std::system("clear");

     cout << "How much RAM memory is there on the simulated computer? => ";
     std::getline(cin, ramSize);

     while (true)
     {

          if (ValidateInput(ramSize) == true)
          {
               break;
          }
          cout << "How much RAM memory is there on the simulated computer? => ";
          std::getline(cin, ramSize);
     }
     auto rsize = std::stoll(ramSize);

     std::cout << "What is the size of a page/frame? => ";
     std::getline(cin, pageSize);

     while (true)
     {

          if (ValidateInput(pageSize) == true)
          {
               break;
          }
          std::cout << "What is the size of a page/frame? => ";
          std::getline(cin, pageSize);
     }

     auto psize = std::stoi(pageSize);

     std::cout << "How many hard disks does the simulated computer have? => ";
     std::getline(cin, hardDisk);

     while (true)
     {

          if (ValidateInput(hardDisk) == true)
          {
               break;
          }
          std::cout << "How many hard disks does the simulated computer have? => ";
          std::getline(cin, hardDisk);
     }

     auto hdsize = std::stoi(hardDisk);

     std::vector<HardDriveData> hd(hdsize);
     Process originalProcess(1);
     originalProcess.setParent(1);
     std::deque<Process> readyQueue;
     std::deque<Process> waitQueue;
     HardDriveData setDefault;
     std::deque<HardDriveData> HD_wait(hdsize * 2);
     DefaultHD_wait(HD_wait);
     std::deque<memData> memory(rsize / psize);
     DefaultRAM(memory, psize);

     std::system("clear");
     StartQuery(originalProcess, readyQueue, waitQueue, hd, HD_wait, memory);
}

void simulation::StartQuery(Process &main_process, std::deque<Process> &readyQueue, std::deque<Process> &waitQueue, std::vector<HardDriveData> &hard_drive, std::deque<HardDriveData> &hd_wait, std::deque<memData> &ram)
{

     std::string command;
     int p_num = 1;
     int t_stamp = 1;

     while (true)
     {

          cout << "\nEnter Command => ";
          std::getline(cin, command);
          if (command.empty())
          {
               continue;
          }

          if (command == "quit")
          {
               system("clear");
               break;
          }
          else if (command == "fork")
          {
               ++p_num;
               auto child = readyQueue.front().fork(readyQueue[0].getPID());
               child.setPID(p_num);
               readyQueue[0].children.push_back(child.getPID());
               readyQueue.push_back(child);
          }
          else if (command == "A" || command == "a")
          {
               ++p_num;
               auto new_process = main_process.newProcess();
               new_process.setPID(p_num);
               readyQueue.push_back(new_process);
          }
          else if (command == "Q" || command == "q")
          { //moves current running process to back of Ready queue

               auto moveProcess = readyQueue[0];
               readyQueue.pop_front();
               readyQueue.push_back(moveProcess);
          }
          else if (command == "exit")
          {
               if (readyQueue.size() == 0)
               {
                    system("clear");
                    cout << "\n Ready Queue is currently empty" << endl;
                    continue;
               }
               if (contains(waitQueue, readyQueue[0].getPID()) == readyQueue[0].getPID())
               {
                    auto moveToRq = ReturnParentProcess(readyQueue, waitQueue);
                    updateWaitQueue(waitQueue, moveToRq); //remove process from wait queue

                    if (moveToRq.getPID() == readyQueue[0].getPID())
                    {
                         system("clear");
                         std::cout << "child is an orphan process" << std::endl;
                         continue;
                    }
                    readyQueue.push_back(moveToRq);

                    for (unsigned int i = 0; i < waitQueue.size(); ++i)
                    {
                         cout << waitQueue[i].getPID() << endl;
                    }
                    updateChildren(waitQueue, readyQueue[0].getPID());
               }
               DefaultRAM_EXIT(ram, readyQueue[0].getPID());             //delete entries currently used by cpu
               CascadeTermination(readyQueue, ram, hard_drive, hd_wait); //parents exits, and all children terminate
               readyQueue.pop_front();
          }
          else if (command == "S r" || command == "s r")
          {

               cout << "\nIn CPU" << endl;
               cout << "[" << readyQueue[0].getPID() << "]"
                    << "\n"
                    << endl;

               cout << "Ready Queue" << endl;
               for (unsigned int i = 1; i < readyQueue.size(); i++)
               {

                    cout << "[";
                    cout << readyQueue[i].getPID() << "]";
                    if (i != readyQueue.size() - 1)
                         cout << " -> ";
               }
               cout << "\n";

          }

          else if (command == "wait" || command == "Wait")
          {

               if (!readyQueue[0].children.empty())
               {

                    auto waitprocess = readyQueue[0];
                    waitQueue.push_front(waitprocess);
                    readyQueue.pop_front();
               }
               else
               {
                    system("clear");
                    cout << "Process does not have child" << endl;
               }
          }
          else if (command[0] == 'd')
          {

               std::stringstream parse(command);
               std::string cmd, hd_num, fname;
               parse >> cmd >> hd_num >> fname;
               if (hd_num.empty() || fname.empty())
               {
                    continue;
               }

               auto hd_number = stoi(hd_num);
               if (hd_number > (int)(hard_drive.size() - 1))
               {
                    system("clear");
                    cout << "Hard Drive " << hd_number << " does not exist." << endl;
                    continue;
               }

               auto process_hd = readyQueue[0];
               readyQueue.pop_front();
               HardDriveData data(process_hd, fname, hd_number);

               if (hard_drive[hd_number].hd_space == -1)
               {
                    hard_drive[hd_number] = data;
               }
               else
               {
                    hd_wait.push_back(data);
               }
          }
          else if (command[0] == 'D')
          {

               std::stringstream parse(command);
               HardDriveData setDefault;
               std::string cmd, HD_NUM;
               parse >> cmd >> HD_NUM;
               if (HD_NUM.empty() || HD_NUM == " ")
               {
                    continue;
               }

               auto HD_NUMBER = stoi(HD_NUM);

               if (HD_NUMBER > (int)(hard_drive.size() - 1))
               {
                    system("clear");
                    cout << "Hard Drive " << HD_NUMBER << " does not exist." << endl;
                    continue;
               }

               auto process = hard_drive[HD_NUMBER];
               hard_drive[HD_NUMBER] = setDefault;

               for (unsigned int i = 0; i < hd_wait.size(); ++i)
               {
                    if (hd_wait[i].hd_space == HD_NUMBER)
                    {
                         hard_drive[HD_NUMBER] = hd_wait[i];
                         updateHardDriveWait(hd_wait, HD_NUMBER);
                    }
               }
               readyQueue.push_back(process.p);
          }
          else if (command == "S i" || command == "s i")
          {

               //Prints processes that are currently using the Hard Disk
               for (unsigned int i = 0; i < hard_drive.size(); ++i)
               {

                    if (hard_drive[i].hd_space != -1)
                    {
                         cout << "\nHard Drive " << i << ": "
                              << "PID => " << hard_drive[i].p.getPID() << endl;
                    }
               }
               cout << "-------------------------------\n";
               cout << "In Hard Drive Queue:" << endl;
               for (unsigned int j = 0; j < hd_wait.size(); ++j)
               {
                    if (hd_wait[j].hd_space != -1 && hd_wait[j].p.getPID() != 0)
                    {
                         cout << "Waiting for Hard Drive: " << hd_wait[j].hd_space << " PID => " << hd_wait[j].p.getPID() << endl;
                    }
               }
          }
          else if (command[0] == 'm')
          {

               std::stringstream parse(command);
               std::string cmd, addr;
               parse >> cmd >> addr;
               if (addr.empty())
               {
                    continue;
               }
               auto address = stoi(addr);
               auto p = readyQueue[0].getPID();
               unsigned int position = floor((address / ram[0].p_size));

               if (position > ram.size())
               {
                    ram[ram.size() - 1].prc_pid = p;
                    ram[ram.size() - 1].timestamp = t_stamp;
                    ram[ram.size() - 1].page_num = position;

                    ++t_stamp;
               }
               else
               {
                    ram[position].prc_pid = p;
                    ram[position].timestamp = t_stamp;
                    ram[position].page_num = position;

                    ++t_stamp;
               }
          }
          else if (command == "S m" || command == "s m")
          {

               cout << "\nFrame#"
                    << "\tPID"
                    << "\tPage#"
                    << "\tTimeStamp" << endl;
               for (unsigned int i = 0; i < ram.size(); ++i)
               {

                    cout << i << "\t" << ram[i].prc_pid << "\t" << ram[i].page_num << "\t" << ram[i].timestamp << endl;
               }
          }
          else
          {
               continue;
          }
     }
}

void simulation::DisplayValidCmds()
{

     cout << "--Commands--"
          << " \t\t\t"
          << "--Description-- " << endl;
     cout << "[A]  \t\t\tinput means that a new process has been created.\n"
          << endl;
     cout << "[Q]   \t\t\tCurrent running process should be preempted to the end of the ready-queue.\n"
          << endl;
     cout << "[fork]   \t\tThe process forks a child.\n"
          << endl;
     cout << "[exit]   \t\tThe process that is currently using the CPU terminates.\n"
          << endl;
     cout << "[wait]    \t\tThe process wants to pause and wait for any of its child processes to terminate.\n"
          << endl;
     cout << "[d number file_name]    The process that currently uses the CPU requests the hard disk #number.\n"
          << endl;
     cout << "[D number]   \t\tThe hard disk #number has finished the work for one process.\n"
          << endl;
     cout << "[m address]   \t\tThe process that is currently using the CPU requests a memory operation for the logical address.\n"
          << endl;
     cout << "[S r]   \t\tShows what process is currently using the CPU and what processes are waiting in the ready-queue.\n"
          << endl;
     cout << "[S i]   \t\tShows what processes are currently using the hard disks and what processes are waiting to use them.\n"
          << endl;
     cout << "[S m]    \t\tShows the state of memory.\n"
          << endl;

     cout << "[quit]   \t\tends simulation program. " << endl;
}

#endif