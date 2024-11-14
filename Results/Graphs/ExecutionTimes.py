import matplotlib.pyplot as plt
from matplotlib.patches import Patch
from matplotlib import rc

# Data
cases = ['Case 1', 'Case 2', 'Case 3', 'Case 4']
values = [3.2604, 5.6519521303, 6637.656, 4653.932]  # Rearranged values
colors = ['blue', 'black', 'red', 'green']  # Updated color order to match cases
descriptions = [
    "Our Proposed RTP Protocol",  # Case 1
    "SCALE-MAMBA (Garbled Circuit for Inverse)",  # Case 2
    "SCALE-MAMBA (Garbled Circuit for Division)",  # Case 3
    "MP-SPDZ (for division)"  # Case 4
]

rc('font', weight='bold')

plt.figure(figsize=(8, 4))

# Plotting the bar chart
bars = plt.bar(cases, values, color=colors)

# Adding title and labels with bold font
plt.xlabel('RTP Implementation Scenarios', fontweight='bold', fontsize=16)
plt.ylabel('Execution Time per Consumer (ms)', fontweight='bold', fontsize=12.5)

# Set x and y ticks font size
plt.xticks(fontsize=13)
plt.yticks(fontsize=13)

# Adjust y-axis limits to allow for text placement
plt.ylim(0, max(values) * 1.1)

# Adding value labels inside bars for Cases 2 and 3, and on top for others
for i, bar in enumerate(bars):
    yval = bar.get_height()
    if i == 2 or i == 3:  # Place values inside for Cases 2 and 3
        plt.text(bar.get_x() + bar.get_width() / 2, yval / 2, f'{yval:,.3f}', ha='center', va='center', fontweight='bold', color='white', fontsize=13)
    else:  # Place values on top for other cases
        plt.text(bar.get_x() + bar.get_width() / 2, yval + 0.05 * max(values), f'{yval:,.3f}', ha='center', va='bottom', fontweight='bold', color='black', fontsize=14)

# Create legend with custom colors
legend_elements = [Patch(facecolor=col, label=f'{case}: {desc}') for case, col, desc in zip(cases, colors, descriptions)]
plt.legend(handles=legend_elements, loc='upper right', framealpha=1, shadow=False, fontsize=13)

plt.tight_layout()
plt.savefig('ExecutionTimes1.png', dpi=500, bbox_inches='tight')

# Show the plot
plt.show()
