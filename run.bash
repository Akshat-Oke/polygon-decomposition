g++ src/main.cpp src/dcel.cpp src/merge.cpp -o main
./main input_py.txt
# python3 draw_turtle.py output.txt & python3 draw_turtle.py decomposition.txt && fg
python3 draw_turtle.py decomposition.txt
python3 draw_turtle.py output.txt