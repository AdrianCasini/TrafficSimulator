#ifndef _TRIANGULATE_H
#define _TRIANGULATE_H


#include "Simulator.h"

using namespace std;

class Triangulate
{
	public:

		// triangulate a contour/polygon, places results in STL vector
		// as series of triangles.
		static bool Process(const Vector2dVector &contour,Vector2dVector &result);

		// compute area of a contour/polygon
		static double Area(const Vector2dVector &contour);

		// decide if point Px/Py is inside triangle defined by
		// (Ax,Ay) (Bx,By) (Cx,Cy)
		static bool InsideTriangle(double Ax, double Ay,
			double Bx, double By,
			double Cx, double Cy,
			double Px, double Py);


	private:
		static bool Snip(const Vector2dVector &contour, int u, int v, int w, int n, int *V);

};


#endif

