import sys
import numpy as np
import matplotlib.pyplot as plt
import os
import struct


def update_canvas(x_axis_data, y_axis_data, fig, ax, y_lim):
    for i in range(2):
        for j in range(2):
            ax[i, j].clear()
            ax[i, j].plot(x_axis_data[i * 2 + j], y_axis_data[i * 2 + j])
            u_lim = y_lim[i * 2 + j] + 1
            ax[i, j].set(xlim=(0, len(x_axis_data[i * 2 + j])), ylim=(0, u_lim))
    plt.draw()
    plt.pause(0.01)


if __name__ == "__main__":
    x_axis_data = [[0], [0], [0], [0]]
    y_axis_data = [[0], [0], [0], [0]]

    fig, ax = plt.subplots(2, 2)
    update_canvas(x_axis_data, y_axis_data, fig, ax, [1, 1, 1, 1])
    max_data = [0.0, 0.0, 0.0, 0.0]

    pipe = open('/tmp/my_pipe', 'rb')
    while True:
        for i in range(4):
            current = struct.unpack('d', pipe.read(8))[0]
            x_axis_data[i].append(len(x_axis_data[i]))
            max_data[i] = max(max_data[i], current)
            y_axis_data[i].append(current)

        update_canvas(x_axis_data, y_axis_data, fig, ax, max_data)

    pipe.close()
