/*
James Albu
John Santoro

*/
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "ConvexHull.h"

using namespace std;
//Point coordinate struct
//typedef struct {
//	int x;
//	int y;
//}point;

void fileInput(vector<point> &fHull, vector<point> &sHull) {

	ifstream myfile("input.txt");
	string data;
	point tmp;
	//dummy character for ',' in file
	char cma = ',';
	//String stream to hold and transfer values to vector
	stringstream ss;

	while (data != "Second") { //FIXME: change conditions to something not error prone
		//Clear the sstream
		ss.str("");
		ss.clear();
		//get the current line
		getline(myfile, data);

		//Skip the first line
		if (data == "First")
			continue;
		//Break loop if "Second" is read
		else if (data == "Second")
			break;
		else {
			ss << data;
			ss >> tmp.x >> cma >> tmp.y;
			fHull.push_back(tmp);
			
		}
	}
	while (!myfile.eof()) {
		ss.str("");
		ss.clear();

		getline(myfile, data);
		ss << data;
		ss >> tmp.x >> cma >> tmp.y;

		sHull.push_back(tmp);
	}
	/*
	for (int i = 0; i < fHull.size(); ++i)
		cout << fHull[i].x << "," << fHull[i].y << endl;
	*/
	myfile.close();
}

int main() 
{

	vector<point> fHull;
	vector<point> sHull;

	fileInput(fHull, sHull);

	ConvexHull firstHull = ConvexHull(fHull, false);
	ConvexHull secondHull = ConvexHull(sHull, false);

	cout << "First Hull" << endl;
	firstHull.PrintTraversal(firstHull.StartPoint());
	firstHull.PrintVisual(0, 15, 0, 15);
	cout << endl << "Second Hull" << endl;
	secondHull.PrintTraversal(secondHull.StartPoint());
	secondHull.PrintVisual(0, 15, 0, 15);

	cin.get();

	return 0;
}