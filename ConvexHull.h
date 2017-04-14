//#pragma once

#include "Vertex2D.h"
#include <string>
#include <vector>

using namespace std;

// Computes a convex hull from a set of points.
class ConvexHull
{
private:
	vector<Vertex2D*> vertex2D;

	// Used to store current traversal vertex + prev and next vertices. [0] = prev, [1] = cur, [2] = next
	vector<Vertex2D*> traversalPoint;
	int traversalIndex;

	// Contains the number of vertices currently present in the polygon
	int polygonVertexCount;

	// Contains the number of tangent points.  At the end of each traversal, 
	// if tanPointCount == 0 then the vertex is invalid.  Otherwise, if tanPointCount == 2
	// then the polygon will be expanded.
	int tanPointCount;

	// Tangent vertices
	vector<Vertex2D*> tangentPoint;

	// Root/Starting point
	Vertex2D* startPoint;

	// When counter == vertex2D.size(), the entire polygon has been processed.
	int counter;

	// Sorts all of the vertices by their x-coordinates.
	void SortVertices()
	{
		for (int i = 0; i < vertex2D.size(); i++)
		{
			for (int j = i + 1; j < vertex2D.size(); j++)
			{
				if (vertex2D[i]->position.x > vertex2D[j]->position.x)
				{
					Vertex2D* temp = vertex2D[i];
					vertex2D[i] = vertex2D[j];
					vertex2D[j] = temp;
				}
			}
		}
	}

	// Prints all vertices, regardless of whether they are a part of the resulting polygon or not
	void PrintVertices()
	{

		for (int i = 0; i < vertex2D.size(); i++)
			cout << "V[" << i << "] = (" << vertex2D[i]->position.x << ", " << vertex2D[i]->position.y << ")" << endl;
	}

	// Determines whether a test point is left of the start and destination or not.
	bool Left(point& Start, point& Destination, point& Test)
	{
		double Val1 = atan2(Destination.y - Start.y, Destination.x - Start.x);
		double Val2 = atan2(Test.y - Start.y, Test.x - Start.x);

		// If sin(val) > 0 then the angle is less than 180 degrees and thus, left.
		return sin(Val2 - Val1) > 0;
	}

	// Gets rid of nonessential edges.
	void RemoveExtraLines(Vertex2D* TraversalStart, Vertex2D* TraversalEnd)
	{
		Vertex2D* traversalCurrent = TraversalStart;
		Vertex2D* traversalPrev = NULL;

		// Find left line and traverse from there.  Note that the old edge will be the one that is removed; hence, it is index 0.
		while (traversalCurrent != TraversalEnd)
		{
			traversalPrev = traversalCurrent;
			traversalCurrent = traversalCurrent->edgeList[0];

			traversalPrev->TryRemoveEdge(traversalCurrent);

			if (traversalCurrent != TraversalEnd)
			{
				// traversalCurrent.col = Color.Gray;
				polygonVertexCount--;
			}
			else
				break;
		}
	}

	// Builds the convex hull from the initial triangle and then returns a starting vertex which can be used to traverse it.
	Vertex2D* BuildHull(bool Debug)
	{
		// Find a new vertex to add to the polygon.
		while (counter < vertex2D.size())
		{
			// vertex2D[counter[0]].col = Color.Lime;
			// vertexStatus[counter[0]] = 1;

			// Traversing the polygon
			if (traversalIndex < polygonVertexCount)
			{
				// Check to see if vertex is a tangent point:
				// int iMinus1 = (counter[1] - 1 >= 0 ? counter[1] - 1 : counter[1] - 1 + counter[0]);
				// int iPlus1 = (counter[1] + 1 < counter[0] ? counter[1] + 1 : counter[1] + 1 - counter[0]);

				bool s1 = Left(traversalPoint[0]->position, traversalPoint[1]->position, vertex2D[counter]->position);
				bool s2 = Left(traversalPoint[1]->position, traversalPoint[2]->position, vertex2D[counter]->position);

				// traversalPoint[1].col = Color.Yellow;

				if ((s1 && !s2) || (!s1 && s2))
				{
					// tangentPoint[tanPointCount++] = traversalPoint[1];
					// traversalPoint[1].col = Color.Lime;
					// vertex2D[counter[1]].isTanPoint = true;

					// Orient new edge correctly.
					if (!s1 && s2)
					{
						vertex2D[counter]->TryAddEdge(traversalPoint[1]);
						tangentPoint[1] = traversalPoint[1];
						tanPointCount++;
					}
					else
					{
						traversalPoint[1]->TryAddEdge(vertex2D[counter]);
						tangentPoint[0] = traversalPoint[1];
						tanPointCount++;
					}
				}

				// Traverse CCW to next vertex
				traversalIndex++;

				traversalPoint[0] = traversalPoint[1];
				traversalPoint[1] = traversalPoint[2];
				traversalPoint[2] = traversalPoint[2]->edgeList[0];

				// Must eliminate all extra lines.
				if (traversalIndex == polygonVertexCount || tanPointCount == 2)
				{
					if (tanPointCount == 2)  // Two tan lines successfully found.  Thus, the new vertex is not in the polygon.
					{
						// Clear current traversal
						traversalPoint[0] = NULL;
						traversalPoint[1] = NULL;
						traversalPoint[2] = NULL;

						RemoveExtraLines(tangentPoint[0], tangentPoint[1]);

						traversalPoint[0] = tangentPoint[0];
						traversalPoint[1] = traversalPoint[0]->edgeList[0];
						traversalPoint[2] = traversalPoint[1]->edgeList[0];

						traversalIndex = 0;
						counter++;
						polygonVertexCount++;
						tanPointCount = 0;

						// Show results of the polygon if debugging.
						if (Debug)
							cout << "Vertex added." << endl;

					}
					else
					{
						// Show results of the polygon if debugging.
						if (Debug)
							cout << "Vertex not added because it is inside the current hull." << endl;

						counter++;
						traversalIndex = 0;
					}

					// tanPointCount = 0;
					if (Debug)
					{
						cout << "Currently, polygon has " << polygonVertexCount << " vertices." << endl;
						PrintTraversal(traversalPoint[0]);
						cout << endl;
					}
				}
			}
		}

		return traversalPoint[0];
	}

public:
	// There must be at least 3 vertices being inputted into the program.  Otherwise, it will not work.
	ConvexHull(vector<point>& Vertices, bool Debug) 
	{
		// Initialize vertices
		vertex2D = vector<Vertex2D*>(Vertices.size());

		for (int i = 0; i < Vertices.size(); i++)
		{
			vertex2D[i] = new Vertex2D(Vertices[i]);
		}

		// Print initial vertices
		if (Debug)
		{
			cout << "Vertices Before Sorting" << endl;
			PrintVertices();
			cout << endl;
		}

		// Sort vertices
		SortVertices();

		// Print after sorting.  Note that vertices are now in order based on their x-coordinates.
		if (Debug)
		{
			cout << "Vertices after sorting" << endl;
			PrintVertices();
			cout << endl;
		}

		// Try initializing 1st 3 vertices.
		vertex2D[0]->TryAddEdge(vertex2D[1]);
		vertex2D[1]->TryAddEdge(vertex2D[2]);
		vertex2D[2]->TryAddEdge(vertex2D[0]);

		// If debugging, print the traversal.
		if (Debug)
		{
			cout << "Initial triangle:  " << endl;
			PrintTraversal(vertex2D[0]);
			cout << endl;
		}

		// Check to see if the triangle is CCW.  If not, reverse order of vertices.
		if (!Left(vertex2D[0]->position, vertex2D[0]->edgeList[0]->position, vertex2D[2]->position))
		{
			vertex2D[0]->RemoveAllEdges();
			vertex2D[1]->RemoveAllEdges();
			vertex2D[2]->RemoveAllEdges();

			Vertex2D* temp = vertex2D[1];

			vertex2D[1] = vertex2D[0];
			// vertex2D[1].UpdateLabel("V1");

			vertex2D[0] = temp;
			// vertex2D[0].UpdateLabel("V0");

			vertex2D[0]->TryAddEdge(vertex2D[1]);
			vertex2D[1]->TryAddEdge(vertex2D[2]);
			vertex2D[2]->TryAddEdge(vertex2D[0]);

			if (Debug)
			{
				cout << "...But the triangle was CW, not CCW.  In order to fix this, we simply reverse the order of the vertices:  " << endl;
				PrintTraversal(vertex2D[0]);
				cout << endl;
			}
		}

		// Initialize all other values that will be necessary for the creation of the convex hull.
		traversalPoint = vector<Vertex2D*>(3);
		traversalPoint[0] = vertex2D[2];
		traversalPoint[1] = vertex2D[0];
		traversalPoint[2] = vertex2D[1];

		traversalIndex = 0;
		polygonVertexCount = 3;

		tanPointCount = 0;
		tangentPoint = vector<Vertex2D*>(2);

		counter = 3;

		// Now that the initial step is complete, we can build the hull that surrounds all of the vertices:
		Vertex2D* hullStart = BuildHull(Debug);
		startPoint = hullStart;
		
		// Show resulting polygon.
		if (Debug)
		{
			cout << "Polygon complete.  Original vertices:" << endl;
			PrintVertices();
			cout << endl;
			cout << "Resulting convex hull:" << endl;
			PrintTraversal(hullStart);
			cout << endl;
			cout << "Visual of Traversal (C = Point on convex hull, P = original vertex):" << endl;
			PrintVisual(-10, 10, -10, 10);
		}

		int blub = 1;
	}

	~ConvexHull() {}

	// Prints a visual representation of the completed polygon.  Note that P's represent vertices,
	// C's represent vertices that are a part of the convex hull, and -'s represent empty space.
	void PrintVisual(int minX, int maxX, int minY, int maxY)
	{
		vector<point> vertices = vector<point>(vertex2D.size());
		vector<string> vertexRepresentations = vector<string>(vertex2D.size());

		for (int i = 0; i < vertex2D.size(); i++)
		{
			vertices[i] = point();

			vertices[i].x = (int)round(vertex2D[i]->position.x);
			vertices[i].y = (int)round(vertex2D[i]->position.y);

			vertexRepresentations[i] = (vertex2D[i]->edgeList.size() > 0 ? "C " : "P ");
		}

		for (int i = maxY; i >= minY; i--)
		{
			for (int j = minX; j <= maxX; j++)
			{
				bool foundVertex = false;

				for (int k = 0; k < vertices.size(); k++)
				{
					if (vertices[k].x == j && vertices[k].y == i)
					{
						foundVertex = true;
						cout << vertexRepresentations[k];
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

	Vertex2D* StartPoint() { return startPoint; };

	// Prints a traversal of all the vertices along the boundary of the polygon.  Note that because
	// the traversal takes only one path, a simple while-loop is used.
	vector<point> PrintTraversal(Vertex2D* Start)
	{
		vector<point> hull;
		point tmp;
		if (Start->edgeList.size() > 0)
		{
			Vertex2D* destination = Start->edgeList[0];  // The correct 'Next' vertex will always be element 0 of the edge list.

			//cout << "Start:  (" << Start->position.x << ", " << Start->position.y << ") --> " << endl;
			tmp.x = Start->position.x;
			tmp.y = Start->position.y;
			hull.push_back(tmp);

			while (destination->position.x != Start->position.x || destination->position.y != Start->position.y)
			{
				//cout << "(" << destination->position.x << ", " << destination->position.y << ") --> " << endl;
				tmp.x = destination->position.x;
				tmp.y = destination->position.y;
				hull.push_back(tmp);

				destination = destination->edgeList[0];
			}

			//cout << "End" << endl;

		}
		return hull;
	}
};