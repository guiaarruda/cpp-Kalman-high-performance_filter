import pandas as pd
import matplotlib.pyplot as plt


data = pd.read_csv('results.csv')


plt.figure(figsize=(10, 6))

plt.plot(data['time'], data['measured_x'], 'r.', label='Measurement', alpha=0.5)
plt.plot(data['time'], data['x'], 'g--', label='Real', linewidth=2)    
plt.plot(data['time'], data['estimated_x(kalman)'], 'b-', label='Kalman Filter', linewidth=2) 

plt.title('Kalman Filter Performance')
plt.xlabel('Time (s)')
plt.ylabel('Position X (m)')
plt.legend()
plt.grid(True)


plt.savefig('kalman_result.png')
print("graph saved as 'kalman_result.png'")