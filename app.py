import dash
from dash import dcc, html
import plotly.graph_objs as go
import serial
import time
from threading import Thread

# Initialize Dash
app = dash.Dash(__name__)

# Set up the serial port (update with your correct port and baud rate)
ser = serial.Serial('COM3', 9600, timeout=1)  # Replace 'COM3' with your serial port
time.sleep(2)  # Give the serial port a moment to initialize

# Global lists to store incoming data
times = []
altitudes = []

# Thread to read data from the serial port in the background
def read_serial_data():
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            try:
                # Assuming data format is: time,altitude
                current_time, altitude = map(float, line.split(','))
                times.append(current_time)
                altitudes.append(altitude)
            except ValueError:
                # Ignore lines that can't be parsed
                continue

# Start the serial reading thread
thread = Thread(target=read_serial_data)
thread.daemon = True
thread.start()

# Layout of the Dashboard
app.layout = html.Div([
    html.H1("Model Rocket Telemetry Dashboard"),
    dcc.Graph(id='live-update-graph'),
    dcc.Interval(
        id='interval-component',
        interval=1000,  # in milliseconds
        n_intervals=0
    )
])

# Callback to update the graph
@app.callback(
    dash.dependencies.Output('live-update-graph', 'figure'),
    [dash.dependencies.Input('interval-component', 'n_intervals')]
)
def update_graph_live(n):
    # Create a figure with the latest data
    fig = go.Figure()
    fig.add_trace(go.Scatter(x=times, y=altitudes, mode='lines+markers', name='Altitude'))

    # Customize layout
    fig.update_layout(
        title='Altitude over Time',
        xaxis=dict(title='Time (s)'),
        yaxis=dict(title='Altitude (m)'),
        showlegend=True
    )
    return fig

# Run the Dash app
if __name__ == '__main__':
    app.run_server(debug=True)