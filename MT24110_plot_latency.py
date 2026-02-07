#!/usr/bin/env python3
#
# MT24110_plot_latency.py
# Plot latency vs thread count for all implementations
# Myself: Akash Singh (MT24110)
# Location: Bulandshahr, UP, INDIA
# Education: MTech at IIITD, CSE
#

import matplotlib.pyplot as plt
import numpy as np

# Hardcoded latency data (us) from experiments
# Format: [threads_4, threads_6, threads_8]
TWO_COPY_SIZE512B = [8080.09, 8389.23, 8715.25]
ONE_COPY_SIZE512B = [7959.31, 8839.55, 8070.84]
ZERO_COPY_SIZE512B = [7782.09, 8449.19, 7460.05]

TWO_COPY_SIZE1KB = [8535.28, 7943.39, 9428.9]
ONE_COPY_SIZE1KB = [7927.42, 8310.38, 9074.25]
ZERO_COPY_SIZE1KB = [7905.57, 8701.31, 12384.47]

TWO_COPY_SIZE8KB = [7845.0, 8847.22, 11402.29]
ONE_COPY_SIZE8KB = [7252.96, 8757.16, 11934.57]
ZERO_COPY_SIZE8KB = [7078.9, 9218.16, 20796.12]

THREAD_COUNTS = [4, 6, 8]
THREAD_LABELS = ['4', '6', '8']

# Create figure with 3 subplots
fig, axes = plt.subplots(1, 3, figsize=(15, 5))

# System configuration info
SYSTEM_CONFIG = "System: Linux 6.17, Intel Core i5, 8GB RAM"

# Plot 1: 512B Message Size
ax1 = axes[0]
ax1.plot(THREAD_COUNTS, TWO_COPY_SIZE512B, 'o-', label='Two-Copy', linewidth=2, markersize=8)
ax1.plot(THREAD_COUNTS, ONE_COPY_SIZE512B, 's-', label='One-Copy', linewidth=2, markersize=8)
ax1.plot(THREAD_COUNTS, ZERO_COPY_SIZE512B, '^-', label='Zero-Copy', linewidth=2, markersize=8)
ax1.set_xlabel('Thread Count', fontsize=12)
ax1.set_ylabel('Latency (us)', fontsize=12)
ax1.set_title('Latency vs Thread Count\n(512B Messages)', fontsize=14)
ax1.grid(True, alpha=0.3)
ax1.legend(loc='upper left')
ax1.set_xticks(THREAD_COUNTS)
ax1.set_xticklabels(THREAD_LABELS)

# Plot 2: 1KB Message Size
ax2 = axes[1]
ax2.plot(THREAD_COUNTS, TWO_COPY_SIZE1KB, 'o-', label='Two-Copy', linewidth=2, markersize=8)
ax2.plot(THREAD_COUNTS, ONE_COPY_SIZE1KB, 's-', label='One-Copy', linewidth=2, markersize=8)
ax2.plot(THREAD_COUNTS, ZERO_COPY_SIZE1KB, '^-', label='Zero-Copy', linewidth=2, markersize=8)
ax2.set_xlabel('Thread Count', fontsize=12)
ax2.set_ylabel('Latency (us)', fontsize=12)
ax2.set_title('Latency vs Thread Count\n(1KB Messages)', fontsize=14)
ax2.grid(True, alpha=0.3)
ax2.legend(loc='upper left')
ax2.set_xticks(THREAD_COUNTS)
ax2.set_xticklabels(THREAD_LABELS)

# Plot 3: 8KB Message Size
ax3 = axes[2]
ax3.plot(THREAD_COUNTS, TWO_COPY_SIZE8KB, 'o-', label='Two-Copy', linewidth=2, markersize=8)
ax3.plot(THREAD_COUNTS, ONE_COPY_SIZE8KB, 's-', label='One-Copy', linewidth=2, markersize=8)
ax3.plot(THREAD_COUNTS, ZERO_COPY_SIZE8KB, '^-', label='Zero-Copy', linewidth=2, markersize=8)
ax3.set_xlabel('Thread Count', fontsize=12)
ax3.set_ylabel('Latency (us)', fontsize=12)
ax3.set_title('Latency vs Thread Count\n(8KB Messages)', fontsize=14)
ax3.grid(True, alpha=0.3)
ax3.legend(loc='upper left')
ax3.set_xticks(THREAD_COUNTS)
ax3.set_xticklabels(THREAD_LABELS)

# Add main title and system config
fig.suptitle('Latency Comparison: Socket Communication Implementations\nMyself: Akash Singh (MT24110)', fontsize=14, fontweight='bold')
fig.text(0.5, 0.02, SYSTEM_CONFIG, ha='center', fontsize=10, style='italic')

plt.tight_layout(rect=[0, 0.05, 1, 0.95])
plt.savefig('MT24110_latency_vs_thread_count.pdf', dpi=150, bbox_inches='tight')
plt.savefig('MT24110_latency_vs_thread_count.png', dpi=150, bbox_inches='tight')
print("Generated: MT24110_latency_vs_thread_count.png")