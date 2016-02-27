# TSP
Basic TSP algorithm based on Lat and Long cords for robo-ops.

## Usage
List of nodes should be placed in file [nodes.txt](/nodes.txt).
Format for nodes.txt should be colon delimited, one LAT:LONG:COLOR per line where LAT and LONG denote latitude and longitude coordinates and color should be the color of the rock at coordinates.
Assumed first node is Mount Kosmo and second node is current location of BigDaddy Rover.

## Using

#### Compile
```
g++ -o TSP main.cpp
```

#### Run
```
./TSP
```

#### Example
An example [nodes.txt](/nodes.txt) file is provided.
