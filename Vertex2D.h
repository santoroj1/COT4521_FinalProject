#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <vector>

using namespace std;

// CHANGED THIS STRUCTURE
typedef struct {
	double x;
	double y;
}point;

class Vertex2D
{
	friend class ConvexHull;

private:
	point position;
	vector<Vertex2D*> edgeList;

public:
	Vertex2D() {}

	Vertex2D(point& Position)
	{
		position = Position;

		edgeList = vector<Vertex2D*>();
	}

	// Tries to add an edge that points to a destination vertex.
	void TryAddEdge(Vertex2D* Destination)
	{
		// Basically, if the destination is not the source, allow addition of new edge.
		if (Destination->position.x != position.x || Destination->position.y != position.y)
		{
			bool edgeAlreadyExists = false;

			for (int i = 0; i < edgeList.size(); i++)
			{
				edgeAlreadyExists = (edgeList[i]->position.x == Destination->position.x && edgeList[i]->position.y == Destination->position.y);
			}

			if (!edgeAlreadyExists)
			{
				edgeList.push_back(Destination);
				// edgeDisplays.Add(new Line(position, Destination.position, LineTex, LineColor));

				// Do not make bidirectional.

				// Destination.edgeList.Add(this);
			}
		}
	}

	// Tries to remove an edge that points to a destination vertex.
	void TryRemoveEdge(Vertex2D* Destination)
	{
		for (int i = 0; i < edgeList.size(); i++)
		{
			// If positions are equal...
			if (Destination->position.x == edgeList[i]->position.x && Destination->position.y == edgeList[i]->position.y)
			{
				/*for (int j = 0; j < edgeDisplays.Count; j++)
				{
					if (edgeDisplays[j].v1 == position && edgeDisplays[j].v2 == Destination.position)
					{
						edgeDisplays.RemoveAt(j);
						break;
					}
				}*/

				edgeList.erase(edgeList.begin() + i);
			}
		}
	}

	// Tries to remove all edges connected to this vertex.
	void RemoveAllEdges()
	{
		for (int i = edgeList.size() - 1; i >= 0; i--)
		{
			TryRemoveEdge(edgeList[i]);
		}
	}

	/*******************************************************************
	*	NEW PUBLIC CODE ADDED HERE!  O(1)
	*******************************************************************/

	vector<Vertex2D*> EdgeList() { return edgeList; };
	point Position() { return position; };

	/*******************************************************************
	*	END NEW PUBLIC CODE
	*******************************************************************/

	~Vertex2D() {}
};

#endif // !VERTEX_H