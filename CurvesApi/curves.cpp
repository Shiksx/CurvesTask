
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <memory>
#include "curves.h"
using namespace std;

/*
Design a small program in C++ that would implement support of 3D curves hierarchy.
1. Support a few types of 3D geometric curves – circles, ellipses and 3D helixes. (Simplified
definitions are below). Each curve should be able to return a 3D point and a first derivative (3D
vector) per parameter t along the curve.
2. Populate a container (e.g. vector or list) of objects of these types created in random manner with
random parameters.
3. Print coordinates of points and derivatives of all curves in the container at t=PI/4.
4. Populate a second container that would contain only circles from the first container. Make sure the
second container shares (i.e. not clones) circles of the first one, e.g. via pointers.
5. Sort the second container in the ascending order of circles’ radii. That is, the first element has the
smallest radius, the last - the greatest.
6. Compute the total sum of radii of all curves in the second container.
(*) Additional optional requirements:
7. Split implementation into a library of curves (.dll or .so) and executable which uses API of this
library.
8. Implement computation of the total sum of radii using parallel computations (e.g. OpenMP or Intel
TBB library).
Requirements to the implementation:
1. The implementation must use virtual methods.
2. Has neither explicit memory deallocation nor memory leaks.
3. Curves must be physically correct (e.g. radii must be positive).
4. Containers and sorting must be implemented using STL (C++ Standard Template Library).
5. The implementation may use constructs of C++11 or higher.
6. The code must compile with any compiler of your choice (gcc, Visual C++, etc).
Curve definitions:
- All curves are parametrically defined, i.e. a point is calculated using some C(t) formula.
- Circle is planar in the plane XoY (i.e. all Z-coordinates are 0) and is defined by its radius.
- Ellipse is planar in the plane XoY and is defined by its two radii, along X and Y axes.
- Helix is spatial and is defined by its radius and step (see the figure below). It takes 2 * PI in
parametric space to make a round, i.e. any point on helix satisfies the condition C(t + 2*PI) = C(t) +
{0, 0, step}
*/

// Needed Curves
// Circle is (r * cos(t), r * sin(t), 0)
// Ellipse is (a * cos(t), b * sin(t), 0)
// Helix is (r * cos(t), r * sin(t), step * t / (2 * PI))


Circle::Circle(double r) : radius(r) {
	if (r < 0)
	{
		throw invalid_argument("Radius must be positive");
	}
}
Point3D Circle::getPoint(double t) 
{
	return Point3D(radius * cos(t), radius * sin(t), 0);
}
Point3D Circle::getDerivativeAt(double t) 
{
	return Point3D(-radius * sin(t), radius * cos(t), 0);
}



Ellipse::Ellipse(double a, double b) : radiusA(a), radiusB(b) {
	if (a < 0 || b < 0)
	{
		throw invalid_argument("Radiuses must be positive");
	}
}
Point3D Ellipse::getPoint(double t) 
{
	return Point3D(radiusA * cos(t), radiusB * sin(t), 0);
}
Point3D Ellipse::getDerivativeAt(double t) 
{
	return Point3D(-radiusA * sin(t), radiusB * cos(t), 0);
}

Helix::Helix(double r, double s) : radius(r), step(s) {
	if (r < 0)
	{
		throw invalid_argument("Radius must be positive");
	}
}
Point3D Helix::getPoint(double t) 
{
	return Point3D(radius * cos(t), radius * sin(t), step * t / (2 * M_PI));
}
Point3D Helix::getDerivativeAt(double t) 
{
	return Point3D(-radius * sin(t), radius * cos(t), step / (2 * M_PI));
}

std::shared_ptr<ICurve> RandomCurveGenerator::buildCircle(double r) { return std::make_shared<Circle>(r); }

std::shared_ptr<ICurve> RandomCurveGenerator::buildEllipse(double a, double b) { return std::make_shared<Ellipse>(a, b); }

std::shared_ptr<ICurve> RandomCurveGenerator::buildHelix(double r, double s) { return std::make_shared<Helix>(r, s); }

vector<shared_ptr<ICurve>> RandomCurveGenerator::buildRandomCurves()
{
	vector<shared_ptr<ICurve>> curves;

	mt19937 rng(m_seed);
	uniform_real_distribution<double> dist(0, 100);
	uniform_int_distribution<int> distType(0, 2);
	uniform_int_distribution<unsigned int> countDist(0, 10000);
	unsigned int count = countDist(rng);

	curves.reserve(count);
	for (unsigned int i = 0; i < count; i++)
	{
		int type = distType(rng);
		switch (type)
		{
		case 0:
		{
			double r = dist(rng);
			curves.push_back(buildCircle(r));
			break;
		}
		case 1:
		{
			double a = dist(rng);
			double b = dist(rng);
			curves.push_back(buildEllipse(a, b));
			break;
		}
		case 2:
		{
			double r = dist(rng);
			double s = dist(rng);
			curves.push_back(buildHelix(r, s));
			break;
		}
		}
	}
	return curves;
}
