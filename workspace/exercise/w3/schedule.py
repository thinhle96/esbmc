import sys
import threading
import queue
import time

# Biến toàn cục, đánh dấu Thread nào được phép chạy
allowed = {}  # {thread_ident: bool}

def tracer(frame, event, arg):
    """
    Được gọi trước khi thực thi mỗi dòng lệnh Python trong thread.
    Nếu 'allowed[thread_id]' là False, thì dừng ở đây (sleep) đến khi True.
    """
    t_id = threading.get_ident()
    while not allowed.get(t_id, False):
        time.sleep(0.0001)
    return tracer

class Task:
    def __init__(self, name, function, priority):
        self.name = name
        self.function = function
        self.priority = priority
        self.finished = False
        self._thread = threading.Thread(target=self._run, name=name)

    def _run(self):
        """
        Hàm chính của Task. Cài đặt tracer để chặn trước mỗi dòng lệnh.
        Sau đó chạy function() (là task_1, task_2, task_3...).
        """
        sys.settrace(tracer)
        self.function()
        self.finished = True

    def start(self):
        """Khởi động thread (mặc định ban đầu sẽ bị 'pause')."""
        if not self._thread.is_alive():
            self._thread.start()
            # Ban đầu ta đặt allowed=False, scheduler sẽ gọi resume() khi cần
            allowed[self._thread.ident] = False

    def pause(self):
        """Không cho thread thực thi thêm (preempt)."""
        t_id = self._thread.ident
        if t_id is not None:
            allowed[t_id] = False

    def resume(self):
        """Cho thread chạy tiếp."""
        t_id = self._thread.ident
        if t_id is not None:
            allowed[t_id] = True

    def is_alive(self):
        return self._thread.is_alive()

    def join(self):
        self._thread.join()

    # PriorityQueue lấy phần tử “nhỏ nhất” trước, ta so sánh ngược
    def __lt__(self, other):
        return self.priority > other.priority

class Scheduler:
    def __init__(self):
        self.lock = threading.Lock()
        self.task_queue = queue.PriorityQueue()
        self.current_task = None
        self.tasks = {}

    def add_task(self, name, function, priority):
        """Tạo Task, lưu vào danh sách, và đưa vào PriorityQueue."""
        task = Task(name, function, priority)
        self.tasks[name] = task
        self.task_queue.put(task)

    def activate_task(self, name):
        """Kích hoạt (hoặc preempt) một Task theo priority."""
        with self.lock:
            if name not in self.tasks:
                return
            new_task = self.tasks[name]

            # Chưa có Task nào => chạy new_task
            if not self.current_task:
                self.current_task = new_task
                self.current_task.start()
                # Ban đầu paused, bây giờ cho chạy
                self.current_task.resume()
            else:
                # Có Task đang chạy => so sánh priority
                if new_task.priority > self.current_task.priority:
                    print(f"Preempting {self.current_task.name} with {new_task.name}")
                    # Ngưng Task hiện tại => cho vào Queue
                    self.current_task.pause()
                    self.task_queue.put(self.current_task)

                    # Khởi động Task mới (nếu chưa)
                    if not new_task.is_alive():
                        new_task.start()
                    # Cho phép nó chạy
                    new_task.resume()
                    self.current_task = new_task
                else:
                    # Priority không cao hơn => bỏ vào queue
                    self.task_queue.put(new_task)

    def complete_current_task(self):
        """Khi Task hiện tại xong => lấy Task priority cao nhất trong queue để chạy."""
        with self.lock:
            if self.current_task and not self.current_task.finished:
                return  # Chưa xong, không chuyển

            if not self.task_queue.empty():
                next_task = self.task_queue.get()
                # Nếu Task chưa từng chạy, start & pause
                if not next_task.is_alive():
                    next_task.start()
                    next_task.pause()

                # Resume Task tiếp theo
                next_task.resume()
                self.current_task = next_task
            else:
                self.current_task = None


# --------------------
# 3 hàm Task KHÔNG CHỈNH SỬA GÌ
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

# --------------------
# Thử chạy
scheduler = Scheduler()
scheduler.add_task("Task1", task_1, priority=1)
scheduler.add_task("Task2", task_2, priority=3)  # Cao nhất
scheduler.add_task("Task3", task_3, priority=2)

# Kích hoạt Task1 đầu
scheduler.activate_task("Task1")

# Vòng lặp chính
while True:
    with scheduler.lock:
        if scheduler.current_task is None:
            break
    # Chờ Task hiện tại chạy xong
    scheduler.current_task.join()
    # Chuyển sang Task kế
    scheduler.complete_current_task()
    time.sleep(0.01)

print("Done.")
