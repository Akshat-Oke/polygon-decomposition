from flask import Flask, redirect, url_for, request, jsonify, send_from_directory
from subprocess import call
app = Flask(__name__)

def writePolygon(polygon):
   with open("api_in.txt", "w") as f:
      f.write(str(len(polygon)) + "\n")
      for point in polygon:
          f.write(str(point[0]) + " " + str(point[1]) + "\n")

def decomp_to_array(decomp):
   decomposition = []
   lines = decomp.splitlines()
   current_line = 0
   for i in range(int(lines[0].strip())):
      decomposition.append([])
      current_line += 1
      for j in range(int(lines[current_line].strip())):
         current_line += 1
         decomposition[i].append([float(lines[current_line].split()[0]), float(lines[current_line].split()[1])])
   return decomposition

def get_decomposition():
   output = ""
   with open("api_in.txt_decomp.txt", "r") as f:
      output= f.read()
   return decomp_to_array(output)
   
def get_merged():
    with open("api_in.txt_merged.txt", "r") as f:
      output= f.read()
      return decomp_to_array(output)

def run_decomposition(polygon):
   writePolygon(polygon)
   call(["./main", "api_in.txt"])
   return get_decomposition()

@app.route('/success/<name>')
def success(name):
   return 'welcome %s' % name

@app.route("/")
def index():
   return send_from_directory('jsapp', 'index.html')
@app.route("/<path:path>")
def indexPath(path):
   return send_from_directory('jsapp', path)
   # return send_from_directory('jsapp', 'index.html')

@app.route('/decompose',methods = ['POST'])
def login():
   polygon = request.get_json(force=True)['polygon']
   decomposition = run_decomposition(polygon)
   # decomposition = get_decomposition()
   merged = get_merged()
   response = jsonify({"decomposition": decomposition, "merged": merged})
   response.headers.add('Access-Control-Allow-Origin', '*')
   # print(response)
   return (response)

if __name__ == '__main__':
   app.run(debug = True)