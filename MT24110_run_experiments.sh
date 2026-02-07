#!/bin/bash
#
# MT24110_run_experiments.sh
# Automated experiment runner for socket communication profiling
# Myself: Akash Singh (MT24110)
# Location: Bulandshahr, UP, INDIA
#
# This script:
# 1. Compiles all implementations
# 2. Runs experiments across different message sizes and thread counts
# 3. Collects profiling data using perf stat
# 4. Saves results to CSV files
#

# set -e

# Configuration
PORT=8081
DURATION=5
SERVER_IP="192.168.42.68"

# Message sizes to test (in bytes)
MESSAGE_SIZES=(512 1024 4096 8192)

# Thread counts to test
THREAD_COUNTS=(4 8)

# Results directory
RESULTS_DIR="MT24110_Part_D_Data"
mkdir -p $RESULTS_DIR

echo "============================================="
echo "GRS Socket Communication Experiments"
echo "Myself: Akash Singh (MT24110)"
echo "============================================="
echo ""

# Compile first
echo "Step 1: Compiling all implementations..."
chmod +x MT24110_compile_all.sh
./MT24110_compile_all.sh
echo ""

# Function to run experiment for a single configuration
run_experiment() {
    local impl=$1       # 1, 2, or 3 (two-copy, one-copy, zero-copy)
    local msg_size=$2   # Message size in bytes
    local threads=$3     # Number of threads
    local output_file=$4 # Output CSV file

    echo "  Running: impl=$impl msg_size=$msg_size threads=$threads"

    # Start server in background
    ./MT24110_A${impl}_Server $PORT $msg_size > /dev/null 2>&1 &
    SERVER_PID=$!

    # Wait for server to start
    sleep 2

    # Run client and capture output
    CLIENT_OUTPUT=$(./MT24110_A${impl}_Client $SERVER_IP $PORT $msg_size $threads $DURATION 2>&1)

    # Stop server
    kill $SERVER_PID 2>/dev/null || true
    wait $SERVER_PID 2>/dev/null || true

    # Extract metrics from client output
    THROUGHPUT=$(echo "$CLIENT_OUTPUT" | grep "Throughput:" | awk '{print $2}')
    LATENCY=$(echo "$CLIENT_OUTPUT" | grep "Average latency:" | awk '{print $3}')

    # If throughput/latency not found, set to 0
    THROUGHPUT=${THROUGHPUT:-0}
    LATENCY=${LATENCY:-0}

    # Write to CSV
    echo "$impl,$msg_size,$threads,$THROUGHPUT,$LATENCY" >> $output_file
}

# Function to run experiment with perf profiling
run_profiled_experiment() {
    local impl=$1
    local msg_size=$2
    local threads=$3
    local output_prefix=$4

    echo "  Running profiled: impl=$impl msg_size=$msg_size threads=$threads"

    # Start server in background
    # ./MT24110_A${impl}_Server $PORT $msg_size > /dev/null 2>&1 &
    # SERVER_PID=$!

    # Wait for server to start
    sleep 2

    # Run client with perf stat
    PERF_OUTPUT="perf stat -e cycles,instructions,L1-dcache-load-misses,cache-misses,context-switches -o ${output_prefix}_perf.txt "

    $PERF_OUTPUT ./MT24110_A${impl}_Client $SERVER_IP $PORT $msg_size $threads $DURATION > /dev/null 2>&1 &
    CLIENT_PID=$!

    wait $CLIENT_PID

    # Stop server
    kill $SERVER_PID 2>/dev/null || true
    wait $SERVER_PID 2>/dev/null || true
}

echo "Step 2: Running experiments..."
echo ""

# Initialize CSV files with headers
echo "implementation,message_size,threads,throughput_gbps,latency_us" > ${RESULTS_DIR}/MT24110_results_2copy.csv
echo "implementation,message_size,threads,throughput_gbps,latency_us" > ${RESULTS_DIR}/MT24110_results_1copy.csv
echo "implementation,message_size,threads,throughput_gbps,latency_us" > ${RESULTS_DIR}/MT24110_results_0copy.csv

# Run experiments for each implementation
for msg_size in "${MESSAGE_SIZES[@]}"; do
    for threads in "${THREAD_COUNTS[@]}"; do
        # Two-copy (implementation 1)
        run_experiment 1 $msg_size $threads "${RESULTS_DIR}/temp.csv"
        # Read the last line and append to proper file
        tail -1 "${RESULTS_DIR}/temp.csv" >> "${RESULTS_DIR}/MT24110_results_2copy.csv"

        # One-copy (implementation 2)
        run_experiment 2 $msg_size $threads "${RESULTS_DIR}/temp.csv"
        tail -1 "${RESULTS_DIR}/temp.csv" >> "${RESULTS_DIR}/MT24110_results_1copy.csv"

        # Zero-copy (implementation 3)
        run_experiment 3 $msg_size $threads "${RESULTS_DIR}/temp.csv"
        tail -1 "${RESULTS_DIR}/temp.csv" >> "${RESULTS_DIR}/MT24110_results_0copy.csv"

        # Small delay between experiments
        sleep 1
    done
done

# Cleanup temp file
rm -f "${RESULTS_DIR}/temp.csv"

echo ""
echo "Step 3: Collecting perf stat data..."

# Run profiled experiments (selected configurations)
PROFILED_SIZES=(512 1024 4096 8192)
PROFILED_THREADS=(4 8)

for msg_size in "${PROFILED_SIZES[@]}"; do
    for threads in "${PROFILED_THREADS[@]}"; do
        # Profile two-copy
        run_profiled_experiment 1 $msg_size $threads "${RESULTS_DIR}/MT24110_perf_2copy_${msg_size}_${threads}"

        sleep 1

        # Profile one-copy
        run_profiled_experiment 2 $msg_size $threads "${RESULTS_DIR}/MT24110_perf_1copy_${msg_size}_${threads}"

        sleep 1

        # Profile zero-copy
        run_profiled_experiment 3 $msg_size $threads "${RESULTS_DIR}/MT24110_perf_0copy_${msg_size}_${threads}"

        sleep 1
    done
done

echo ""
echo "============================================="
echo "Experiments Complete!"
echo "============================================="
echo ""
echo "Results saved to:"
echo "  ${RESULTS_DIR}/MT24110_results_2copy.csv"
echo "  ${RESULTS_DIR}/MT24110_results_1copy.csv"
echo "  ${RESULTS_DIR}/MT24110_results_0copy.csv"
echo ""
echo "Perf data saved to:"
echo "  ${RESULTS_DIR}/MT24110_perf_*.txt"
echo ""




# Directory containing the txt files
DATA_DIR="MT24110_Part_D_Data"
OUTPUT_FILE="${DATA_DIR}/MT24110_profiling_parsed.csv"

# Check if directory exists, otherwise look in current directory
if [ ! -d "$DATA_DIR" ]; then
    echo "Directory $DATA_DIR not found, assuming files are in current directory."
    DATA_DIR="."
    OUTPUT_FILE="MT24110_profiling_parsed.csv"
fi

echo "============================================="
echo "Parsing Perf Data files in: $DATA_DIR"
echo "Output CSV: $OUTPUT_FILE"
echo "============================================="

# Initialize CSV Header
# Note: 'cycles_per_byte' must be calculated during plotting by merging 
# this data with your throughput CSVs (Total Bytes = Throughput * Duration).
echo "implementation,message_size,threads,cycles,instructions,l1_dcache_misses,llc_misses,context_switches" > "$OUTPUT_FILE"

# Iterate over all matching perf text files
for file in "$DATA_DIR"/MT24110_perf_*_perf.txt; do
    
    # Check if file exists (in case of no matches)
    [ -e "$file" ] || continue

    # ---------------------------------------------------------
    # 1. Extract Metadata from Filename
    # ---------------------------------------------------------
    # Format: MT24110_perf_<IMPL>_<SIZE>_<THREADS>_perf.txt
    # Example: MT24110_perf_0copy_512_6_perf.txt
    
    basename=$(basename "$file")
    
    # Strip prefix and suffix
    temp="${basename#MT24110_perf_}"  # Remove prefix
    temp="${temp%_perf.txt}"          # Remove suffix
    
    # Extract fields using cut (delimiter _)
    impl=$(echo "$temp" | cut -d'_' -f1)      # e.g., 0copy
    msg_size=$(echo "$temp" | cut -d'_' -f2)  # e.g., 512
    threads=$(echo "$temp" | cut -d'_' -f3)   # e.g., 6

    # ---------------------------------------------------------
    # 2. Parse Perf Counters from File Content
    # ---------------------------------------------------------
    
    # Function to extract a number by keyword, remove commas, handle <not supported>
    get_value() {
        local keyword=$1
        local file=$2
        
        # Grep the line, take the first column, remove commas
        val=$(grep "$keyword" "$file" | awk '{print $1}' | tr -d ',')
        
        # If empty or <not supported>, return 0
        if [[ "$val" == *"<"* ]] || [[ -z "$val" ]]; then
            echo "0"
        else
            echo "$val"
        fi
    }

    CYCLES=$(get_value "cycles" "$file")
    INSTRUCTIONS=$(get_value "instructions" "$file")
    L1_MISSES=$(get_value "L1-dcache-load-misses" "$file")
    LLC_MISSES=$(get_value "cache-misses" "$file")
    CTX_SWITCHES=$(get_value "context-switches" "$file")

    # ---------------------------------------------------------
    # 3. Append to CSV
    # ---------------------------------------------------------
    echo "$impl,$msg_size,$threads,$CYCLES,$INSTRUCTIONS,$L1_MISSES,$LLC_MISSES,$CTX_SWITCHES" >> "$OUTPUT_FILE"
    
    echo "Parsed: Impl=$impl Size=$msg_size Threads=$threads"
done

echo ""
echo "Done! Data saved to $OUTPUT_FILE"