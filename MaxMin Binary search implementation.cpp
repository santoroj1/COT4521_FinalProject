#include <vector>

//Use dot product to find up, down, dr, above, below
#define dot(u,v)					((u.x * v.x) + (u.y * v.y))
#define up(u,v)						(dot(u,v) > 0)
#define down(u,v)                   (dot(u,v) < 0)
#define dr(u,Vi,Vj)                 (dot(u, (Vi)-(Vj)))		 	// direction sign of (Vi-Vj)
#define above(u,Vi,Vj)              (dr(u,Vi,Vj) > 0)		 	// true if Vi is above Vj
#define below(u,Vi,Vj)				(dr(u,Vi,Vj) < 0) 			//true if Vi is below Vj


double MaxBinaryPolygonDistance(vector<point>& hull1, vector<point>& hull2){
	//Declare index variables - a,b,c is for first hull
	int a, b, c;
	Vector A,C;
	int upA,upC; 	
	//Declare U, the direction vector, may have to get from aruguments
	Vector U;
	a = 0;
	b = hull1.size();
	/*
	//Declare index variables - x,y,z is for second hull
	x = 0;
	y = hull2.size();
	//Midpoint
	z = hull2.size()/2;
	*/
	A = hull1[1] - hull[0];
	upA = up(U,A);
	//Test if V[0] is a local maximum
	if (!upA && !above(U,V[n-1], V[0]))
		return 0;

	for(;;) {
		c = (a + b) /2;
		C = V[c+1] - V[c];
		upC = up(U,C);
		if (!upC && !above(U,V[c-1], V[c]))
			return c;
		if(upA) {
			if (upC) {
				b = c;
			}

			else {
				if  (above(U,V[a],V[c])) {
					b = c;
				}
				else {
					a = c;
					A = C;
					upA = upC;
				}
			}
		}
		else {
			if (upC) {
				a = c;
				A = c;
				upA = upC;
			}
			else {
				if (below(U,V[a],V[c])) {
					b = c;
				}
				else {
					a = c;
					A = C;
					upA = upC;
				}
			}
		}
	}
}