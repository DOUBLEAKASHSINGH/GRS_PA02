# MT24110_Makefile
# Socket Communication Performance Study
# Student: Akash Singh (MT24110)
# Location: Bulandshahr, UP, INDIA
# Education: MTech at IIITD, CSE
#
# This Makefile compiles all socket implementations for the
# GRS assignment on studying data movement cost in network I/O

CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread
LDFLAGS = -pthread

# Source files
COMMON_SRC = MT24110_Common.c
A1_SERVER_SRC = MT24110_Part_A1_Server.c
A1_CLIENT_SRC = MT24110_Part_A1_Client.c
A2_SERVER_SRC = MT24110_Part_A2_Server.c
A2_CLIENT_SRC = MT24110_Part_A2_Client.c
A3_SERVER_SRC = MT24110_Part_A3_Server.c
A3_CLIENT_SRC = MT24110_Part_A3_Client.c

# Object files
COMMON_OBJ = MT24110_Common.o

# Binaries
A1_SERVER = MT24110_A1_Server
A1_CLIENT = MT24110_A1_Client
A2_SERVER = MT24110_A2_Server
A2_CLIENT = MT24110_A2_Client
A3_SERVER = MT24110_A3_Server
A3_CLIENT = MT24110_A3_Client

# Default target - compile all
all: $(A1_SERVER) $(A1_CLIENT) $(A2_SERVER) $(A2_CLIENT) $(A3_SERVER) $(A3_CLIENT)

# Compile common library first
$(COMMON_OBJ): $(COMMON_SRC)
	$(CC) $(CFLAGS) -c $(COMMON_SRC) -o $(COMMON_OBJ)

# Part A1 - Two-Copy Implementation
$(A1_SERVER): $(A1_SERVER_SRC) $(COMMON_OBJ)
	$(CC) $(CFLAGS) $(A1_SERVER_SRC) $(COMMON_OBJ) -o $(A1_SERVER)

$(A1_CLIENT): $(A1_CLIENT_SRC) $(COMMON_OBJ)
	$(CC) $(CFLAGS) $(A1_CLIENT_SRC) $(COMMON_OBJ) -o $(A1_CLIENT)

# Part A2 - One-Copy Implementation
$(A2_SERVER): $(A2_SERVER_SRC) $(COMMON_OBJ)
	$(CC) $(CFLAGS) $(A2_SERVER_SRC) $(COMMON_OBJ) -o $(A2_SERVER)

$(A2_CLIENT): $(A2_CLIENT_SRC) $(COMMON_OBJ)
	$(CC) $(CFLAGS) $(A2_CLIENT_SRC) $(COMMON_OBJ) -o $(A2_CLIENT)

# Part A3 - Zero-Copy Implementation
$(A3_SERVER): $(A3_SERVER_SRC) $(COMMON_OBJ)
	$(CC) $(CFLAGS) $(A3_SERVER_SRC) $(COMMON_OBJ) -o $(A3_SERVER)

$(A3_CLIENT): $(A3_CLIENT_SRC) $(COMMON_OBJ)
	$(CC) $(CFLAGS) $(A3_CLIENT_SRC) $(COMMON_OBJ) -o $(A3_CLIENT)

# Clean build artifacts
clean:
	rm -f $(COMMON_OBJ) $(A1_SERVER) $(A1_CLIENT) $(A2_SERVER) $(A2_CLIENT) $(A3_SERVER) $(A3_CLIENT)
	rm -f *.o
	rm -f MT24110_throughput_vs_message_size.pdf MT24110_throughput_vs_message_size.png
	rm -f MT24110_latency_vs_thread_count.pdf MT24110_latency_vs_thread_count.png
	rm -f MT24110_cache_misses_vs_message_size.pdf MT24110_cache_misses_vs_message_size.png
	rm -f MT24110_cycles_per_byte.pdf MT24110_cycles_per_byte.png

# Generate plots
plots: MT24110_plot_throughput.py MT24110_plot_latency.py MT24110_plot_cache.py MT24110_plot_cycles.py
	python3 MT24110_plot_throughput.py
	python3 MT24110_plot_latency.py
	python3 MT24110_plot_cache.py
	python3 MT24110_plot_cycles.py

# Run all experiments
run: all
	chmod +x MT24110_run_experiments.sh
	./MT24110_run_experiments.sh

# Help target
help:
	@echo "GRS Socket Communication Performance Study - Makefile"
	@echo "====================================================="
	@echo "Targets:"
	@echo "  all      - Compile all implementations (default)"
	@echo "  clean    - Remove all compiled binaries"
	@echo "  plots    - Generate all plots using matplotlib"
	@echo "  run      - Run all experiments"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Binaries:"
	@echo "  MT24110_A1_Server/Client - Two-copy baseline"
	@echo "  MT24110_A2_Server/Client - One-copy optimized"
	@echo "  MT24110_A3_Server/Client - Zero-copy implementation"

.PHONY: all clean plots run help

