import sys
import numpy as np
import matplotlib.pyplot as plt
import os
import struct


def set_labels(ax):
    ax[0, 0].set_title('Preasure')
    ax[0, 1].set_title('Temp')
    ax[1, 0].set_title('Sound')
    ax[1, 1].set_title('Light')
    ax[0, 0].set_xlabel('time')
    ax[0, 0].set_ylabel('kPa')
    ax[0, 1].set_xlabel('time')
    ax[0, 1].set_ylabel('C°')
    ax[1, 0].set_xlabel('time')
    ax[1, 0].set_ylabel('Db')
    ax[1, 1].set_xlabel('time')
    ax[1, 1].set_ylabel('L')


def update_canvas(x_axis_data, y_axis_data, fig, ax):
    for i in range(2):
        for j in range(2):
            ax[i, j].clear()
            ax[i, j].plot(x_axis_data[i * 2 + j], y_axis_data[i * 2 + j])
            u_lim = max(y_axis_data[i * 2 + j]) + 1
            d_lim = min(y_axis_data[i * 2 + j]) - 1
            ax[i, j].set(xlim=(x_axis_data[i * 2 + j][0], x_axis_data[i * 2 + j][-1]), ylim=(d_lim, u_lim))
    set_labels(ax)
    plt.draw()
    plt.pause(0.01)


if __name__ == "__main__":
    x_axis_data = [[], [], [], []]
    y_axis_data = [[], [], [], []]
    fig, ax = plt.subplots(2, 2)
    plt.subplots_adjust(hspace=0.5)
    plt.subplots_adjust(wspace=0.5)
    pipe = open('/tmp/my_pipe', 'rb')
    while True:
        for i in range(4):
            current = struct.unpack('d', pipe.read(8))[0]
            if len(x_axis_data[i]) == 0:
                x_axis_data[i].append(0)
            else:
                x_axis_data[i].append(x_axis_data[i][-1] + 1)
            y_axis_data[i].append(current)
            if len(x_axis_data[i]) > 50:
                x_axis_data[i].pop(0)
                y_axis_data[i].pop(0)

        update_canvas(x_axis_data, y_axis_data, fig, ax)

    pipe.close()
