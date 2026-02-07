#!/usr/bin/env python3
#
# MT24110_plot_throughput.py
# Plot throughput vs message size for all implementations
# Myself: Akash Singh (MT24110)
# Location: Bulandshahr, UP, INDIA
# Education: MTech at IIITD, CSE
#

import matplotlib.pyplot as plt
import numpy as np

# Hardcoded throughput data (Gbps) from experiments
# Format: [msg_size_512B, msg_size_1KB, msg_size_4KB, msg_size_8KB]
TWO_COPY_THREAD4 = [0.002, 0.0038, 0.0294, 0.0346]
ONE_COPY_THREAD4 = [0.0021, 0.0041, 0.0312, 0.0362]
ZERO_COPY_THREAD4 = [0.0021, 0.0041, 0.0312, 0.0385]

TWO_COPY_THREAD6 = [0.0029, 0.0062, 0.0317, 0.0452]
ONE_COPY_THREAD6 = [0.0028, 0.0059, 0.0277, 0.0453]
ZERO_COPY_THREAD6 = [0.0029, 0.0057, 0.0309, 0.0434]

TWO_COPY_THREAD8 = [0.0038, 0.007, 0.035, 0.0465]
ONE_COPY_THREAD8 = [0.0041, 0.0072, 0.0341, 0.0445]
ZERO_COPY_THREAD8 = [0.0044, 0.0053, 0.0325, 0.0254]

MESSAGE_SIZES = [512, 1024, 4096, 8192]
MESSAGE_SIZES_LABEL = ['512B', '1KB', '4KB', '8KB']

# Create figure with 3 subplots
fig, axes = plt.subplots(1, 3, figsize=(15, 5))

# System configuration info
SYSTEM_CONFIG = "System: Linux 6.17, Intel Core i5, 8GB RAM"

# Plot 1: 4 Threads
ax1 = axes[0]
ax1.plot(MESSAGE_SIZES, TWO_COPY_THREAD4, 'o-', label='Two-Copy', linewidth=2, markersize=8)
ax1.plot(MESSAGE_SIZES, ONE_COPY_THREAD4, 's-', label='One-Copy', linewidth=2, markersize=8)
ax1.plot(MESSAGE_SIZES, ZERO_COPY_THREAD4, '^-', label='Zero-Copy', linewidth=2, markersize=8)
ax1.set_xlabel('Message Size', fontsize=12)
ax1.set_ylabel('Throughput (Gbps)', fontsize=12)
ax1.set_title('Throughput vs Message Size\n(4 Threads)', fontsize=14)
ax1.set_xscale('log')
ax1.grid(True, alpha=0.3)
ax1.legend(loc='upper left')
ax1.set_xticks(MESSAGE_SIZES)
ax1.set_xticklabels(MESSAGE_SIZES_LABEL, rotation=45)

# Plot 2: 6 Threads
ax2 = axes[1]
ax2.plot(MESSAGE_SIZES, TWO_COPY_THREAD6, 'o-', label='Two-Copy', linewidth=2, markersize=8)
ax2.plot(MESSAGE_SIZES, ONE_COPY_THREAD6, 's-', label='One-Copy', linewidth=2, markersize=8)
ax2.plot(MESSAGE_SIZES, ZERO_COPY_THREAD6, '^-', label='Zero-Copy', linewidth=2, markersize=8)
ax2.set_xlabel('Message Size', fontsize=12)
ax2.set_ylabel('Throughput (Gbps)', fontsize=12)
ax2.set_title('Throughput vs Message Size\n(6 Threads)', fontsize=14)
ax2.set_xscale('log')
ax2.grid(True, alpha=0.3)
ax2.legend(loc='upper left')
ax2.set_xticks(MESSAGE_SIZES)
ax2.set_xticklabels(MESSAGE_SIZES_LABEL, rotation=45)

# Plot 3: 8 Threads
ax3 = axes[2]
ax3.plot(MESSAGE_SIZES, TWO_COPY_THREAD8, 'o-', label='Two-Copy', linewidth=2, markersize=8)
ax3.plot(MESSAGE_SIZES, ONE_COPY_THREAD8, 's-', label='One-Copy', linewidth=2, markersize=8)
ax3.plot(MESSAGE_SIZES, ZERO_COPY_THREAD8, '^-', label='Zero-Copy', linewidth=2, markersize=8)
ax3.set_xlabel('Message Size', fontsize=12)
ax3.set_ylabel('Throughput (Gbps)', fontsize=12)
ax3.set_title('Throughput vs Message Size\n(8 Threads)', fontsize=14)
ax3.set_xscale('log')
ax3.grid(True, alpha=0.3)
ax3.legend(loc='upper left')
ax3.set_xticks(MESSAGE_SIZES)
ax3.set_xticklabels(MESSAGE_SIZES_LABEL, rotation=45)

# Add main title and system config
fig.suptitle('Throughput Comparison: Socket Communication Implementations\nMyself: Akash Singh (MT24110)', fontsize=14, fontweight='bold')
fig.text(0.5, 0.02, SYSTEM_CONFIG, ha='center', fontsize=10, style='italic')

plt.tight_layout(rect=[0, 0.05, 1, 0.95])
plt.savefig('MT24110_throughput_vs_message_size.pdf', dpi=150, bbox_inches='tight')
plt.savefig('MT24110_throughput_vs_message_size.png', dpi=150, bbox_inches='tight')
print("Generated: MT24110_throughput_vs_message_size.png")