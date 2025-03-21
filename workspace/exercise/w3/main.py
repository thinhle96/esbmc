import heapq

chain_task = {}
completed_tasks = set()

def t1():
    print("t1 x++")
    activate_task(Task(t2, 3))
    print("t1 x++ (finish t1)")

def t2():
    print("t2 x--")
    activate_task(Task(t3, 2))
    print("t2 x-- (finish t2)")

def t3():
    print("t3 x++")
    print("t3 x-- (finish t3)")

chain_task = {
    t1: t2,
    t2: t3
}

class Task:
    def __init__(self, func, priority):
        self.func = func
        self.priority = priority

    def run(self):
        if self.func in chain_task:
            dep = chain_task[self.func]
            if dep not in completed_tasks:
                activate_task(Task(dep, self.priority + 1))
                activate_task(Task(self.func, self.priority))
                return
        self.func()
        completed_tasks.add(self.func)
        terminate_task()

q = []
s = []

def activate_task(t):
    if not s or t.priority > s[-1].priority:
        s.append(t)
        t.run()
    else:
        heapq.heappush(q, (-t.priority, t))

def terminate_task():
    if s:
        s.pop()
    if q and s:
        top_priority, top_task = q[0]
        if -top_priority > s[-1].priority:
            heapq.heappop(q)
            s.append(top_task)
            top_task.run()

def main():
    activate_task(Task(t1, 1))
    while q:
        _, next_task = heapq.heappop(q)
        next_task.run()

if __name__ == "__main__":
    main()
