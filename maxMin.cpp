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

// #define N_INF -1000000000

using namespace std;
//Point coordinate struct
//typedef struct {
//	int x;
//	int y;
//}point;

// Determines the distance from a point to a line.  Note that this function works like a left test:  If
// the point is left of the line, the returned value will be positive; otherwise, it will be negative.  An
// undefined value will be returned if the point is not in the same domain as the line segment.
double PointToLineDistance(point& LineStart, point& LineEnd, point& TestVertex)
{
	// Create line direction vector
	point LineDirection = point();

	LineDirection.x = LineEnd.x - LineStart.x;
	LineDirection.y = LineEnd.y - LineStart.y;

	// Perpendicular vector must be oriented away from the vertex.
	point PerpendicularLineDirection = point();

	PerpendicularLineDirection.x = -LineDirection.y;
	PerpendicularLineDirection.y = LineDirection.x;

	// Normalize direction vectors...
	double pointLen = sqrt(pow(PerpendicularLineDirection.x, 2) + pow(PerpendicularLineDirection.y, 2));

	PerpendicularLineDirection.x = (pointLen > 0 ? PerpendicularLineDirection.x / pointLen : 0);
	PerpendicularLineDirection.y = (pointLen > 0 ? PerpendicularLineDirection.y / pointLen : 0);

	// Get distance to line, and then show perpendicular line from point.
	double distance = ((LineEnd.x - LineStart.x) * (LineStart.y - TestVertex.y) - (LineStart.x - TestVertex.x) * (LineEnd.y - LineStart.y)) /
		sqrt(pow(LineEnd.x - LineStart.x, 2) + pow(LineEnd.y - LineStart.y, 2));

	point intersectionTestPoint = point();

	intersectionTestPoint.x = TestVertex.x + distance * PerpendicularLineDirection.x;
	intersectionTestPoint.y = TestVertex.y + distance * PerpendicularLineDirection.y;

	// Check to see if the distance is valid, and return -1 if it is not.  Note that the endpoint of the normal will always appear on the same line
	// as the line segment; as a result, distance-checking is a valid comparison.
	point centerOfLine = point();

	centerOfLine.x = LineStart.x + LineDirection.x / 2.0;
	centerOfLine.y = LineStart.y + LineDirection.y / 2.0;

	double ptDist = sqrt(pow(centerOfLine.x - intersectionTestPoint.x, 2) + pow(centerOfLine.y - intersectionTestPoint.y, 2));
	double ptLen = sqrt(pow(LineDirection.x, 2) + pow(LineDirection.y, 2));

	if (ptDist > ptLen / 2.0)
	{
		if (distance < 0)
			distance = DBL_MAX;
		else
			distance = DBL_MIN;
	}
	else
		distance = -distance;

	return distance;
}

double MinPolygonDistance(vector<point>& Hull1, vector<point>& Hull2)
{
	double minDist = DBL_MAX;

	// Check to see if one polygon is inside of the other.
	vector<int> v2InsideP1 = vector<int>(Hull2.size());
	vector<int> v1InsideP2 = vector<int>(Hull1.size());

	for (int i = 0; i < Hull2.size(); i++)  // Go through vertices in P1 and determine minimum distance to other polygon's vertices.
	{
		for (int j = 0; j < Hull1.size(); j++)
		{
			// Point is actually a vector of the length of the vertex from hull 1 to that of hull 2.
			point P1VertexToP2Vertex = point();

			P1VertexToP2Vertex.x = Hull2[i].x - Hull1[j].x;
			P1VertexToP2Vertex.y = Hull2[i].y - Hull1[j].y;

			double distToPoint = sqrt(pow(P1VertexToP2Vertex.x, 2) + pow(P1VertexToP2Vertex.y, 2));

			if (abs(distToPoint) < abs(minDist))
				minDist = distToPoint;
		}
	}

	for (int i = 0; i < Hull2.size(); i++)  // Check P1.Vertex --> P2.Edge
	{
		bool validL = false, validR = false;

		for (int j = 0; j < Hull1.size(); j++)
		{
			double distToLine = PointToLineDistance(Hull2[i], Hull2[(i + 1) % Hull2.size()], Hull1[j]);

			v1InsideP2[j] += (distToLine > 0 && distToLine != DBL_MIN ? 1 : 0);

			if (!validL)
				validL = (distToLine > 0 && distToLine != DBL_MIN && distToLine != DBL_MAX);
			if (!validR)
				validR = (distToLine < 0 && distToLine != DBL_MIN && distToLine != DBL_MAX);

			// Case when one vertex is left of all other polygons' edges.
			if (v1InsideP2[j] == Hull2.size() || (validL && validR))
				return 0;

			if (abs(distToLine) < abs(minDist) && distToLine != DBL_MIN)
				minDist = abs(distToLine);
		}
	}

	for (int i = 0; i < Hull1.size(); i++)  // Check P2.Vertex --> P1.Edge
	{
		bool validL = false, validR = false;

		for (int j = 0; j < Hull2.size(); j++)
		{
			// Comparison should always return a valid number, but needs a boolean to determine whether that value is valid for the edge being checked.
			double distToLine = PointToLineDistance(Hull1[i], Hull1[(i + 1) % Hull1.size()], Hull2[j]);

			v2InsideP1[j] += (distToLine > 0 && distToLine != DBL_MIN ? 1 : 0);

			if (!validL)
				validL = (distToLine > 0 && distToLine != DBL_MIN && distToLine != DBL_MAX);
			if (!validR)
				validR = (distToLine < 0 && distToLine != DBL_MIN && distToLine != DBL_MAX);

			// Case when one vertex is left of all other polygons' edges.
			if (v2InsideP1[j] == Hull1.size() || (validL && validR))
				return 0;

			if (abs(distToLine) < abs(minDist) && distToLine != DBL_MIN)
				minDist = abs(distToLine);
		}
	}

	return minDist;
}

double MaxPolygonDistance(vector<point>& Hull1, vector<point>& Hull2, int StartingHull)
{
	double maxDist = DBL_MIN;

	// double currentMaxDist = DBL_MIN;
	double currentMaxDistOuter = DBL_MIN;

	if (StartingHull == 2)
	{
		for (int i = 0; i < Hull2.size(); i++)  // Go through vertices in P2 and determine maximum distance to other polygon's vertices (P1).
		{
			double currentMinDist = DBL_MAX;

			for (int j = 0; j < Hull1.size(); j++)
			{
				// Point is actually a vector of the length of the vertex from hull 1 to that of hull 2.
				point P1VertexToP2Vertex = point();

				P1VertexToP2Vertex.x = Hull2[i].x - Hull1[j].x;
				P1VertexToP2Vertex.y = Hull2[i].y - Hull1[j].y;

				double distToPoint = sqrt(pow(P1VertexToP2Vertex.x, 2) + pow(P1VertexToP2Vertex.y, 2));

				if (abs(distToPoint) > abs(maxDist))
				{
					// maxDist = distToPoint;

					if (abs(distToPoint) < currentMinDist)
						currentMinDist = abs(distToPoint);
				}

				// Comparison should always return a valid number, but needs a boolean to determine whether that value is valid for the edge being checked.
				double distToLine = PointToLineDistance(Hull2[i], Hull2[(i + 1) % Hull2.size()], Hull1[j]);

				if (abs(distToLine) > abs(maxDist) && distToLine != DBL_MAX)
				{
					// maxDist = abs(distToLine);

					if (abs(distToLine) < currentMinDist && distToLine < 0)
						currentMinDist = abs(distToLine);
				}
			}

			if (currentMaxDistOuter < currentMinDist && currentMinDist != DBL_MAX)
			{
				currentMaxDistOuter = currentMinDist;
			}
		}
	}

	if (StartingHull == 1)
	{
		for (int i = 0; i < Hull1.size(); i++)  // Go through vertices in P2 and determine maximum distance to other polygon's vertices (P1).
		{
			double currentMinDist = DBL_MAX;

			for (int j = 0; j < Hull2.size(); j++)
			{
				// Point is actually a vector of the length of the vertex from hull 1 to that of hull 2.
				point P1VertexToP2Vertex = point();

				P1VertexToP2Vertex.x = Hull1[i].x - Hull2[j].x;
				P1VertexToP2Vertex.y = Hull1[i].y - Hull2[j].y;

				double distToPoint = sqrt(pow(P1VertexToP2Vertex.x, 2) + pow(P1VertexToP2Vertex.y, 2));

				if (abs(distToPoint) > abs(maxDist))
				{
					// maxDist = distToPoint;

					if (abs(distToPoint) < currentMinDist)
						currentMinDist = abs(distToPoint);
				}

				// Comparison should always return a valid number, but needs a boolean to determine whether that value is valid for the edge being checked.
				double distToLine = PointToLineDistance(Hull1[i], Hull1[(i + 1) % Hull1.size()], Hull2[j]);

				if (abs(distToLine) > abs(maxDist) && distToLine != DBL_MAX)
				{
					// maxDist = abs(distToLine);

					if (abs(distToLine) < currentMinDist && distToLine < 0)
						currentMinDist = abs(distToLine);
				}
			}

			if (currentMaxDistOuter < currentMinDist && currentMinDist != DBL_MAX)
			{
				currentMaxDistOuter = currentMinDist;
			}
		}
	}

	maxDist = currentMaxDistOuter;

	return maxDist;
}

// Prints a visual representation of the completed polygon.  Note that 1's represent P1.vertices,
// 2's represent P2.vertices that are a part of the convex hull, and -'s represent empty space.
void PrintVisual(int minX, int maxX, int minY, int maxY, vector<point>& Hull1, vector<point>& Hull2)
{
	vector<point> vertices1 = vector<point>(Hull1.size());
	vector<point> vertices2 = vector<point>(Hull2.size());

	vector<string> vertexRepresentations1 = vector<string>(Hull1.size());
	vector<string> vertexRepresentations2 = vector<string>(Hull2.size());

	for (int i = 0; i < vertices1.size(); i++)
	{
		vertices1[i] = point();

		vertices1[i].x = (int)round(Hull1[i].x);
		vertices1[i].y = (int)round(Hull1[i].y);

		vertexRepresentations1[i] = "1 ";
	}

	for (int i = 0; i < vertices2.size(); i++)
	{
		vertices2[i] = point();

		vertices2[i].x = (int)round(Hull2[i].x);
		vertices2[i].y = (int)round(Hull2[i].y);

		vertexRepresentations2[i] = "2 ";
	}

	for (int i = maxY; i >= minY; i--)
	{
		for (int j = minX; j <= maxX; j++)
		{
			bool foundVertex = false;

			for (int k = 0; k < vertices1.size(); k++)
			{
				if (vertices1[k].x == j && vertices1[k].y == i)
				{
					foundVertex = true;
					cout << vertexRepresentations1[k];
					break;
				}
			}

			for (int k = 0; k < vertices2.size(); k++)
			{
				if (vertices2[k].x == j && vertices2[k].y == i)
				{
					foundVertex = true;
					cout << vertexRepresentations2[k];
					break;
				}
			}

			if (!foundVertex)
			{
				if (i == 0 || j == 0)
					cout << "+ ";
				else
					cout << "- ";
			}
		}

		cout << endl;
	}
}

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

	cout << endl << "First And Second Hull" << endl;
	PrintVisual(0, 15, 0, 15, fHull, sHull);

	cout << endl << "Minimum Distance = " << MinPolygonDistance(fHull, sHull) << endl;
	cout << "Maximum Distance H1 --> H2 = " << MaxPolygonDistance(fHull, sHull, 1) << endl;
	cout << "Maximum Distance H2 --> H1 = " << MaxPolygonDistance(fHull, sHull, 2) << endl;

	cin.get();

	return 0;
}