#include <vector>

//Use dot product to find up, down, dr, above, below
#define dot(u,v)					((u.x * v.x) + (u.y * v.y))
#define up(u,v)						(dot(u,v) > 0)
#define down(u,v)                   (dot(u,v) < 0)
#define dr(u,Vi,Vj)                 (dot(u, (Vi)-(Vj)))		 	// direction sign of (Vi-Vj)
#define above(u,Vi,Vj)              (dr(u,Vi,Vj) > 0)		 	// true if Vi is above Vj
#define below(u,Vi,Vj)				(dr(u,Vi,Vj) < 0) 			//true if Vi is below Vj


double MaxPointBinary(vector U,vector<point>& hull1){
		//Find max vertex first

	//Declare index variables - a,b,c is for first hull
	int a, b, c;
	Vector A,C; 	//used to hold the vertices of the convex polygon
	int upA,upC; 	
	int n = hull1.size();
	//Declare U, the direction vector, may have to get from aruguments
	//Vector U;
	a = 0;
	b = n;
	
	A = hull1[1] - hull[0];
	upA = up(U,A);
	//Test if V[0] is a local maximum
	if (!upA && !above(U,hull1[n-1], hull1[0]))
		return 0;

	for(;;) {
		c = (a + b) /2;
		C = hull1[c+1] - hull1[c];
		upC = up(U,C);
		if (!upC && !above(U,hull1[c-1], hull1[c]))
			return c;
		if(upA) {
			if (upC) {
				b = c;
			}

			else {
				if  (above(U,hull1[a],hull1[c])) {
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
				if (below(U,hull1[a],hull1[c])) {
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
//not sure if needed, could use above function and reverse U
/*
double minPointBinary(vector U, vector<point> hull) {
		//Find min vertex first - couls flip the vector so that it points 
		//toward the other one

	//Declare index variables - a,b,c is for first hull
	int a, b, c;
	Vector A,C; 	//used to hold the vertices of the convex polygon
	int upA,upC; 	
	int n = hull1.size();
	//Declare U, the direction vector, may have to get from aruguments
	//Reverse U so that it points in the other direction
	//Vector U;
	a = 0;
	b = n;
	
	//Declare index variables - x,y,z is for second hull
	//x = 0;
	//y = hull2.size();
	//Midpoint
	//z = hull2.size()/2;
	
	A = hull1[1] - hull[0];
	upA = up(U,A);
	//Test if V[0] is a local maximum
	if (!upA && !above(U,hull1[n-1], hull1[0]))
		return 0;

	for(;;) {
		c = (a + b) /2;
		C = hull1[c+1] - hull1[c];
		upC = up(U,C);
		if (!upC && !above(U,hull1[c-1], hull1[c]))
			return c;
		if(upA) {
			if (upC) {
				b = c;
			}

			else {
				if  (above(U,hull1[a],hull1[c])) {
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
				if (below(U,hull1[a],hull1[c])) {
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
}*/
double MinDistanceBinary(vector<point>& hull1,vector<point>& hull2) {
	vector U,N;
	int max1; 			//index of maximum point of hull1
	int min2; 			//index of minimum point of hull2

	//Find the vector that somehow

	max1 = MaxPointBinary(U,hull1);
	
	//Reverse direction vector ????
	min2 = MaxPointBinary(U,hull2);

	return sqrt(pow(hull2[min2].x-hull1[max1].x,2) + pow(hull2[min2].y-hull1[max1].y,2));

}