import matplotlib.pyplot as plt
from generate import *
import sys

def generate_draw_write(irreg, spike, vert_count):
    vertices = generate_polygon(center=(250, 250),
                                avg_radius=100,
                                irregularity=irreg,
                                spikiness=spike,
                                num_vertices=vert_count)

    x = []
    y = []

    for i in range(len(vertices)):
        x.append(vertices[i][0])
        y.append(vertices[i][1])

    end_x = [x[0], x[len(vertices)-1]]
    end_y = [y[0], y[len(vertices)-1]]

    plt.plot(x, y, color='black')
    plt.plot(end_x, end_y, color='black')
    plt.show()

    vertices.reverse()
    with open('input.txt', 'w') as file:
        file.write(str(len(vertices)) + '\n')
        for element in vertices:
            a, b = element
            file.write(str(a) + " " + str(b) + '\n')

def read_draw():
    x = []
    y = []
    count = -1

    with open('input.txt','r') as file:
        for line in file:
            if count == -1:
                count += 1
                continue

            for word in line.split():
                if count%2 == 0:
                    x.append(float(word))
                else:
                    y.append(float(word))
                count += 1

    end_x = [x[0], x[len(x)-1]]
    end_y = [y[0], y[len(y)-1]]

    plt.plot(x, y, color='black')
    plt.plot(end_x, end_y, color='black')
    plt.show()

if sys.argv[1] == '0':
	generate_draw_write(0.5, 0.7, 25)
elif sys.argv[1] == '1':
	read_draw()
