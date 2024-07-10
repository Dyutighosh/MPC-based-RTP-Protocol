import matplotlib.pyplot as plt
from matplotlib.patches import Patch
import sys
import numpy as np
from matplotlib import rc,rcParams
import matplotlib.pyplot as plt
import matplotlib.patches as patches

# 70 MBps
# 9.62 MB


# Data
cases = ['Case 1', 'Case 2', 'Case 3', 'Case 4', 'Case 5', 'Case 6']
values = [5.6519521303, 26637, 9668.9, 9653.18, 9717.656, 3.2604]
colors = ['black', 'red', 'blue', 'green', 'cyan', 'magenta']
descriptions = [
    "SCALE-MAMBA (Garbled Circuit for Inverse)",  # Description for Case 1
    "SCALE-MAMBA (Garbled Circuit for Division)",        # Description for Case 2
    "MP-SPDZ Protocol with 100 Consumers",   # Description for Case 3
    "MP-SPDZ Protocol with 1000 Consumers",   # Description for Case 4
    "MP-SPDZ Protocol with 10000 Consumers",   # Description for Case 5
    "Our Proposed RTP Protocol"    # Description for Case 6
]


rc('font', weight='bold')

plt.figure(figsize=(9, 5))  # Adjusted for better visibility

# Plotting the bar chart
bars = plt.bar(cases, values, color=colors)

# Adding title and labels with bold font
plt.xlabel('RTP Implementation Scenarios', fontweight='bold', fontsize=12)
plt.ylabel('Execution Time per Consumer (milliseconds)', fontweight='bold', fontsize=12)

# Adjust y-axis limits to allow for text placement
plt.ylim(0, max(values) * 1.1)  # Add space above the highest value

# Adding value labels on each bar for clarity, also in bold
for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, yval + 0.05 * max(values), f'{yval:,.3f}', ha='center', va='bottom', fontweight='bold', color='black')

# Create legend with custom colors
legend_elements = [Patch(facecolor=col, label=f'{case}: {desc}') for case, col, desc in zip(cases, colors, descriptions)]
plt.legend(handles=legend_elements, title="Descriptions", title_fontsize='13', loc='upper right', framealpha=1, shadow=True)

plt.tight_layout()
plt.savefig('ExecutionTimes.png', dpi=500, bbox_inches='tight')

# Show the plot
plt.show()
