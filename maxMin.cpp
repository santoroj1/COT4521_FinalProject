/*
James Albu
John Santoro

*/
//#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cmath>
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
	//Used to make sure the file is open properly
	
	if (!myfile.is_open()) {
		cout << "File was read unsuccesfully" << endl;
	}

	while (1) { //FIXME: change conditions to something not error prone
		//Clear the sstream
		ss.str("");
		ss.clear();
		//get the current line
		getline(myfile, data);
		//Skip the first line
		if (data.find("First") != string::npos) {
			continue;
		}
		//Break loop if "Second" is read
		else if (data.find("Second") != string::npos) {
			break;
		}
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

		// Changed this line to use '<<' operator as '+' doesn't work in Visual Studio.
		cout << tmp.x << " " << tmp.y << endl;
	}
	
	myfile.close();
}

int main() 
{
	cout << "Beginning";
	vector<point> fHull;
	vector<point> sHull;
	cout << "After initialization";
	fileInput(fHull, sHull);

	ConvexHull firstHull = ConvexHull(fHull, false);
	ConvexHull secondHull = ConvexHull(sHull, false);

	cout << "First Hull" << endl;
	fHull = firstHull.PrintTraversal(firstHull.StartPoint());
	firstHull.PrintVisual(0, 15, 0, 15);
	cout << endl << "Second Hull" << endl;
	sHull = secondHull.PrintTraversal(secondHull.StartPoint());
	secondHull.PrintVisual(0, 15, 0, 15);

	return 0;
}