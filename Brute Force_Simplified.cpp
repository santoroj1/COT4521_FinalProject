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
		distance = DBL_MAX;
	else
		distance = -distance;

	return distance;
}

double MinPolygonDistance(vector<point>& Hull1, vector<point>& Hull2)
{
	double minDist = DBL_MAX;

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
		for (int j = 0; j < Hull1.size(); j++)
		{
			double distToLine = PointToLineDistance(Hull2[i], Hull2[(i + 1) % Hull2.size()], Hull1[j]);

			if (abs(distToLine) < abs(minDist))
				minDist = -distToLine;
		}
	}

	for (int i = 0; i < Hull1.size(); i++)  // Check P2.Vertex --> P1.Edge
	{
		for (int j = 0; j < Hull2.size(); j++)
		{
			// Comparison should always return a valid number, but needs a boolean to determine whether that value is valid for the edge being checked.
			double distToLine = PointToLineDistance(Hull1[i], Hull1[(i + 1) % Hull1.size()], Hull2[j]);

			if (abs(distToLine) < abs(minDist))
				minDist = -distToLine;
		}
	}

	return minDist;
}

double MaxPolygonDistance(vector<point>& Hull1, vector<point>& Hull2)
{
	double maxDist = DBL_MIN;

	for (int i = 0; i < Hull2.size(); i++)  // Go through vertices in P1 and determine maximum distance to other polygon's vertices (P2).
	{
		for (int j = 0; j < Hull1.size(); j++)
		{
			// Point is actually a vector of the length of the vertex from hull 1 to that of hull 2.
			point P1VertexToP2Vertex = point();

			P1VertexToP2Vertex.x = Hull2[i].x - Hull1[j].x;
			P1VertexToP2Vertex.y = Hull2[i].y - Hull1[j].y;

			double distToPoint = sqrt(pow(P1VertexToP2Vertex.x, 2) + pow(P1VertexToP2Vertex.y, 2));

			if (abs(distToPoint) > abs(maxDist))
				maxDist = distToPoint;
		}
	}

	return maxDist;
}