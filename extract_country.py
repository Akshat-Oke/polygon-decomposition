import json
with open("countries.geo.json") as file:
    data = json.load(file)

countries = data["features"]
# Find country with name Italy
# json is {properties: {name: "Italy"}}
# find Italy
for i in range(len(countries)):
    if countries[i]["properties"]["name"] == "Spain":
        print(i)
        with open("input_geo.txt", "w") as file:
          file.write(str(len(countries[i]["geometry"]["coordinates"][0])) + "\n")
          for coordinate in (countries[i]["geometry"]["coordinates"][0]):
            file.write(str(coordinate[0]) + " " + str(coordinate[1]) + "\n")
            
        break