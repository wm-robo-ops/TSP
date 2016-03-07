#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <utility>

#define earthRadiusM 6371000.0


using namespace std;

//Kosmo must be first node and the rover must be second node
vector< vector<double> > nodes;
//Anyting but valid colors will result in 0 value (note alien = 7pts)
vector<string> colors;
//Store the min route here
vector<int> min_route;
//Min path distance
double min_distance = HUGE_VAL;
//Store the number of each rock node here (start with 2)
vector<int> rock_nodes;
//total points from best route
int total_pts;

void outputJSON();
int getTotalPoints();
int rockColorValue (string color);
void swap(int& a, int& b);
void printVector(vector<int>& theVector);
void generateAllPermutations(vector<int>& toBePermuted, unsigned int nextIndex);
void generateAllPermutations(vector<int>& toBePermuted); 
void fillNodes ();
double calculateTotalDistance(vector<int> rock_nodes, vector< vector<double> > nodes);
double cordsToDist(vector<double> cord1, vector<double> cord2);
double deg2rad(double deg);
double rad2deg(double rad);



int main() {

	//read in nodes.txt
	fillNodes();	
	
	//need at least Mount Kosmo and BigDaddy location
	if (nodes.size() < 2) {
		cerr << "not enough nodes\n";
		return 1;
	}
  	
	//find min distance
	generateAllPermutations(rock_nodes);
	
	//print results
	//cout << "min distance: " << min_distance << "\n";
	
	//cout << "order: ";
	//printVector(rock_nodes);

	//total_pts = getTotalPoints();
	//cout << "total points: " << total_pts << "\n";

	outputJSON();

}

void outputJSON() {
	
	cout << "{\n\t\"nodes\":\n\t\t[\n";
	
	
	//add rover position first to route
	string color = colors[1];
	cout << "\t\t\t{ \"lat\": \"" << nodes[1][0] << "\", \"lon\": \"" << nodes[1][1] << "\", \"color\": \"" << color << "\" },\n";
	
	for (unsigned int i = 0; i < nodes.size(); i++) {
		
		color = colors[i];
		transform(color.begin(), color.end(), color.begin(), ::tolower);
		cout << "\t\t\t{ \"lat\": \"" << nodes[i][0] << "\", \"lon\": \"" << nodes[i][1] << "\", \"color\": \"" << color << "\" },\n";

	}

	//add mount kosmo position to route
	color = colors[0];
	cout << "\t\t\t{ \"lat\": \"" << nodes[0][0] << "\", \"lon\": \"" << nodes[0][1] << "\", \"color\": \"" << color << "\" }\n";
	
	
	cout << "\t\t],\n";

	cout << "\t\"distance\": \"" << min_distance << "\",\n";

	cout << "\t\"points\": \"" << total_pts << "\"\n}\n";

}

int getTotalPoints() {

	int total = 0;

	for (unsigned int i = 0; i < min_route.size(); i++) {
		total += rockColorValue(colors[min_route[i]]);
	}

	return total;

}

//assumes colors are all lowercase
int rockColorValue (string color) {
	
	transform(color.begin(), color.end(), color.begin(), ::tolower);
	
	if (!color.compare("yellow")) {
		return 1;
	} else if (!color.compare("orange")) {
		return 2;
	} else if (!color.compare("red")) {
		return 3;
	} else if (!color.compare("blue")) {
		return 4;
	} else if (!color.compare("green")) {
		return 5;
	} else if (!color.compare("purple")) {
		return 6;
	} else if (!color.compare("alien"))  {
		return 7;
	} else  {
		return 0;
	}
}

void swap(int& a, int& b){ 
	int x=a;
  	a=b;
  	b=x;
}

void printVector(vector<int>& theVector) { 
	for (unsigned int i=0; i<theVector.size(); i++) {
    		std::cout << theVector[i];
		if (i != theVector.size() - 1) {
			cout  << ",";	
		}
	}
  	std::cout << "\n";
}

void generateAllPermutations(vector<int>& toBePermuted, unsigned int nextIndex) { 
	if (nextIndex==toBePermuted.size()) { 
		//printVector(toBePermuted);
		double distance = calculateTotalDistance(rock_nodes, nodes);
		if (min_distance >  distance) {
			min_distance = distance;
			min_route = rock_nodes;
		}
    		return;
  	}
	for (unsigned int i=nextIndex; i<toBePermuted.size(); i++) { 
		swap(toBePermuted[i], toBePermuted[nextIndex]);
    		generateAllPermutations(toBePermuted, nextIndex+1);
    		swap(toBePermuted[i], toBePermuted[nextIndex]);
  	}
}

void generateAllPermutations(vector<int>& toBePermuted) { 
	generateAllPermutations(toBePermuted, 0);
}

void fillNodes () {
	
	//line should  be Lat:Long:color
	char line[1024];
	FILE *file;
	int counter = 0;
	file = fopen("nodes.txt", "r");
	while (fgets(line, 1024, file) != NULL) {
		double lat_cord, long_cord;
		char color[100];
		if (sscanf(line, "%lf : %lf : %99s", &lat_cord, &long_cord, color) != 3) {
			cerr << "Couldn't parse line " << counter+1 << " correctly. Continuing ...\n";
			continue;
		} 
		//cout << line << "\nlat cord: " << lat_cord << "\nlong cord: " << long_cord << "\ncolor: " <<  color << "\n\n";
		double array[] = {lat_cord, long_cord};
		vector<double> vec (array, array + sizeof(array)/sizeof(double));
		
		//add to all node list
		nodes.push_back(vec);
		//add to color list
		colors.push_back(color);
		
		//if 2nd node or more then it is a rock
		if (counter >= 2) {
			rock_nodes.push_back(counter);
		}
		counter ++;
		
	}
	fclose(file);

}

double calculateTotalDistance(vector<int> rock_nodes, vector< vector<double> > nodes) {
	//calculate the distance to get back to the start only (no rock nodes)
	if (nodes.size() <= 2) {
		return cordsToDist(nodes[0], nodes[1]);
	}
	
	//At least one rock node, calculate distance from rover to first rock
	double cur_distance = cordsToDist(nodes[1], nodes[2]);

	//Calculate distances from each rock to rock and final rock to start
	for (unsigned int i=0; i < rock_nodes.size(); i++) {
    		if (i != (rock_nodes.size() - 1)) {
			cur_distance += cordsToDist(nodes[rock_nodes[i]], nodes[rock_nodes[i+1]]);
		} else {
			cur_distance += cordsToDist(nodes[rock_nodes[i]], nodes[0]);
		}
	}
	
	return cur_distance;
}


//Lat,Long cord pairs in vectors
double cordsToDist(vector<double> cord1, vector<double> cord2) {
	double lat1Rad, lat2Rad, lon1Rad, lon2Rad, u, v, distance = 0;
	lat1Rad = deg2rad(cord1.at(0));
	lon1Rad = deg2rad(cord1.at(1));
	lat2Rad = deg2rad(cord2.at(0));
	lon2Rad = deg2rad(cord2.at(1));
	u = sin((lat2Rad - lat1Rad)/2);
	v = sin((lon2Rad - lon1Rad)/2);
	distance = 2.0 * earthRadiusM * asin(sqrt(u * u + cos(lat1Rad) * cos(lat2Rad) * v * v));
	return distance;

}

double deg2rad(double deg) {
	return ((deg*M_PI)/180);
}

double rad2deg(double rad) {
	return ((rad*180)/M_PI);
}

