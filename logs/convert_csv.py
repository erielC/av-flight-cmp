import csv
import re

# Define input and output files
input_file = 'device-monitor-241106-175444.log'
output_file = 'magnetometer_data.csv'

# Regular expressions to extract X, Y, Z values
x_pattern = re.compile(r"X axis field \(Gauss\): ([\-\d\.]+)")
y_pattern = re.compile(r"Y axis field \(Gauss\): ([\-\d\.]+)")
z_pattern = re.compile(r"Z axis field \(Gauss\): ([\-\d\.]+)")

# Initialize lists to store the parsed data
data = []

# Read the .log file and extract magnetic field values
with open(input_file, 'r') as log_file:
    for line in log_file:
        # Check if the line contains X, Y, Z field data
        x_match = x_pattern.search(line)
        y_match = y_pattern.search(line)
        z_match = z_pattern.search(line)
        
        # If all three values are found, store them as a new row
        if x_match and y_match and z_match:
            x_value = float(x_match.group(1))
            y_value = float(y_match.group(1))
            z_value = float(z_match.group(1))
            data.append([x_value, y_value, z_value])

# Write the extracted data to a .csv file
with open(output_file, 'w', newline='') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(['X (Gauss)', 'Y (Gauss)', 'Z (Gauss)'])  # Column headers
    writer.writerows(data)  # Write all data rows

print(f"Data extracted and saved to {output_file}")
