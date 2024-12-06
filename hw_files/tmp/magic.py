import pandas as pd
import matplotlib.pyplot as plt

# Load data from the CSV file
data = pd.read_csv('magic.csv', skiprows=1)  # Skip the first row which is the title

# Display the data for verification
print(data)

# Output data into magic_square_performance.txt file
with open('magic_square_performance.txt', 'w') as f:
    f.write(data.to_string())

# Plot Total Time vs. Size
plt.figure(figsize=(12, 6))

# Grouping by Size to average out Total Time across Iterations
avg_total_time = data.groupby('Size')['Total Time (s)'].mean()
plt.subplot(1, 2, 1)
avg_total_time.plot(kind='bar', color='skyblue', edgecolor='black')
plt.title('Average Total Time vs. Size of Magic Square')
plt.xlabel('Size of Magic Square')
plt.ylabel('Average Total Time (seconds)')
plt.xticks(rotation=45)
plt.grid(axis='y')

# Plot Average Time vs. Size
avg_avg_time = data.groupby('Size')['Average Time (s)'].mean()
plt.subplot(1, 2, 2)
avg_avg_time.plot(kind='bar', color='lightgreen', edgecolor='black')
plt.title('Average Time per Iteration vs. Size of Magic Square')
plt.xlabel('Size of Magic Square')
plt.ylabel('Average Time per Iteration (seconds)')
plt.xticks(rotation=45)
plt.grid(axis='y')

# Show the plots
plt.tight_layout()
plt.savefig('magic_square_performance.png')  # Save the figure
plt.show()

