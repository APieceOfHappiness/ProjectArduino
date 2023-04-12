import plotly.graph_objects as go
import plotly.express as px
import csv

with open("data.csv", encoding='utf-8') as r_file:
    reader_object = csv.reader(r_file, delimiter = ",")
    data = []
    for line in reader_object:
        data += list(map(int, line))
    
    data = data[-1000:]
    X = [x for x in range(len(data))]

    print(len(data))

    fig = go.Figure()
    fig.add_trace(go.Scatter(x=X, y=data))
    fig.show()

