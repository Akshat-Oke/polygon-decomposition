import matplotlib.pyplot as plt
 
# Read the data from the text file
# final_times.txt
with open('timelog.txt') as f:
    data = [line.strip().split() for line in f]
 
# Convert the data to integers
data = [(int(n), int(t)) for n, t in data]
data.sort()
# Extract the x and y values from the data
x_values = [n for n, t in data]
y_values = [t for n, t in data]
 
# Create the plot
plt.plot(x_values, y_values)
 
# Add labels and title to the plot
plt.xlabel('Start vertex number')
plt.ylabel('Time (um)')
plt.title('Time vs Start Vertex')
plt.savefig('plot2.png')