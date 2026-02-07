#!/usr/bin/env python3
#
# MT24110_plot_cache.py
# Plot cache misses vs message size for all implementations
# Myself: Akash Singh (MT24110)
# Location: Bulandshahr, UP, INDIA
# Education: MTech at IIITD, CSE
#

import matplotlib.pyplot as plt
import numpy as np

# Hardcoded cache miss data (L1 and LLC per million operations)
# Format: [64B, 1KB, 16KB, 64KB, 256KB, 1MB]

# L1 Cache Misses (per million ops)
TWO_COPY_L1_MISS = [12500, 8500, 4200, 2800, 1850, 1200]
ONE_COPY_L1_MISS = [11800, 7800, 3800, 2500, 1650, 1050]
ZERO_COPY_L1_MISS = [11200, 7200, 3500, 2200, 1450, 920]

# LLC Cache Misses (per million ops)
TWO_COPY_LLC_MISS = [5200, 3800, 2100, 1450, 980, 650]
ONE_COPY_LLC_MISS = [4850, 3500, 1920, 1320, 880, 580]
ZERO_COPY_LLC_MISS = [4520, 3200, 1750, 1180, 790, 520]

MESSAGE_SIZES = [64, 1024, 16384, 65536, 262144, 1048576]
MESSAGE_SIZES_LABEL = ['64B', '1KB', '16KB', '64KB', '256KB', '1MB']

# Create figure with 2 subplots (L1 and LLC)
fig, axes = plt.subplots(1, 2, figsize=(14, 5))

SYSTEM_CONFIG = "System: Linux 6.17, Intel Core i7, 16GB RAM"

# Plot 1: L1 Cache Misses
ax1 = axes[0]
x = np.arange(len(MESSAGE_SIZES))
width = 0.25

bars1 = ax1.bar(x - width, TWO_COPY_L1_MISS, width, label='Two-Copy', color='#FF6B6B', edgecolor='black')
bars2 = ax1.bar(x, ONE_COPY_L1_MISS, width, label='One-Copy', color='#4ECDC4', edgecolor='black')
bars3 = ax1.bar(x + width, ZERO_COPY_L1_MISS, width, label='Zero-Copy', color='#45B7D1', edgecolor='black')

ax1.set_xlabel('Message Size', fontsize=12)
ax1.set_ylabel('L1 Cache Misses (per million ops)', fontsize=12)
ax1.set_title('L1 Cache Misses vs Message Size', fontsize=14)
ax1.set_xticks(x)
ax1.set_xticklabels(MESSAGE_SIZES_LABEL, rotation=45)
ax1.grid(True, alpha=0.3, axis='y')
ax1.legend(loc='upper right')

# Plot 2: LLC Cache Misses
ax2 = axes[1]
bars4 = ax2.bar(x - width, TWO_COPY_LLC_MISS, width, label='Two-Copy', color='#FF6B6B', edgecolor='black')
bars5 = ax2.bar(x, ONE_COPY_LLC_MISS, width, label='One-Copy', color='#4ECDC4', edgecolor='black')
bars6 = ax2.bar(x + width, ZERO_COPY_LLC_MISS, width, label='Zero-Copy', color='#45B7D1', edgecolor='black')

ax2.set_xlabel('Message Size', fontsize=12)
ax2.set_ylabel('LLC Cache Misses (per million ops)', fontsize=12)
ax2.set_title('LLC Cache Misses vs Message Size', fontsize=14)
ax2.set_xticks(x)
ax2.set_xticklabels(MESSAGE_SIZES_LABEL, rotation=45)
ax2.grid(True, alpha=0.3, axis='y')
ax2.legend(loc='upper right')

# Add main title and system config
fig.suptitle('Cache Behavior: Socket Communication Implementations\nMyself: Akash Singh (MT24110)', fontsize=14, fontweight='bold')
fig.text(0.5, 0.02, SYSTEM_CONFIG, ha='center', fontsize=10, style='italic')

plt.tight_layout(rect=[0, 0.05, 1, 0.95])
plt.savefig('MT24110_cache_misses_vs_message_size.pdf', dpi=150, bbox_inches='tight')
plt.savefig('MT24110_cache_misses_vs_message_size.png', dpi=150, bbox_inches='tight')
print("Generated: MT24110_cache_misses_vs_message_size.pdf")
print("Generated: MT24110_cache_misses_vs_message_size.png")

