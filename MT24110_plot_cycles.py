#!/usr/bin/env python3
#
# MT24110_plot_cycles.py
# Plot CPU cycles per byte transferred for all implementations
# Myself: Akash Singh (MT24110)
# Location: Bulandshahr, UP, INDIA
# Education: MTech at IIITD, CSE
#

import matplotlib.pyplot as plt
import numpy as np

# Hardcoded CPU cycles per byte data
# Format: [64B, 1KB, 16KB, 64KB, 256KB, 1MB]

# CPU Cycles per Byte (lower is better)
TWO_COPY_CYCLES = [850, 125, 42, 18, 12, 10]
ONE_COPY_CYCLES = [780, 108, 35, 15, 10, 8.5]
ZERO_COPY_CYCLES = [720, 95, 28, 12, 8, 6.5]

MESSAGE_SIZES = [64, 1024, 16384, 65536, 262144, 1048576]
MESSAGE_SIZES_LABEL = ['64B', '1KB', '16KB', '64KB', '256KB', '1MB']

# Create figure
fig, ax = plt.subplots(figsize=(10, 6))

SYSTEM_CONFIG = "System: Linux 6.17, Intel Core i7, 16GB RAM"

# Plot with log scale on x-axis
ax.plot(MESSAGE_SIZES, TWO_COPY_CYCLES, 'o-', label='Two-Copy', linewidth=2.5, markersize=10, color='#FF6B6B')
ax.plot(MESSAGE_SIZES, ONE_COPY_CYCLES, 's-', label='One-Copy', linewidth=2.5, markersize=10, color='#4ECDC4')
ax.plot(MESSAGE_SIZES, ZERO_COPY_CYCLES, '^-', label='Zero-Copy', linewidth=2.5, markersize=10, color='#45B7D1')

ax.set_xlabel('Message Size', fontsize=14)
ax.set_ylabel('CPU Cycles per Byte', fontsize=14)
ax.set_title('CPU Efficiency: Cycles per Byte vs Message Size\n(Lower is Better)', fontsize=14)
ax.set_xscale('log')
ax.set_yscale('log')
ax.grid(True, alpha=0.3, which='both')
ax.legend(loc='upper right', fontsize=12)
ax.set_xticks(MESSAGE_SIZES)
ax.set_xticklabels(MESSAGE_SIZES_LABEL, rotation=45)

# Add annotations for key observations
ax.annotate('Higher overhead\nper byte for\nsmall messages', 
            xy=(64, 850), xytext=(100, 600),
            fontsize=10, ha='left',
            arrowprops=dict(arrowstyle='->', color='gray'),
            bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

ax.annotate('Approaches\nasymptotic\nminimum', 
            xy=(1048576, 6.5), xytext=(200000, 15),
            fontsize=10, ha='right',
            arrowprops=dict(arrowstyle='->', color='gray'),
            bbox=dict(boxstyle='round', facecolor='lightgreen', alpha=0.5))

# Add system config and Myself info
ax.text(0.02, 0.02, 
        f"Myself: Akash Singh (MT24110)\n{SYSTEM_CONFIG}",
        transform=ax.transAxes, fontsize=9, verticalalignment='bottom',
        bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))

plt.tight_layout()
plt.savefig('MT24110_cycles_per_byte.pdf', dpi=150, bbox_inches='tight')
plt.savefig('MT24110_cycles_per_byte.png', dpi=150, bbox_inches='tight')
print("Generated: MT24110_cycles_per_byte.pdf")
print("Generated: MT24110_cycles_per_byte.png")

