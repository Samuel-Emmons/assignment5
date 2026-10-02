#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

struct Task{
    int pid; // process ID
    int arrival_time; //when process enters system
    int burst_time; //total CPU time it needs

    int remaining_time; //CPU time still needed
    int start_time; //when it starts
    int end_time; //when it ends
    int waiting_time; //time spent waiting

    bool started; //has it ran yet?
    bool completed; //has it finished yet?
};

void fcfs(vector<Task>& tasks);

void sjf(vector<Task>& tasks);

void rr(vector<Task>& tasks, int quantum);

int main(int argc, char* argv[]) {

    if(argc < 3)
    {
        cout << "Usage: ./a.out input_file [FCFS|RR|SJF] [time_quantum]" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);

    if(!file)
    {
        cout << "error opening file" << endl;
        return 1;
    }

    int num_tasks;
    file >> num_tasks;

    vector<Task> tasks(num_tasks);

    //read task info from file
    for(int i = 0; i < num_tasks; i++)
    {
        file >> tasks[i].pid;
        file >> tasks[i].arrival_time;
        file >> tasks[i].burst_time;

        tasks[i].remaining_time = tasks[i].burst_time;
        tasks[i].start_time = -1;
        tasks[i].end_time = -1;
        tasks[i].waiting_time = 0;
        tasks[i].started = false;
        tasks[i].completed = false;
    }

    file.close();

    //run the specified algorithm
    if(argc == 3)
    {
        if(string(argv[2]) == "FCFS")
        {
            fcfs(tasks);
        }
        else if(string(argv[2]) == "SJF")
        {
            sjf(tasks);
        }

        return 0;
    }
    else if (argc == 4)
    {
        if(string(argv[2]) == "RR")
        {
            int quantum = stoi(argv[3]);
            rr(tasks, quantum);
        }

        return 0;
    }

    for(size_t i = 0; i < tasks.size(); i++)
    {
        cout << " PID: " << tasks[i].pid << endl;
        cout << " Arrival: " << tasks[i].arrival_time << endl;
        cout << " Burst: " << tasks[i].burst_time << endl;
    }

    cout << "Program Works!" << endl;

    return 0;
}

void fcfs(vector<Task>& tasks){
    int current_time = 0;
    size_t completed_count = 0;
    size_t num_of_tasks = tasks.size();

    while(completed_count < num_of_tasks)
    {
        Task* selected_task = nullptr;
        
        //find earliest task that hasn't been completed
        for(size_t i = 0; i < tasks.size(); i++)
        {
            if(!tasks[i].completed && tasks[i].arrival_time <= current_time)
            {
                if(selected_task == nullptr || tasks[i].arrival_time < selected_task->arrival_time)
                {
                    selected_task = &tasks[i];
                }
            }
        }

        //if no task is ready, CPU is idle
        if(selected_task == nullptr)
        {
            cout << "Time " << current_time << ": No task was selected." << endl;
            current_time++;
        }
        else
        {
            //run task till completed
            selected_task->start_time = current_time;

            cout << "Time " << current_time
                 << ": Process " << selected_task->pid
                 << " has started." << endl;

            current_time += selected_task->burst_time;

            selected_task->end_time = current_time;
            selected_task->completed = true;
            completed_count++;

            cout << "Time " << current_time
                 << ": Process " << selected_task->pid
                 << " has completed." << endl;
        }
    }

    double total_waiting_time = 0;

    //calculate avg wait time
    for(size_t i = 0; i < tasks.size(); i++)
    {
        tasks[i].waiting_time = tasks[i].end_time - tasks[i].arrival_time - tasks[i].burst_time;

        total_waiting_time += tasks[i].waiting_time;

        cout << "PID: " << tasks[i].pid << endl;
        cout << "Arrival Time: " << tasks[i].arrival_time << endl;
        cout << "Start Time: " << tasks[i].start_time << endl;
        cout << "End Time: " << tasks[i].end_time << endl;
        cout << "Running Time: " << tasks[i].burst_time << endl;
        cout << "Waiting Time: " << tasks[i].waiting_time << endl;
        cout << endl;
    }

    double average_waiting_time = total_waiting_time / tasks.size();

    cout << "Average Waiting Time: " << average_waiting_time << endl;

}

void sjf(vector<Task>& tasks)
{
    int current_time = 0;
    size_t completed_count = 0;
    size_t num_of_tasks = tasks.size();

    while(completed_count < num_of_tasks)
    {
        int selected_task = -1;
        
        //find task with the shortest burst time
        for(size_t i = 0; i < tasks.size(); i++)
        {
            if(!tasks[i].completed && tasks[i].arrival_time <= current_time)
            {
                if(selected_task == -1 || tasks[i].burst_time < tasks[selected_task].burst_time)
                {
                    selected_task = i;
                }
            }
        }

        //if no task is ready, cpu is idle
        if(selected_task == -1)
        {
            cout << "Cpu is idle right now" << endl;
            current_time++;
        }
        else
        {
            // run selected task till its finished
            tasks[selected_task].start_time = current_time;
            cout << tasks[selected_task].pid << " has started." << endl;
            current_time += tasks[selected_task].burst_time;
            tasks[selected_task].end_time = current_time;
            tasks[selected_task].completed = true;
            completed_count++;
            cout << tasks[selected_task].pid << " has completed." << endl;
        }
    }

    double total_waiting_time = 0;

    //calculate waiting times
    for(size_t i = 0; i < tasks.size(); i++)
    {
        tasks[i].waiting_time = tasks[i].end_time - tasks[i].arrival_time - tasks[i].burst_time;

        total_waiting_time += tasks[i].waiting_time;

        cout << "PID: " << tasks[i].pid << endl;
        cout << "Arrival Time: " << tasks[i].arrival_time << endl;
        cout << "Start Time: " << tasks[i].start_time << endl;
        cout << "End Time: " << tasks[i].end_time << endl;
        cout << "Running Time: " << tasks[i].burst_time << endl;
        cout << "Waiting Time: " << tasks[i].waiting_time << endl;
        cout << endl;
    }

    double average_waiting_time = total_waiting_time / tasks.size();

    cout << "Average Waiting Time: " << average_waiting_time << endl;
}

void rr(vector<Task>& tasks, int quantum){
    int current_time = 0;
    size_t completed_count = 0;
    size_t num_of_tasks = tasks.size();

    queue<int> ready_queue;

    vector<bool> added(tasks.size(), false);

    while(completed_count < num_of_tasks)
    {
        // Add tasks that have arrived
        for(size_t i = 0; i < tasks.size(); i++)
        {
            if(!added[i] && !tasks[i].completed && tasks[i].arrival_time <= current_time)
            {
                ready_queue.push(i);
                added[i] = true;
            }
        }

        // Nothing is ready
        if(ready_queue.empty())
        {
            cout << "Time " << current_time << ": No task was selected." << endl;
            current_time++;
            continue;
        }

        // Get task from front of queue
        int selected_task = ready_queue.front();
        ready_queue.pop();

        // Set start time only the first time process runs
        if(!tasks[selected_task].started)
        {
            tasks[selected_task].start_time = current_time;
            tasks[selected_task].started = true;
        }

        cout << "Time " << current_time
             << ": Process " << tasks[selected_task].pid
             << " has started." << endl;

        int run_time;

        // Decide whether task uses full quantum or finishes early
        if(tasks[selected_task].remaining_time > quantum)
        {
            run_time = quantum;
        }
        else
        {
            run_time = tasks[selected_task].remaining_time;
        }

        current_time += run_time;
        tasks[selected_task].remaining_time -= run_time;

        cout << "Time " << current_time
             << ": Process " << tasks[selected_task].pid
             << " stopped running." << endl;

        // Add any tasks that arrived while this task was running
        for(size_t i = 0; i < tasks.size(); i++)
        {
            if(!added[i] && !tasks[i].completed && tasks[i].arrival_time <= current_time)
            {
                ready_queue.push(i);
                added[i] = true;
            }
        }

        // Check if selected task finished
        if(tasks[selected_task].remaining_time == 0)
        {
            tasks[selected_task].end_time = current_time;
            tasks[selected_task].completed = true;
            completed_count++;

            cout << "Process " << tasks[selected_task].pid
                 << " has completed." << endl;
        }
        else
        {
            // Task is not finished, put it back in queue
            ready_queue.push(selected_task);
        }
    }

    double total_waiting_time = 0;

    //calculate waiting times
    for(size_t i = 0; i < tasks.size(); i++)
    {
        tasks[i].waiting_time = tasks[i].end_time - tasks[i].arrival_time - tasks[i].burst_time;

        total_waiting_time += tasks[i].waiting_time;

        cout << "PID: " << tasks[i].pid << endl;
        cout << "Arrival Time: " << tasks[i].arrival_time << endl;
        cout << "End Time: " << tasks[i].end_time << endl;
        cout << "Running Time: " << tasks[i].burst_time << endl;
        cout << "Waiting Time: " << tasks[i].waiting_time << endl;
        cout << endl;
    }

    double average_waiting_time = total_waiting_time / tasks.size();

    cout << "Average Waiting Time: " << average_waiting_time << endl;
}