#Created by: H. Timmermann - https://github.com/gottemmm
#Last updated: 7/25/21
#This code analyzes CO2 data; it sorts out outliers, plots all CO2 data as scatter points and plots the average CO2 level along with the best fit line for the data

#Importing required libraries
import matplotlib.pyplot as plt
import numpy as np
from collections import Counter
import time

#Input .txt file name here
array_from_file = np.loadtxt("INPUT HERE", dtype=str)

#Measuring time 
start_time = time.perf_counter()

#Setting up the figure 
fig = plt.figure()
a1 = fig.add_axes([0,0,1,1])

#This prints out all np data in arrange; used for debugging
np.set_printoptions(threshold=np.inf)

#Setting standard deviation, using it to sort out outliers 
dataset = []

for i in np.arange(len(array_from_file)):
    dataset.append(float(array_from_file[i][0]))

#Can change the number if you want more/less outliers 
std1 = np.mean(dataset) - .46*np.std(dataset)
std2 = np.mean(dataset) + .41*np.std(dataset)

outlier_list = []

for i in np.arange(len(array_from_file)):
    if std1 <= float(array_from_file[i][0]) <= std2:
        pass
    else:
        outlier_list.append(i)
    
#Outlier free data in array format (CO2 datapoint, time, date)
non_outlier_data = np.delete(array_from_file,outlier_list,0)

#Best Fit Line Calculation

def best_fit(x,y):
    w, b  = np.polyfit(x,y,deg=1)
    line  = w * x + b
    return line

non_outlier_data_list = []
for i in np.arange(len(non_outlier_data)):
    non_outlier_data_list.append(float(non_outlier_data[i][0]))

x1 = np.array(non_outlier_data_list)
line = best_fit(np.arange(x1.shape[0]),x1)

#Total of all CO2 levels; used for average CO2 level line 
total = 0
for ele in range(0, len(non_outlier_data)): 
    total = total + float(non_outlier_data[ele][0])

#Coloring the scatter points; green(0-800), yellow(800-1000), red(1000+)

length = np.arange(0,len(non_outlier_data))
col =[]

for i in range(0, len(non_outlier_data)):
  if non_outlier_data[i][0] in range(0,800):
      col.append('green')  
  if non_outlier_data[i][0] in range(800,1000):
      col.append('yellow')
  else:
      col.append('red') 

#Graphing
    
    
#Scatter plot with colored points; also graphing a random scatter point with the most popular color for the legend 

plt.scatter([10000], [0], color = Counter(col).most_common(1)[0][0], label ='CO2 Data point ')
for i in range(0,len(non_outlier_data)):
    plt.scatter(non_outlier_data[i][1], non_outlier_data_list[i], c = col[i], s = 10, linewidth = 0)
z = np.arange(0, len(non_outlier_data),step=50)

#Plotting only 11 ticks on the x-axis; can change 

xtick1 = int(len(non_outlier_data)/10)
plt.xticks(np.arange(0, len(non_outlier_data), xtick1),rotation=30)

#Graphs average CO2 level
davg = (total/len(non_outlier_data))
plt.axhline(y=davg, color='black', linestyle='--', label = 'Average CO2 Level')

#Graphs Best Fit Line

plt.plot(np.arange(len(non_outlier_data)), line, color='blue', label = 'Best Fit')

#prints max/min CO2 levels
print('The maximum CO2 level was: ' + str(max(non_outlier_data_list)) + ' PPM')
print('The minimum CO2 level was: ' + str(min(non_outlier_data_list)) + ' PPM')

#Labeling and setting limits 
plt.xlabel('Time (HH:MM:SS)')
plt.ylabel('CO2 levels (PPM)')
plt.title('CO2 Levels on ' + array_from_file[0][2])
a1.set_xlim(0,len(non_outlier_data_list))
a1.set_ylim(min(non_outlier_data_list)-50, max(non_outlier_data_list)+50)
plt.legend()
plt.show()

#Printing total time taken - (Time will vary depending on length of .txt file)
end_time = time.perf_counter()
print('This took:', end_time - start_time, 'seconds')
if (end_time - start_time) > 60:
    print('or', (end_time - start_time)/60, 'minutes')

#WORKED AT 61.52 data points plotted per second 
