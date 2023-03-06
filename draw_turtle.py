import sys
import turtle

# Create a turtle object
t = turtle.Turtle()

def draw(filename, offset, scale=1):
  # Open the file and read the data
  with open(filename, "r") as file:
      num_polygons = int(file.readline())

      # Draw each polygon
      for i in range(num_polygons):
          num_points = int(file.readline())

          # Move the turtle to the first point of the polygon
          line1 = file.readline().split()
          x, y = map(float, line1)

          x += offset
          
          t.penup()
          t.goto(x, y)
          t.pendown()

          # Draw each point of the polygon
          for j in range(num_points - 1):
              x, y = map(float, file.readline().split())
              t.goto(x, y)

          # Return the turtle to the first point of the polygon to close it
          # line = file.readline().split()
          t.goto(float(line1[0]), float(line1[1]))

draw(sys.argv[1], 0)
# draw("decomposition.txt", 0)
# Hide the turtle when done
t.hideturtle()

# Exit the turtle window when clicked
turtle.exitonclick()
