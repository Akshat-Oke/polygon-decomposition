# Decompose Polygon into convex polygons

## Creating polygons

Run `python3 draw.py gen` to generate polygons and save it to `input_py.txt`.

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

Run `bash run.bash`

## Output format

The program writes the decomposed (and merged) polgons to `output.txt` with the number of polygons on the first line and for each polygon, the number of points in that polygon followed by its points as in the input format.

Decomposed polygons not merged are saved to `decomposition.txt`.

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

The program opens a window displaying the decomposition without merging. On closing this window it opens another window showing the decomposition after merging.
