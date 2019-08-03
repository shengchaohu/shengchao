import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import _open

temp=list(map(_open.keep_two,['mean','std']))
std=temp[1]
mean=temp[0]
k=list(range(1,len(std)+1))
s=['S'+str(kk) for kk in k]
fig, ax = plt.subplots()
ax.bar(np.arange(len(std)), mean, yerr=std, align='center', alpha=0.5, ecolor='black', capsize=10)
ax.set_ylabel('Mean attention')
ax.set_xticks(np.arange(len(std)))
ax.set_xticklabels(s)
ax.set_title('Mean attention by student')
ax.yaxis.grid(True)

# Save the figure and show
plt.tight_layout()
plt.savefig('bar_plot_with_error_bars.png')
plt.show()