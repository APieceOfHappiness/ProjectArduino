import serial
import csv
import plotly.graph_objects as go
import plotly.express as px
s = serial.Serial('/dev/ttyACM0', 9600)


SPEED_OF_CHECKING = 100 
cnt = 0
data = []

for i in range(300):
    s.readline()


while ...:
    cnt = (cnt + 1) % 100
    if (cnt % 100) == 0:
        with open("data.csv", mode="a", encoding='utf-8') as w_file:
            file_writer = csv.writer(w_file, delimiter=',')
            file_writer.writerow(data)
            data = []
    data.append(int(str(s.readline())[2:-5]))
    print(data[len(data) - 1])