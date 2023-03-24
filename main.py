from subprocess import call
from extract_country import save_country
import matplotlib.pyplot as plt
from generate import *
import sys

def decompose_and_save_country(country_name):
    save_country(country_name)
    call(["./main", "input_geo.txt"])
    all_decompositions("input_geo.txt", "input_geo.txt_decomp.txt", "input_geo.txt_merged.txt", country_name)


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

def read_draw(filename):
    x = []
    y = []
    count = -1

    with open(filename,'r') as file:
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
    x.append(x[0])
    y.append(y[0])

    plt.plot(x, y, color='black')
    # plt.plot(end_x, end_y, color='black')
    plt.show()

def get_points(filename):
    x = []
    y = []
    count = -1

    with open(filename,'r') as file:
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

    x.append(x[0])
    y.append(y[0])
    return x, y

def get_decomposition_points(filename):
    polygons = []
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
        x.append(x[0])
        y.append(y[0])
        polygons.append({"x": x, "y": y})
    return polygons

def all_decompositions(origfile, decomp1, decomp2, figname):
    original_x, original_y = get_points(origfile)
    decomposition1 = get_decomposition_points(decomp1)
    decomposition2 = get_decomposition_points(decomp2)
    #make a subplot of 1x3 and plot the original and the two decompositions
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3)
    fig.suptitle(figname)
    ax1.plot(original_x, original_y, color='black')
    ax1.set_title('Original')
    ax1.axis("equal")
    for polygon in decomposition1:
        ax2.fill(polygon["x"], polygon["y"], color=(random.random(), random.random(), random.random(), 0.5))
    ax2.set_title('Decomposition')
    ax2.axis("equal")
    for polygon in decomposition2:
        ax3.fill(polygon["x"], polygon["y"], color=(random.random(), random.random(), random.random(), 0.5))
    ax3.set_title('Merged')
    ax3.axis("equal")
    # increase the display window size
    fig.set_size_inches(17, 5)
    plt.show()
    fig.savefig("retype/fig/"+figname+".png")

    

def draw_decomposition(filename):
    file = open(filename, "r")
    lines = file.readlines()
    file.close()
    total_polygons = int(lines[0])
    cur_line = 1
    plt.figure();
    plt.axis("equal")
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
        # plt.plot(x, y, color='black')
        #get a random light color
        plt.fill(x, y, color=(random.random(), random.random(), random.random(), 0.5))
        # plt.fill(x, y, color='black')
        # plt.plot(end_x, end_y, color='black')
    plt.show()

countries = ["Germany", "Spain", "India", "South Korea", "Switzerland"]
if sys.argv[1] == 'country':
    # decompose_and_save_country(sys.argv[2])
    for c in countries:
        decompose_and_save_country(c)
elif sys.argv[1] == 'draw-all':
    all_decompositions(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
elif sys.argv[1] == 'decomp':
    draw_decomposition(sys.argv[2])
elif sys.argv[1] == 'gen':
	generate_draw_write(0.5, 0.3, int(sys.argv[2]), "input_gen.txt")
elif sys.argv[1] == 'draw':
	read_draw(sys.argv[2])
elif sys.argv[1] == 'gen_dir':
     for i in range(2, int(sys.argv[2]), 5):
          generate_draw_write(0.9, 0.4, 35, "test10/"+str(i) + ".txt")        

