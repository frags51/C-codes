/**
 * CS16BTECH11038 Supreet Singh
 * OS-2 Assignment 1
 * Earliest Deadline First Scheduling
 * Assuming preemption takes ZERO time.
 * Assuming Process can start executing at the same time it comes in.
 * Assuming Processes don't get killed if they miss their deadline.
 * Assuming if period (deadline) is same then earliest to enter system is higher priority.
 * Assuming if dline is t=50, process has to finish on or before t=49.
 */ 

#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <fstream>

class Process{ // A class to represent a process
    public:
    //static int no; // The number of processes currently in system.
    int pid; // Pid of the process
    int t; // Processing Time
    int p; // Period
    int k; // Number of times it appears
    int st; // Start time of the process
    int dline; // The deadline (absolute) of the process
    Process(int pid_, int t_, int p_, int k_, int st_) : pid{pid_}, t{t_}, p{p_}, k{k_}, st{st_} {
        dline=st+p;
    }  // constructor
    Process() : pid{-1} {} // -1 indicates not a valid process.
    int wait_time {0}; // Time spent waiting.
    int end_time; // Time whent it ends
    int run_time {0}; // Total time spent running till now.
    
    bool pr_lt(const Process& a){ // return true if this->PRIORITY < a.PRIORITY
        if(this->dline < a.dline) return false;
        else if(this->dline > a.dline) return true;
        else return !(this->st<a.st);
    } // pr_gt
}; // class Process

struct prTimeCompare{ // Custom Functor for priority Queue :: Lowest start time, then lowest priority.
    bool operator()(const Process&b, const Process& a){
        if(b.st<a.st) return false;
        else if(b.st>a.st) return true;
        else return !(b.dline<a.dline);
    }
};

struct prPrCompare{ // Custom Functor based on priority (not start time)
    bool operator()(const Process& b, const Process& a){
        if(b.dline < a.dline) return false;
        else if(b.dline > a.dline) return true;
        else return !(b.st<a.st);
    }
}; 

void print_q(std::priority_queue<Process, std::vector<Process>, prTimeCompare> all){ // To print the queue
        while(!all.empty()){
        std::cout<<"Pid: "<<(all.top().pid)<<" st: "<<(all.top().st) <<" Per:" <<(all.top().p)<<"\n";
        all.pop();
    }
    std::cout<<std::endl;
}

int main(void){
    std::priority_queue<Process, std::vector<Process>, prTimeCompare> all; // Queue containing all the processes
    std::priority_queue<Process, std::vector<Process>, prPrCompare> rq; // The ready-queue: waiting for exec
    std::cout<<"CS16BTECH11038 : Supreet Singh : Earliest Deadline First Scheduling\n";
    std::ifstream infile("inp-params.txt");
    int n,size; // no of lines to take input
    if(infile.is_open()){ // if opened file succsessfully

        infile>>n;
        size=n;
        while(n>0){ // while still lines left to read.
            int pid_, t_, p_, k_;
            infile>>pid_>>t_>>p_>>k_;
            for(int i=0; i<k_; i++){
                Process p1{pid_, t_, p_, k_, i*p_};
                all.push(p1);
            }
            n--;
        } //while
    }//if
    infile.close();
    std::cout<<"Debug Info: \n";
    print_q(all);

    /**************************** Begin Simulation *********************************/
    std::cout<<"*********** Begin Simulation! ***************"<<std::endl;
    std::queue<Process> fin; // Contains processes that have finished execution.
    std::ofstream _log("EDF-Log.txt");
    int cur_time=0;
    //bool was_idle=false;
    int idle_sec=0;
    Process running; // The process which is running currently
    while(true){ // Simulator loop
        while(!all.empty() && all.top().st==cur_time){
            if(idle_sec>1) _log<<"CPU is idle till time "<<(cur_time-1)<<std::endl;
            if(idle_sec>1) std::cout<<"CPU is idle till time "<<(cur_time-1)<<std::endl;
            Process p1=all.top();
            _log<<"Process P"<<p1.pid<<": processing time="<<p1.t<<"; deadline="<<(p1.st+p1.p)<<"; period="<<p1.p;
            _log<<" joined the systen at time "<<cur_time<<std::endl;
            std::cout<<"Process P"<<p1.pid<<": processing time="<<p1.t<<"; deadline="<<(p1.st+p1.p)<<"; period="<<p1.p;
            std::cout<<" joined the systen at time "<<cur_time<<std::endl;
            rq.push(p1);
            all.pop();
        }

        if(running.pid==-1 && rq.empty() && all.empty()){ // Stop simulation if nothing left.
            idle_sec++;
            break;
        }

        if(running.pid==-1){
            idle_sec++;
            //std::cout<<"\tDEBG: idle_sec_nw: "<<idle_sec<<std::endl;
        }
         
        if(idle_sec > 0 && !rq.empty()){
            idle_sec=0;
            running=rq.top();
            rq.pop();
            running.run_time++;
            _log<<"Process P"<<running.pid<<" starts execution at time "<<cur_time<<std::endl;
            std::cout<<"Process P"<<running.pid<<" starts execution at time "<<cur_time<<std::endl;
        }
        else if(!rq.empty()){ // was not idle earlier
            Process p2=rq.top();
            if(!running.pr_lt(p2)){
                running.run_time++;
            }
            else{
                Process temp = rq.top();
                _log<<"Process P"<<running.pid<<" is preempted by process P"<<temp.pid<<" att time "<<cur_time;
                _log<<" Remaining processing time: "<<(running.t-running.run_time)<<std::endl;
                std::cout<<"Process P"<<running.pid<<" is preempted by process P"<<temp.pid<<" att time "<<cur_time;
                std::cout<<" Remaining processing time: "<<(running.t-running.run_time)<<std::endl;
                
                rq.pop();
                rq.push(running);
                running=temp;
                // The process starts running now.
                running.run_time++;
                _log<<"Process P"<<running.pid<<" starts execution at time "<<cur_time<<std::endl;
                std::cout<<"Process P"<<running.pid<<" starts execution at time "<<cur_time<<std::endl;
            }
        } //rq was not empty.
        else{ // rq was empty at this time, but a process was running
            running.run_time++;
        } // rq was empty, and process was running
    
        if(running.run_time==running.t){ // Check if the process finished its execution.
            _log<<"Process P"<<running.pid<<" finishes execution at time "<<cur_time<<std::endl;
            std::cout<<"Process P"<<running.pid<<" finishes execution at time "<<cur_time<<std::endl;
            // End the process
            running.end_time=cur_time;
            fin.push(running);
            running.pid=-1; 
        }
        cur_time++;
    } // Simulator Loop

    /******************************* Stats-Analysis ***********************/

    std::ofstream stats {"EDF-Stats.txt"};
    
    int dead {0};
    long long wait_time {0L};
    long long turn_time {0L};
    while(!fin.empty()){ // stats loop
        Process& p = fin.front();
        if(p.end_time>=p.dline){
            std::cout<<">> Process P"<<p.pid<<" missed deadline; came at: "<<p.st<<"; exit at: "<<p.end_time<<std::endl;
            _log<<">> Process P"<<p.pid<<" missed deadline; came at: "<<p.st<<"; exit at: "<<p.end_time;
            _log<<"; deadline was: "<<p.dline<<std::endl;
            dead++;
        }
        wait_time+=(long long)(p.end_time-p.st-p.t);
        turn_time+=(long long)(p.end_time-p.st);
        fin.pop();
    } // stats loop
    stats<<"Total Number of Processes that came in: "<<size<<std::endl;
    stats<<"Total Number of Processes executed successfully: "<<size-dead<<std::endl;
    stats<<"Total Number of Processes missed deadline: "<<dead<<std::endl;
    stats<<"Average waiting time: "<<(wait_time)/(double)size<<std::endl;
    stats<<"Average Turnaround time: "<<(turn_time)/(double)size<<std::endl;
    _log.close();
    stats.close(); // close this output stream.
    return 0;
}