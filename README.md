# 🚀 Docker Image - ESBMC 7.8.1 trên Ubuntu 24.04

📝 Đây là một **Docker image** đã được cài đặt **ESBMC phiên bản 7.8.1** trên hệ điều hành **Ubuntu 24.04**.
Sử dụng Docker giúp bạn **tránh các vấn đề về hệ điều hành và cài đặt thư viện**, đảm bảo môi trường phát triển nhất quán.

---

## 👍 Hướng dẫn cài đặt
Để có thể sử dụng sẵn code trong thư mục /workspace/esbmc, cần bỏ option volumes trong docker compose, nhưng code sẽ không được lưu sau khi container down.
### 🔹 Yêu cầu
- **Phần mềm**: Docker, Docker Desktop
- **IDE**: Visual Studio Code
- **Extensions cần thiết**:
  - Remote - SSH
  - Dev Containers
  - Docker

### 🔹 Các bước truy cập dev container
1. **Clone repository**:
   ```bash
   git clone https://github.com/thinhle96/esbmc.git
   ```
2. **Chạy Docker Compose**:
   ```bash
   docker compose up -d
   ```
3. **Mở VS Code và kết nối với container**:
   - Chọn mục **Docker** từ thanh menu bên trái.
   - Chuột phải vào container -> **Attach Visual Studio Code**.
   - Khi cửa sổ VS Code mở ra, vào **File > Open Folder**, chọn thư mục **`/workspace`**.

   ![Docker VS Code](image.png)

---

## ▶️ Hướng dẫn chạy ESBMC
1. **Di chuyển vào thư mục làm việc**:
   ```bash
   cd /workspace/esbmc-demo
   ```
2. **Chạy ESBMC với `k-induction`**:
   ```bash
   esbmc sample.c --k-induction
   ```

## ▶️ Hướng dẫn debug ESBMC
1. **Tạo folder debug**:
   ```bash
   cd /workspace
   git clone https://github.com/esbmc/esbmc.git
   mkdir debug && cd debug
   cmake .. -DENABLE_Z3=1 -DCMAKE_BUILD_TYPE=Debug
   make -j$(nproc)
   ```
2. **Tạo debug launch.json**:
   Open folder: File -> Open folder: /workspace/esbmc
   Create file launch.json: Ctrl + Shift + D -> Create launch.json -> Overwrite nội dung file launch.json
3. **Chạy debug**
   Chọn file : ví dụ /workspace/esbmc/src/esbmc/main.cpp
   Thêm breakpoint và ấn fn + F5
## ⚠️ Lưu ý
- **Thư mục `/workspace` đã chứa thư mục code**: `esbmc-demo`.
- **Nên viết code trong thư mục `/workspace`** để tránh mất dữ liệu khi container bị xóa, vì thư mục này đã được mount với Docker Volumes.

---

# 🚀 Docker Image - ESBMC 7.8.1 on Ubuntu 24.04

📝 This is a **Docker image** with **ESBMC version 7.8.1** pre-installed on **Ubuntu 24.04**.
Using Docker helps you **avoid OS compatibility issues and library installation problems**, ensuring a consistent development environment.

---

## 👍 Installation Guide
You can use the existing code inside /workspace/esbmc. The volumes option in Docker Compose has been removed, meaning code changes will not be saved after the container is stopped.
### 🔹 Requirements
- **Software**: Docker, Docker Desktop
- **IDE**: Visual Studio Code
- **Required Extensions**:
  - Remote - SSH
  - Dev Containers
  - Docker

### 🔹 Steps to Access the Dev Container
1. **Clone the repository**:
   ```bash
   git clone https://github.com/thinhle96/esbmc.git
   ```
2. **Run Docker Compose**:
   ```bash
   docker compose up -d
   ```
3. **Open VS Code and connect to the container**:
   - Select the **Docker** option from the left menu.
   - Right-click on the container -> **Attach Visual Studio Code**.
   - When the VS Code window opens, go to **File > Open Folder** and select the **`/workspace`** folder.

   ![Docker VS Code](image.png)

---

## ▶️ Running ESBMC
1. **Navigate to the working directory**:
   ```bash
   cd /workspace/esbmc-demo
   ```
2. **Run ESBMC with `k-induction`**:
   ```bash
   esbmc sample.c --k-induction
   ```

## ▶️ Debugging ESBMC
1. **Create a debug folder**:
   ```bash
   cd /workspace
   git clone https://github.com/esbmc/esbmc.git
   mkdir debug && cd debug
   cmake .. -DENABLE_Z3=1 -DCMAKE_BUILD_TYPE=Debug
   make -j$(nproc)
   ```
2. **Create a debug `launch.json` file**:
   - Open folder: **File -> Open Folder -> /workspace/esbmc**
   - Create `launch.json`: **Ctrl + Shift + D -> Create launch.json -> Overwrite the content of launch.json**
3. **Run Debug**:
   - Select the file, e.g., `/workspace/esbmc/src/esbmc/main.cpp`
   - Add breakpoints and press `F5`

## ⚠️ Notes
- The **`/workspace`** directory already contains the `esbmc-demo` code folder.
- It is **recommended to write code inside `/workspace`** to prevent data loss if the container is deleted, as this directory is mounted with Docker Volumes.


