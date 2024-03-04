import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df = pd.read_fwf('temp_data_3-4.txt')
df.drop(df.index[0:200], inplace=True)
# print(df)
df["time"] = range(0,len(df["temperature"]))
df["time"] = [x / 2 for x in df["time"]]
print(df.head())

temp_np = df["temperature"].to_numpy()
# print(temp_np)
temp_np = np.gradient(temp_np)
for (index,value) in enumerate(temp_np):
    if value > 30 or value < -20:
        temp_np[index] = 0

df["np_gradient"] = temp_np

df = df.assign(fire=0)
fire_ignited = False
for i in df.index:
    if df['np_gradient'][i] > 2:
        fire_ignited = True
    if fire_ignited and df['np_gradient'][i] < 0:
        fire_ignited = False
    if fire_ignited:
        df['fire'][i] = 1


print(df)
df.to_csv("output 3-4.csv")
# fix, ax = plt.subplots(1, 2)
# ax[0].scatter(df["time"],df["temperature"],s = 0.5)
# ax[0].set_title('Time vs. Temperature')
# ax[0].set_xlabel("Time (s)")
# ax[0].set_ylabel("Temperature (deg C)")
# ax[1].scatter(df["time"],df["gradient"],s = 0.5,c="red")
# ax[1].set_title('Time vs. Gradient')
# ax[1].set_xlabel("Time (s)")
# ax[1].set_ylabel("Gradient (deg C/ s)")
fig, ax1 = plt.subplots()
color = 'tab:blue'
ax1.set_xlabel('time (s)')
ax1.set_ylabel('temperature C', color=color)
ax1.scatter(df["time"],df["temperature"],s = 0.6, color = color)
ax1.tick_params(axis='y', labelcolor=color)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

color = 'tab:orange'
ax2.set_ylabel('gradient degC/s', color=color)  # we already handled the x-label with ax1
ax2.plot(df["time"],df["np_gradient"], color=color, linestyle = ':')
ax2.tick_params(axis='y', labelcolor=color)
ax2.plot(df["time"],df['fire'], color='red', linewidth = 1.3)

fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.grid(which='both',axis='both',linestyle = '--')
plt.show()