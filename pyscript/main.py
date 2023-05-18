import sys
import numpy as np
import matplotlib.pyplot as plt


def update_canvas(x_axis_data, y_axis_data, fig, ax, x_lim, y_lim):
    ax.clear()
    ax.plot(x_axis_data, y_axis_data)
    plt.xlabel(x_label, fontsize=14)
    plt.xlabel(y_label, fontsize=14)
    ax.set(xlim=(0, x_lim), ylim=(0, y_lim))
    plt.draw()
    plt.pause(0.01)


if __name__ == "__main__":
    if len(sys.argv) != 5:
        exit(1)
    x_label = sys.argv[1]
    y_label = sys.argv[2]
    data_file = sys.argv[3]
    lock_file = sys.argv[4]

    x_axis_data = [0]
    y_axis_data = [0]

    fig, ax = plt.subplots(1, 1)
    update_canvas(x_axis_data, y_axis_data, fig, ax, 1, 1)
    max_data = 0.0

    while True:
        lock_f = open(lock_file, 'r')
        if lock_f.read() == 'r':
            data_f = open(data_file, 'r')
            x_axis_data.append(len(x_axis_data))
            value_data = float(data_f.read())
            max_data = max(max_data, value_data)
            y_axis_data.append(value_data)
            update_canvas(x_axis_data, y_axis_data, fig, ax, len(x_axis_data), max_data + 1)
            data_f.close()

            lock_fw = open(lock_file, 'w')
            lock_fw.write('w')
            lock_fw.close()
        elif lock_f.read() == 't':
            break
        lock_f.close()
    