#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

static void t1();
static void t2();
static void t3();

struct Task {
    void (*func)();
    int priority;
    void run();
};

map<void(*)(), void(*)()> chain_task;
set<void(*)()> completed_tasks;

class MyStack {
    Task* arr;
    int cap;
    int topIndex;
public:
    MyStack(int c=1000) : cap(c), topIndex(-1) { arr = new Task[cap]; }
    ~MyStack() { delete[] arr; }
    bool empty() const { return topIndex < 0; }
    Task& top() { return arr[topIndex]; }
    void push(const Task& t) {
        if(topIndex+1<cap) {
            topIndex++;
            arr[topIndex] = t;
        }
    }
    void pop() { if(!empty()) topIndex--; }
};

class MyMaxHeap {
    vector<pair<int, Task>> data;
    void heapifyUp(int idx) {
        while(idx>0) {
            int p=(idx-1)/2;
            if(data[idx].first>data[p].first) {
                auto temp=data[idx]; data[idx]=data[p]; data[p]=temp;
                idx=p;
            } else break;
        }
    }
    void heapifyDown(int idx) {
        while(true) {
            int l=2*idx+1, r=2*idx+2, largest=idx;
            if(l<(int)data.size() && data[l].first>data[largest].first) largest=l;
            if(r<(int)data.size() && data[r].first>data[largest].first) largest=r;
            if(largest!=idx) {
                auto temp=data[idx]; data[idx]=data[largest]; data[largest]=temp;
                idx=largest;
            } else break;
        }
    }
public:
    bool empty() const { return data.empty(); }
    pair<int,Task> top() { return data[0]; }
    void push(const pair<int,Task>& val) {
        data.push_back(val);
        heapifyUp((int)data.size()-1);
    }
    void pop() {
        if(!data.empty()) {
            data[0] = data[data.size()-1];
            data.pop_back();
            if(!data.empty()) heapifyDown(0);
        }
    }
};

MyStack s;
MyMaxHeap q;

void activate_task(const Task& t);
void terminate_task();

void Task::run() {
    if(chain_task.find(func)!=chain_task.end()) {
        auto dep=chain_task[func];
        if(completed_tasks.find(dep)==completed_tasks.end()) {
            activate_task({dep,priority+1});
            activate_task({func,priority});
            return;
        }
    }
    func();
    completed_tasks.insert(func);
    terminate_task();
}

void t1() {
    cout<<"t1 x++"<<endl;
    activate_task({t2,3});
    cout<<"t1 x++ (finish t1)"<<endl;
}
void t2() {
    cout<<"t2 x--"<<endl;
    activate_task({t3,2});
    cout<<"t2 x-- (finish t2)"<<endl;
}
void t3() {
    cout<<"t3 x++"<<endl;
    cout<<"t3 x-- (finish t3)"<<endl;
}

void activate_task(const Task& t) {
    if(s.empty() || t.priority>s.top().priority) {
        s.push(t);
        s.top().run();
    } else {
        q.push({t.priority,t});
    }
}

void terminate_task() {
    if(!s.empty()) {
        s.pop();
    }
    if(!q.empty() && !s.empty()) {
        auto top_pair=q.top();
        if(top_pair.first>s.top().priority) {
            q.pop();
            s.push(top_pair.second);
            s.top().run();
        }
    }
}

int main(){
    // chain_task[t1]=t2;
    // chain_task[t2]=t3;
    activate_task({t1,1});
    while(!q.empty()){
        auto next_pair=q.top(); 
        q.pop();
        next_pair.second.run();
    }
    return 0;
}
