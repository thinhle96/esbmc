#include <bits/stdc++.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <unistd.h>
using namespace std;

// ----------------- Tiện ích ptrace -----------------
pid_t getThreadId() {
    return (pid_t) syscall(SYS_gettid);
}
bool pauseThread(pid_t tid) {
    if(ptrace(PTRACE_ATTACH, tid, NULL, NULL) == -1) {
        cerr << "[pauseThread] PTRACE_ATTACH failed, tid=" << tid
             << " errno=" << errno << endl;
        return false;
    }
    int status = 0;
    if(waitpid(tid, &status, 0) == -1) {
        cerr << "[pauseThread] waitpid failed, tid=" << tid
             << " errno=" << errno << endl;
        return false;
    }
    return true;
}
bool resumeThread(pid_t tid) {
    if(ptrace(PTRACE_DETACH, tid, NULL, NULL) == -1) {
        cerr << "[resumeThread] PTRACE_DETACH failed, tid=" << tid
             << " errno=" << errno << endl;
        return false;
    }
    return true;
}

// ----------------- Khai báo trước Task --------------
struct Task {
    string name;
    int priority;
    thread th;
    pid_t tid;
    atomic<bool> finished;
    Task(const string &nm, int pri)
        : name(nm), priority(pri), tid(-1), finished(false) {}
    void run(function<void()> fn);
};

//  So sánh priority (cao > thấp)
struct TaskCompare {
    bool operator()(Task* a, Task* b) {
        return a->priority < b->priority;
    }
};

// ----------------- Khai báo đầy đủ Scheduler --------------
struct Scheduler {
    priority_queue<Task*, vector<Task*>, TaskCompare> taskQueue;
    unordered_map<string, Task*> tasks;
    Task* currentTask = nullptr;
    mutex mtx;
    void addTask(const string &name, function<void()> fn, int pri);
    void activateTask(const string &name);
    void completeCurrentTask();
    bool hasActiveTask();
};

// ----------------- Triển khai hàm run của Task --------------
void Task::run(function<void()> fn) {
    // Lấy TID
    tid = getThreadId();
    // Chạy logic
    fn();
    finished = true;
}

// ----------------- Triển khai Scheduler ---------------------
void Scheduler::addTask(const string &name, function<void()> fn, int pri) {
    Task* t = new Task(name, pri);
    // Tạo thread => t->run(fn)
    t->th = thread([t, fn]{
        t->run(fn);
    });
    // detach => cho nó chạy song song
    t->th.detach();
    tasks[name] = t;
    taskQueue.push(t);
}

void Scheduler::activateTask(const string &name) {
    lock_guard<mutex> lk(mtx);
    if(!tasks.count(name)) return;
    Task* newTask = tasks[name];
    // Đợi nó có tid
    while(newTask->tid < 0) {
        this_thread::sleep_for(1ms);
    }
    // pause 1 lần
    pauseThread(newTask->tid);

    if(!currentTask) {
        currentTask = newTask;
        cout << "[Scheduler] Activate " << currentTask->name << endl;
        resumeThread(currentTask->tid);
    } else {
        if(newTask->priority > currentTask->priority) {
            cout << "Preempting " << currentTask->name 
                 << " with " << newTask->name << endl;
            pauseThread(currentTask->tid);
            taskQueue.push(currentTask);
            currentTask = newTask;
            resumeThread(currentTask->tid);
        } else {
            taskQueue.push(newTask);
        }
    }
}

void Scheduler::completeCurrentTask() {
    lock_guard<mutex> lk(mtx);
    if(currentTask && currentTask->finished) {
        currentTask = nullptr;
    }
    while(!taskQueue.empty()) {
        Task* top = taskQueue.top();
        if(top->finished) {
            taskQueue.pop();
            continue;
        }
        pauseThread(top->tid);
        currentTask = top;
        taskQueue.pop();
        cout << "[Scheduler] Switch to " << currentTask->name << endl;
        resumeThread(currentTask->tid);
        break;
    }
}

bool Scheduler::hasActiveTask() {
    lock_guard<mutex> lk(mtx);
    if(currentTask) return true;
    auto cp = taskQueue;
    while(!cp.empty()) {
        Task* t = cp.top(); cp.pop();
        if(!t->finished) return true;
    }
    return false;
}

// ----------------- Định nghĩa Scheduler toàn cục -----------
Scheduler scheduler;

// ----------------- Các hàm task_1, task_2, task_3 -----------
void task_1() {
    cout << "t1.x++" << endl;
    scheduler.activateTask("Task2");
    cout << "t1.x++" << endl;
}
void task_2() {
    cout << "t2.x--" << endl;
    scheduler.activateTask("Task3");
    cout << "t2.x--" << endl;
}
void task_3() {
    cout << "t3.x++" << endl;
    cout << "t3.x--" << endl;
}

// ----------------- main() ----------------------------------
int main(){
    // Thêm 3 task
    scheduler.addTask("Task1", task_1, 1);
    scheduler.addTask("Task2", task_2, 3);
    scheduler.addTask("Task3", task_3, 2);

    // Kích hoạt Task1
    scheduler.activateTask("Task1");

    // Vòng lặp
    while(scheduler.hasActiveTask()) {
        if(scheduler.currentTask && scheduler.currentTask->finished) {
            pauseThread(scheduler.currentTask->tid);
            scheduler.completeCurrentTask();
        }
        this_thread::sleep_for(50ms);
    }

    cout << "All tasks completed." << endl;
    return 0;
}
