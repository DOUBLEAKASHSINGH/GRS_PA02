# MT24110_README.md
# GRS Assignment - Socket Communication Performance Study
# Student: Akash Singh (MT24110)
# Location: Bulandshahr, UP, INDIA
# Education: MTech at IIITD, CSE
#

# GRS Assignment - Socket Communication Performance Study

## Overview

This project implements and compares three socket communication strategies to study the cost of data movement in network I/O:

1. **Two-Copy (Baseline)** - Traditional send()/recv()
2. **One-Copy** - Optimized using sendmsg() with iovec
3. **Zero-Copy** - Using MSG_ZEROCOPY for DMA transfer

## Project Structure

```
MT24110_PA02/
├── MT24110_Makefile              # Compilation script
├── MT24110_README.md             # This file
├── MT24110_Common.h/.c           # Common utilities and data structures
├── MT24110_Part_A1_Server.c      # Two-copy server
├── MT24110_Part_A1_Client.c      # Two-copy client
├── MT24110_Part_A2_Server.c      # One-copy server
├── MT24110_Part_A2_Client.c      # One-copy client
├── MT24110_Part_A3_Server.c      # Zero-copy server
├── MT24110_Part_A3_Client.c      # Zero-copy client
├── MT24110_compile_all.sh        # Compilation script
├── MT24110_run_experiments.sh   # Automated experiment runner
├── MT24110_plot_throughput.py   # Throughput plots
├── MT24110_plot_latency.py      # Latency plots
├── MT24110_plot_cache.py        # Cache miss plots
├── MT24110_plot_cycles.py       # CPU cycles plots
└── MT24110_Part_D_Data/          # Experiment results directory
```

## Building

```bash
# Using Makefile (recommended)
make all

# Or using the compilation script
chmod +x MT24110_compile_all.sh
./MT24110_compile_all.sh
```

## Running Experiments

### Manual Testing

**Start Server:**
```bash
# Two-copy server
./MT24110_A1_Server <port> <message_size>

# Example
./MT24110_A1_Server 8080 1024
```

**Start Client:**
```bash
# Two-copy client
./MT24110_A1_Client <server_ip> <port> <message_size> <num_threads> <duration_sec>

# Example - 4 threads, 5 seconds, 1KB messages
./MT24110_A1_Client 192.168.41.101 8080 1024 4 5
```

### Automated Experiments

```bash
chmod +x MT24110_run_experiments.sh
./MT24110_run_experiments.sh
```

This will:
1. Compile all implementations
2. Run tests across multiple message sizes (64B to 1MB)
3. Test with different thread counts (1 to 16)
4. Collect throughput and latency metrics
5. Save results to CSV files

## Implementation Details

### Message Structure

Each message contains 8 dynamically allocated string fields:
```c
typedef struct {
    char *field1;
    char *field2;
    char *field3;
    char *field4;
    char *field5;
    char *field6;
    char *field7;
    char *field8;
} MT24110_Message;
```

### Two-Copy Implementation (A1)

Uses standard send()/recv() which involves:
- **Copy 1**: User buffer -> Kernel socket buffer (send)
- **Copy 2**: Kernel socket buffer -> User buffer (recv)

### One-Copy Implementation (A2)

Uses sendmsg() with iovec to reduce one copy:
- Eliminates intermediate copy on send path
- Kernel reads directly from user buffer via scatter-gather

### Zero-Copy Implementation (A3)

Uses MSG_ZEROCOPY for true zero-copy transmission:
- DMA transfer directly from user buffer
- No intermediate kernel buffer copies
- Requires SO_ZEROCOPY socket option

## Generating Plots

```bash
# Install dependencies if needed
pip install matplotlib numpy

# Generate all plots
make plots

# Or run individual scripts
python3 MT24110_plot_throughput.py
python3 MT24110_plot_latency.py
python3 MT24110_plot_cache.py
python3 MT24110_plot_cycles.py
```

## Profiling with perf

```bash
# Install perf if not available
sudo apt install linux-tools-common

# Profile a single experiment
perf stat -e cycles,instructions,cache-references,cache-misses,context-switches ./MT24110_A1_Client 192.168.**.** 8081 8192 4 5
```

## Myself

**Akash Singh (MT24110)**
- Location: Bulandshahr, UP, INDIA
- Education: MTech at IIITD, CSE

## References

- Linux socket documentation: man 7 socket, man 2 send, man 2 recv
- MSG_ZEROCOPY: Documentation/networking/msg_zerocopy.rst
- perf: man perf-stat

