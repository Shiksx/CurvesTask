#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <memory>
#include "curves.h"


Circle::Circle(double r) : radius(r) {
	if (r < 0)
	{
		throw std::invalid_argument("Radius must be positive");
	}
}
Point3D Circle::getPoint(double t)
{
	return Point3D(radius * std::cos(t), radius * std::sin(t), 0);
}
Point3D Circle::getDerivativeAt(double t)
{
	return Point3D(-radius * std::sin(t), radius * std::cos(t), 0);
}



Ellipse::Ellipse(double a, double b) : radiusA(a), radiusB(b) {
	if (a < 0 || b < 0)
	{
		throw std::invalid_argument("Radiuses must be positive");
	}
}
Point3D Ellipse::getPoint(double t)
{
	return Point3D(radiusA * std::cos(t), radiusB * std::sin(t), 0);
}
Point3D Ellipse::getDerivativeAt(double t)
{
	return Point3D(-radiusA * std::sin(t), radiusB * std::cos(t), 0);
}

Helix::Helix(double r, double s) : radius(r), step(s) {
	if (r < 0)
	{
		throw std::invalid_argument("Radius must be positive");
	}
}
Point3D Helix::getPoint(double t)
{
	return Point3D(radius * std::cos(t), radius * std::sin(t), step * t / (2 * M_PI));
}
Point3D Helix::getDerivativeAt(double t)
{
	return Point3D(-radius * std::sin(t), radius * std::cos(t), step / (2 * M_PI));
}

std::shared_ptr<ICurve> RandomCurveGenerator::buildCircle(double r) { return std::make_shared<Circle>(r); }

std::shared_ptr<ICurve> RandomCurveGenerator::buildEllipse(double a, double b) { return std::make_shared<Ellipse>(a, b); }

std::shared_ptr<ICurve> RandomCurveGenerator::buildHelix(double r, double s) { return std::make_shared<Helix>(r, s); }

std::vector<std::shared_ptr<ICurve>> RandomCurveGenerator::buildRandomCurves()
{
	std::vector<std::shared_ptr<ICurve>> curves;

	std::mt19937 rng(m_seed);
	std::uniform_real_distribution<double> dist(0, 100);
	std::uniform_int_distribution<int> distType(0, 2);
	std::uniform_int_distribution<unsigned int> countDist(0, 10000);
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