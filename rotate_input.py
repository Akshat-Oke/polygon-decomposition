import sys
from subprocess import call

# def rotate_input():
#   with open(sys.argv[1], 'r+') as f:
#       lines = f.readlines()
#       second_line = lines.pop(1)
#       f.seek(0)
#       f.writelines(lines)
#       f.write(second_line)

# for i in range(50):
#   rotate_input()
#   call(["./main", sys.argv[1]])

with open(sys.argv[1] , 'r+') as f:
    lines = f.readlines();
    f.seek(0)
    #write back each line, with the line number in front
    for i in range(len(lines)):
        f.write(str(i+1) + " " + lines[i])
    f.close()