#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#define earthRadiusM 6371000.0

using namespace std;

//Kosmo must be first node and the rover must be second node
vector< vector<double> > nodes;
//Let black denote no value (for start and rover positions)
vector<string> colors;
//Store the min route here
vector<int> min_route;
//Min path distance
double min_distance = HUGE_VAL;
//Store the number of each rock node here (start with 2)
vector<int> rock_nodes;


void fillNodes ();
double calculateTotalDistance(vector<int> rock_nodes, vector< vector<double> > nodes);
double cordsToDist(vector<double> cord1, vector<double> cord2);
double deg2rad(double deg);
double rad2deg(double rad);
void swap(int& a, int& b);
void printVector(vector<int>& theVector);
void generateAllPermutations(vector<int>& toBePermuted, unsigned int nextIndex);
void generateAllPermutations(vector<int>& toBePermuted);


void swap(int& a, int& b){
	int x=a;
  	a=b;
  	b=x;
}

void printVector(vector<int>& theVector) {
	for (unsigned int i=0; i<theVector.size(); i++) {
    		cout << theVector[i];
		if (i != theVector.size() - 1) {
			cout  << ",";
		}
	}
  	cout << endl;
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

int main() {

	fillNodes();
	//double RIC[] = {37.5333, 77.4667};
	//double DC[] = {38.9047, 77.0164};
	//double WM[] = {37.2711, 76.7075};
	//double HS[] = {29.7604, 95.3698};
	//vector<double> RIC_cords (RIC, RIC + sizeof(RIC) / sizeof(double));
	//vector<double> DC_cords (DC, DC + sizeof(DC) / sizeof(double));
	//vector<double> WM_cords (WM,  WM + sizeof(WM) / sizeof(double));
	//vector<double> HS_cords (HS, HS + sizeof(HS) / sizeof(double));

	//nodes.push_back(RIC_cords);
	//nodes.push_back(DC_cords);
	//nodes.push_back(WM_cords);
	//nodes.push_back(HS_cords);

	//double distance = cordsToDist(DC_cords, WM_cords);
	//distance += cordsToDist(WM_cords, RIC_cords);
	////cout<< distance << " m.\n";

  	////rock_nodes.push_back(4);
  	//rock_nodes.push_back(3);
  	//rock_nodes.push_back(2);
  	////rock_nodes.push_back(1);
  	////rock_nodes.push_back(6);
  	////rock_nodes.push_back(8);
	if (nodes.size() < 2) {
		cerr << "not enough nodes\n";
		return 1;
	}
  	generateAllPermutations(rock_nodes);
	cout << "min distance: " << min_distance << "\n";
	cout << "order: ";
	printVector(rock_nodes);
}

void fillNodes () {

	//line should  be Lat:Long:color
	char line[1024];
	FILE *file;
	int counter = 0;
	file = fopen("nodes.txt", "r");
    cout << "[" << endl;
	while (fgets(line, 1024, file) != NULL) {
		double lat, lon;
		char color[100];
		if (sscanf(line, "%lf : %lf : %99s", &lat, &lon, color) != 3) {
			cerr << "Couldn't parse line " << counter + 1 << " correctly. Continuing ...\n";
			continue;
		}
		cout << "{ \"lat\": " << lat << ", \"lon\": " << lon << ", \"color\": \"" <<  color << "\" }" << endl;
		double array[] = {lat, lon};
		vector<double> vec (array, array + sizeof(array)/sizeof(double));
		nodes.push_back(vec);
		if (counter >= 2) {
			rock_nodes.push_back(counter);
		}
		counter++;
	}
    cout << "]" << endl;
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

