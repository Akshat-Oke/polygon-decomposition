import matplotlib.pyplot as plt
from generate import *
import sys

def generate_draw_write(irreg, spike, vert_count, filename):
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

    # plt.plot(x, y, color='black')
    # plt.plot(end_x, end_y, color='black')
    # plt.show()

    vertices.reverse()
    with open(filename, 'w') as file:
        file.write(str(len(vertices)) + '\n')
        for element in vertices:
            a, b = element
            file.write(str(a) + " " + str(b) + '\n')

def read_draw():
    x = []
    y = []
    count = -1

    with open('input_geo.txt','r') as file:
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

def draw_decomposition(filename):
    file = open(filename, "r")
    lines = file.readlines()
    file.close()
    total_polygons = int(lines[0])
    cur_line = 1
    for i in range(total_polygons):
        num_points = int(lines[cur_line])
        cur_line += 1
        x = []
        y = []
        for j in range(num_points):
            x.append(float(lines[cur_line].split()[0]))
            y.append(float(lines[cur_line].split()[1]))
            cur_line += 1
        end_x = [x[0], x[len(x)-1]]
        end_y = [y[0], y[len(y)-1]]
        plt.plot(x, y, color='black')
        plt.plot(end_x, end_y, color='black')
    plt.show()

if sys.argv[1] == 'decomp':
    draw_decomposition(sys.argv[2])
elif sys.argv[1] == 'gen':
	generate_draw_write(0.5, 0.3, int(sys.argv[2]), "input_gen.txt")
elif sys.argv[1] == 'draw':
	read_draw()
elif sys.argv[1] == 'gen_dir':
     for i in range(1000, int(sys.argv[2]), 5):
          generate_draw_write(0.5, 0.3, i + 10, "test10/"+str(i) + ".txt")        

