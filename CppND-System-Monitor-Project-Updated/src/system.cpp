#include "system.h"

#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>
#include <set>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::size_t;
using std::string;
using std::vector;
using std::set;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids{LinuxParser::Pids()};
  updateProcesses(pids);
  updateCpuUtilizations();
  std::sort(processes_.rbegin(), processes_.rend(), std::less<Process>());
  return processes_;
}


void System::updateProcesses(const vector<int>& pids) {
  for (const auto& pid : pids) {
      if (pids_.find(pid) == pids_.end()) {
          pids_.insert(pid);
          processes_.emplace_back(pid);
      }
  }
}


void System::updateCpuUtilizations() {
  for (auto& process : processes_) {
    const long process_jiffies = LinuxParser::ActiveJiffies(process.Pid());
    const long system_jiffies = LinuxParser::Jiffies();
    process.CpuUtilization(process_jiffies, system_jiffies);
  }
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
