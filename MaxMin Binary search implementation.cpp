	//Use dot product to find up, down, dr, above, below
	/*
	#define up(u,v)						(dot(u,v) > 0)
	#define down(u,v)                   (dot(u,v) < 0)
	#define dr(u,Vi,Vj)                 (dot(u, (Vi)-(Vj)))		 	// direction sign of (Vi-Vj)
	#define above(u,Vi,Vj)              (dr(u,Vi,Vj) > 0)		 	// true if Vi is above Vj
	#define below(u,Vi,Vj)				(dr(u,Vi,Vj) < 0) 			//true if Vi is below Vj
	*/

double MaxBinaryPolygonDistance(vector<point>& hull1, vector<point>& hull2){
	//Declare index variables - a,b,c is for first hull
	a = 0;
	b = hull1.size();
	//Midpoint
	c = hull1.size()/2;

	//Declare index variables - x,y,z is for second hull
	x = 0;
	y = hull2.size();
	//Midpoint
	z = hull2.size()/2;

	//Vector A,C
}