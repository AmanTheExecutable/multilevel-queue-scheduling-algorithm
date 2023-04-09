// I have assumed arrival times as 0, so turn around and completion times are same.

#include <iostream>
using namespace std;

class process
{
public:
    int priority;       // priority of process
    int burst_time;     // burst time of process
    int tt_time;        // turnaround time of process
    int total_time = 0; // Completion time of process
};

class queues
{
public:
    int priority_start;    // start of priority range
    int priority_end;      // end of priority range
    int total_time = 0;    // total time of queue took to execute
    int length = 0;        // length of queue
    process *p;            // pointer to array of processes inside the queue
    bool executed = false; // to check if queue is executed or not
};

bool notComplete(queues q[]) // to check if all queues are executed or not
{
    bool a = false;
    int countInc = 0;
    for (int i = 0; i < 3; i++)
    {
        countInc = 0;
        for (int j = 0; j < q[i].length; j++)
        {
            if (q[i].p[j].burst_time != 0)
            {
                a = true;
            }
            else
            {
                countInc += 1;
            }
        }
        if (countInc == q[i].length)
        {

            q[i].executed = true;
        }
    }
    return a;
}

void sort_q2(queues q) // to sort processes in queue in which priority scheduling is to be implemented according to priority
{

    for (int i = 1; i < q.length; i++)
    {
        for (int j = 0; j < q.length - 1; j++)
        {
            if (q.p[j].priority < q.p[j + 1].priority)
            {
                process temp = q.p[j + 1];
                q.p[j + 1] = q.p[j];
                q.p[j] = temp;
            }
        }
    }
}

void checkCompleteTimer(queues q[]) // to check if any process is completed in any queue or not and if completed then to increment total time of queue
{
    bool a = notComplete(q);
    for (int i = 0; i < 3; i++)
    {
        if (q[i].executed == false)
        {
            for (int j = 0; j < q[i].length; j++)
            {
                if (q[i].p[j].burst_time != 0)
                {
                    q[i].p[j].total_time += 1;
                }
            }
            q[i].total_time += 1;
        }
    }
}

int main()
{
    // Initializing 3 queues and assigning specific range of priority to every queue.

    queues q[3]; // q[0] = highest priority queue, q[1] = medium priority queue, q[2] = lowest priority queue
    q[0].priority_start = 9;
    q[0].priority_end = 12;
    q[1].priority_start = 5;
    q[1].priority_end = 8;
    q[2].priority_start = 1;
    q[2].priority_end = 4;

    int no_of_processes, priority_of_process, burst_time_of_process;
    cout << "Enter the number of processes: ";
    cin >> no_of_processes;
    process p1[no_of_processes];

    for (int i = 0; i < no_of_processes; i++)
    {
        cout << "Enter the priority of the process " << i + 1 << " : ";
        cin >> priority_of_process;
        cout << "Enter the burst time of the process " << i + 1 << " : ";
        cin >> burst_time_of_process;
        p1[i].priority = priority_of_process;
        p1[i].burst_time = burst_time_of_process;
        p1[i].tt_time = burst_time_of_process;
        for (int j = 0; j < 3; j++) // to assign processes to respective queues
        {
            if (q[j].priority_start <= priority_of_process && priority_of_process <= q[j].priority_end)
            {
                q[j].length++;
            }
        }
    }

    for (int i = 0; i < 3; i++) // to allocate memory to array of processes in every queue
    {
        int len = q[i].length;
        q[i].p = new process[len];
    }

    int a = 0;
    int b = 0;
    int c = 0;

    for (int i = 0; i < 3; i++) // to assign processes to respective queues
    {
        for (int j = 0; j < no_of_processes; j++)
        {
            if ((q[i].priority_start <= p1[j].priority) && (p1[j].priority <= q[i].priority_end))
            {
                if (i == 0) // to assign processes to queue 1
                {
                    q[i].p[a++] = p1[j];
                }
                else if (i == 1) // to assign processes to queue 2
                {
                    q[i].p[b++] = p1[j];
                }
                else // to assign processes to queue 3
                {
                    q[i].p[c++] = p1[j];
                }
            }
        }
    }

    a--; // to decrement the value of a, b and c as they are incremented in the above for loop
    b--;
    c--;

    for (int i = 0; i < 3; i++) // to print the processes in every queue
    {
        cout << "Queue " << i + 1 << " : ";
        for (int j = 0; j < q[i].length; j++)
        {
            cout << q[i].p[j].priority << "->";
        }
        cout << "NULL\n";
    }

    // While Round robin on all three queues is not complete, keep on repeating

    int timer = 0;       // to keep track of time
    int l = 0;           // to keep track of which queue is being processed
    int rr_timer = 4;    // to keep track of time for round robin for queue 1
    int counter = 0;     // to keep track of number of processes completed in queue 1
    int counterps = 0;   // to keep track of number of processes completed in queue 2
    int counterfcfs = 0; // to keep track of number of processes completed in queue 3

    while (notComplete(q)) // while any queue is not completed and still has some processes to execute
    {
        if (timer == 10) // to check if 10 seconds have passed or not
        {
            timer = 0;
        }
        // l += 1;
        // if (l >= 3)
        // {
        //     l = l % 3;
        // }

        /* Process lth queue if its already not executed.
        If its executed change the value of l and move to next queue */

        if (q[l].executed == true)
        {
            cout << "Queue " << l + 1 << " completed\n";
            l += 1;
            if (l >= 3)
            {
                l = l % 3;
            }
            continue;
        }

        // Finally you now have a queue which is not completely executed
        // Process the incomplete processes over it

        if (l == 0) // to execute queue 1 i.e Round Robin
        {
            cout << "Queue " << l + 1 << " in hand\n";
            if (rr_timer == 0)
            {
                rr_timer = 4;
            }

            for (int i = 0; i < q[l].length; i++) // to execute all the processes in queue 1
            {
                if (q[l].p[i].burst_time == 0)
                {
                    counter++;
                    continue;
                }
                if (counter == q[l].length) // to check if all the processes in queue 1 are completed or not
                {
                    break;
                }
                while (rr_timer > 0 && q[l].p[i].burst_time != 0 && timer != 10) // to execute the process for a unit time
                {
                    int temp = q[l].p[i].burst_time;
                    cout << "Executing queue 1 and process " << i + 1 << " for a unit time. Process has priority of " << q[l].p[i].priority << " . Remaining burst time " << temp - 1 << "\n";
                    q[l].p[i].burst_time--;
                    checkCompleteTimer(q);
                    rr_timer--;
                    timer++;
                }
                if (timer == 10) // to check if 10 seconds have passed or not if yes then go to next queue
                {
                    break;
                }
                if (q[l].p[i].burst_time == 0 && rr_timer == 0) // to check if the process is completed and if yes then go to next process
                {
                    rr_timer = 4;
                    if (i == (q[i].length - 1))
                    {
                        i = -1;
                    }
                    continue;
                }
                if (q[l].p[i].burst_time == 0 && rr_timer > 0) // to check if the process is completed and if yes then go to next process
                {
                    if (i == (q[i].length - 1))
                    {
                        i = -1;
                    }
                    continue;
                }
                if (rr_timer <= 0) // to check if the time for round robin is over and if yes then go to next process
                {
                    rr_timer = 4;
                    if (i == (q[i].length - 1))
                    {
                        i = -1;
                    }
                    continue;
                }
            }
        }

        else if (l == 1) // to execute queue 2 i.e priority queue
        {
            cout << "Queue " << l + 1 << " in hand\n";
            sort_q2(q[l]);                        // to sort processes in queue 2 according to their priority
            for (int i = 0; i < q[l].length; i++) // to execute all processes in queue 2
            {
                if (q[l].p[i].burst_time == 0) // to check if process is already completed
                {
                    counterps++;
                    continue;
                }
                if (counterps == q[l].length) // to check if all processes in queue 2 are completed
                {
                    break;
                }
                while (q[l].p[i].burst_time != 0 && timer != 10) // to execute process for a unit time
                {
                    int temp = q[l].p[i].burst_time;
                    cout << "Executing queue 2 and process " << i + 1 << " for a unit time. Process has priority of " << q[l].p[i].priority << " . Remaining burst time " << temp - 1 << "\n";
                    q[l].p[i].burst_time--;
                    checkCompleteTimer(q); // to check if any process in any queue is completed
                    timer++;
                }
                if (timer == 10) // to check if 10 seconds have passed or not
                {
                    break;
                }
                if (q[l].p[i].burst_time == 0) // to check if process is completed
                {
                    continue;
                }
            }
        }
        else // to execute queue 3 i.e FCFS
        {
            cout << "Queue " << l + 1 << " in hand\n";
            for (int i = 0; i < q[l].length; i++)
            {
                if (q[l].p[i].burst_time == 0) // to check if process is already completed
                {
                    counterfcfs++;
                    continue;
                }
                if (counterfcfs == q[l].length) // to check if all processes in queue 3 are completed
                {
                    break;
                }
                while (q[l].p[i].burst_time != 0 && timer != 10) // to execute process for a unit time
                {
                    int temp = q[l].p[i].burst_time;
                    cout << "Executing queue 3 and process " << i + 1 << " for a unit time. Process has priority of " << q[l].p[i].priority << " . Remaining burst time " << temp - 1 << "\n";
                    q[l].p[i].burst_time--;
                    checkCompleteTimer(q); // to check if any process in any queue is completed

                    timer++;
                }
                if (timer == 10)
                {
                    break;
                }
                if (q[l].p[i].burst_time == 0)
                {
                    continue;
                }
            }
        }
        cout << "Broke from queue " << l + 1 << "\n";
    }

    for (int i = 0; i < 3; i++) // to print total time taken by each queue
    {
        cout << "\nTime taken for queue " << i + 1 << " to execute: " << q[i].total_time << "\n";
        for (int j = 0; j < q[i].length; j++)
        {
            cout << "Process " << j + 1 << " of queue " << i + 1 << " took " << q[i].p[j].total_time + 1 << " unit time to complete execution."
                 << "\n";
        }
    }

    float sum_tt = 0;
    float sum_wt = 0;

    cout << "\n\nProcess     | Turn Around Time | Waiting Time\n";
    for (int i = 0; i < 3; i++)
    {
        cout << "Queue " << i + 1 << "\n";
        for (int j = 0; j < q[i].length; j++)
        {
            cout << "Process P" << j + 1 << "\t" << q[i].p[j].total_time + 1 << "\t\t    " << q[i].p[j].total_time - q[i].p[j].tt_time + 1 << "\n";
            sum_tt += q[i].p[j].total_time;
            sum_wt += q[i].p[j].total_time - q[i].p[j].tt_time;
        }
        cout << "\n";
    }

    cout << "\n The average turnaround time is : " << sum_tt / no_of_processes + 1 << endl;
    cout << "\n The average waiting time is : " << sum_wt / no_of_processes + 1 << endl;

    return 0;
}
