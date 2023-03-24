# Polygon Decomposition {#mainpage}

# Decompose Polygon into convex polygons

## Creating polygons

### Format of a polygon file

For a polygon of n points, the file has n+1 lines of which the first line has the number of vertices and the remaining n lines have the vertices listed one on each line.
A vertex on a line has x and coordinates separated by a space.

```polygon.txt
3
0 0
0 1
1 1
```

### One polygon

`python3 draw.py gen [number of points]` saves the list of vertices in clockwise order in `input_gen.txt`.

### Multiple polygons

Run `python3 draw.py gen_dir [number_of_polygons]` to generate polygons and save them to `tests/`.

> Note that tests/ directory should exist.

Polygons are saved in `[number_of_points].txt` files. Ex. `24.txt` will have 24 vertices.

## Visualizing the polygons

### Visualizing a (maybe nonconvex) polygon

`python3 draw.py draw [filename]`

### Visualizing a decomposition

`python3 draw.py decomp [filename]`

## Input format

Enter the polygon in `input_py.txt` (or any other file) with each point on one line with its coordinates (x, y) separated by a space. Specify total number of points on the first line.

> The points must be in clockwise order

```
[number of points]
x1 y1
x2 y2
...
```

## Running the program

Compile the program: `g++ src/main.cpp src/dcel.cpp src/merge.cpp -o main`

You can run a single file or a series of testcases in a folder. Specify the path to the file or folder as the command line argument.

Ex. `./main spain.txt` will decompose the polygon in `spain.txt`. This will log the time taken in `timelog.txt` in microseconds.
Ex. `./main countries/` will decompose all polygons in `countries/` and log the time taken and number of vertices in `timelog.txt` in microseconds.

## Output format

The program writes the decomposed (and merged) polgons to `[filename]_merged.txt` with the number of polygons on the first line and for each polygon, the number of points in that polygon followed by its points as in the input format.

Decomposed polygons not merged are saved to `[filename]_decomposition.txt`.
Ex. `./main countries/spain.txt` will save the decomposition (without merging) to `countries/spain_decomposition.txt` and save the merged decomposition to `countries/spain_merged.txt`

```
[number of polygons]
[points in polygon 1]
x1 y1
x2 y2
[points in polygon 2]
x1 y1
x2 y2
...
```

You can then use the visualizing programs mentioned above the view the output.
