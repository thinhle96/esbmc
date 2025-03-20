import threading
import queue
import time

class Task:
    def __init__(self, name, function, priority):
        self.name = name
        self.function = function
        self.priority = priority
        # Thread sẽ chỉ được khởi tạo 1 lần, nhưng ban đầu CHƯA start
        self.thread = threading.Thread(target=self.run)
        self.finished = False
        self.paused = False

    def run(self):
        self.function()
        self.finished = True

    def start(self):
        if not self.thread.is_alive():
            self.thread.start()

    def pause(self):
        self.paused = True

    def __lt__(self, other):
        # Priority cao hơn => xếp trước trong PriorityQueue => trả về True nếu mình cao hơn
        return self.priority > other.priority

class Scheduler:
    def __init__(self):
        self.lock = threading.Lock()
        self.task_queue = queue.PriorityQueue()
        self.current_task = None
        self.tasks = {}

    def add_task(self, name, function, priority):
        task = Task(name, function, priority)
        self.tasks[name] = task
        self.task_queue.put(task)

    def activate_task(self, name):
        with self.lock:
            if name not in self.tasks:
                return
            new_task = self.tasks[name]

            # Nếu chưa có Task đang chạy, ta chọn luôn Task này
            if not self.current_task:
                self.current_task = new_task
                self.current_task.start()
            else:
                # So sánh priority
                if new_task.priority > self.current_task.priority:
                    print(f"Preempting {self.current_task.name} with {new_task.name}")
                    # Tạm ngừng Task hiện tại, đưa nó vào lại queue
                    self.current_task.pause()
                    self.task_queue.put(self.current_task)

                    # Chuyển sang Task mới
                    self.current_task = new_task
                    self.current_task.start()
                else:
                    # Nếu priority không cao hơn, xếp lại vào queue
                    self.task_queue.put(new_task)

    def complete_current_task(self):
        with self.lock:
            # Nếu Task hiện tại đã finish thì ta lấy Task tiếp theo
            if self.current_task and not self.current_task.finished:
                return  # Chưa xong, chưa chuyển

            if not self.task_queue.empty():
                self.current_task = self.task_queue.get()
                if not self.current_task.finished:
                    self.current_task.start()
            else:
                self.current_task = None

# Ví dụ minh họa

def task_1():
    print("t1.x++")
    scheduler.activate_task("Task2")
    print("t1.x++")

def task_2():
    print("t2.x--")
    scheduler.activate_task("Task3")
    print("t2.x--")

def task_3():
    print("t3.x++")
    print("t3.x--")

scheduler = Scheduler()


scheduler.add_task("Task1", task_1, priority=1)
scheduler.add_task("Task2", task_2, priority=3)
scheduler.add_task("Task3", task_3, priority=2)

scheduler.activate_task("Task1")

while scheduler.current_task is not None:
    scheduler.current_task.thread.join()
    scheduler.complete_current_task()
    time.sleep(0.2)
