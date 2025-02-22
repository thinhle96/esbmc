# Use Ubuntu 24.04 as the base image
FROM ubuntu:24.04

# Set non-interactive mode for apt-get to prevent prompts
ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies for ESBMC and other build tools
RUN apt-get update && apt-get install -y \
    nodejs \
    npm \
    clang-14 \
    llvm-14 \
    clang-tidy-14 \
    python-is-python3 \
    python3 \
    python3-pip \
    git \
    ccache \
    unzip \
    wget \
    curl \
    bison \
    flex \
    g++-multilib \
    linux-libc-dev \
    libboost-all-dev \
    libz3-dev \
    libclang-14-dev \
    libclang-cpp-dev \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Keep container running with tail -f /dev/null
CMD ["bash", "-c", "tail -f /dev/null"]
