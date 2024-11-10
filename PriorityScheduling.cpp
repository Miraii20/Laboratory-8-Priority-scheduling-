#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip> 

struct Process {
    int processID;
    int arrivalTime;
    int burstTime;
    int originalBurstTime; 
    int priority;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

void calculateTimesPreemptive(std::vector<Process>& processes) {
    int time = 0, completed = 0;
    int n = processes.size();
    std::vector<bool> isCompleted(n, false);

    while (completed < n) {
        int highestPriorityIdx = -1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= time && !isCompleted[i]) {
                if (highestPriorityIdx == -1 || processes[i].priority > processes[highestPriorityIdx].priority) {
                    highestPriorityIdx = i;
                }
            }
        }

        if (highestPriorityIdx != -1) {
            processes[highestPriorityIdx].burstTime--;
            time++;
            if (processes[highestPriorityIdx].burstTime == 0) {
                isCompleted[highestPriorityIdx] = true;
                processes[highestPriorityIdx].completionTime = time;
                processes[highestPriorityIdx].turnaroundTime = processes[highestPriorityIdx].completionTime - processes[highestPriorityIdx].arrivalTime;
                processes[highestPriorityIdx].waitingTime = processes[highestPriorityIdx].turnaroundTime - processes[highestPriorityIdx].originalBurstTime;
                completed++;
            }
        } else {
            time++;
        }
    }
}

void calculateTimesNonPreemptive(std::vector<Process>& processes) {
    int time = 0, completed = 0;
    int n = processes.size();

    while (completed < n) {
        int highestPriorityIdx = -1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= time && processes[i].completionTime == 0) {
                if (highestPriorityIdx == -1 || processes[i].priority > processes[highestPriorityIdx].priority) {
                    highestPriorityIdx = i;
                }
            }
        }

        if (highestPriorityIdx != -1) {
            time += processes[highestPriorityIdx].burstTime;
            processes[highestPriorityIdx].completionTime = time;
            processes[highestPriorityIdx].turnaroundTime = processes[highestPriorityIdx].completionTime - processes[highestPriorityIdx].arrivalTime;
            processes[highestPriorityIdx].waitingTime = processes[highestPriorityIdx].turnaroundTime - processes[highestPriorityIdx].burstTime;
            completed++;
        } else {
            time++;
        }
    }
}

void displayResults(const std::vector<Process>& processes) {
    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;
    int n = processes.size();

    std::cout << std::left << std::setw(8) << "Process"
              << std::setw(8) << "Arrival"
              << std::setw(8) << "Burst"
              << std::setw(10) << "Priority"
              << std::setw(12) << "Completion"
              << std::setw(10) << "Waiting"
              << std::setw(12) << "Turnaround"
              << "\n";

    for (const auto& p : processes) {
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
        std::cout << std::left << std::setw(8) << ("P" + std::to_string(p.processID))
                  << std::setw(8) << p.arrivalTime
                  << std::setw(8) << p.originalBurstTime
                  << std::setw(10) << p.priority
                  << std::setw(12) << p.completionTime
                  << std::setw(10) << p.waitingTime
                  << std::setw(12) << p.turnaroundTime
                  << "\n";
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nAverage Waiting Time: " << totalWaitingTime / n << "\n";
    std::cout << "Average Turnaround Time: " << totalTurnaroundTime / n << "\n";
}

int main() {
    int n, choice;
    std::cout << "Enter the number of processes: ";
    std::cin >> n;

    std::vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        processes[i].processID = i + 1;
        std::cout << "Enter arrival time, burst time, and priority for process " << i + 1 << ": ";
        std::cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
        processes[i].originalBurstTime = processes[i].burstTime; // Store original burst time
    }

    std::cout << "Choose Scheduling Type:\n";
    std::cout << "1. Preemptive Priority Scheduling\n";
    std::cout << "2. Non-Preemptive Priority Scheduling\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    if (choice == 1) {
        calculateTimesPreemptive(processes);
    } else if (choice == 2) {
        calculateTimesNonPreemptive(processes);
    } else {
        std::cout << "Invalid choice.\n";
        return 1;
    }

    displayResults(processes);

    return 0;
}
