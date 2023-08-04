#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <vector>
#include <memory>

#ifdef _WIN32
#define CURVES_LIB __declspec(dllexport)
#else
#define CURVES_LIB
#endif // WIN32


class alignas(16) Point3D
{
public:
	Point3D() : x(0), y(0), z(0) {}
	Point3D(double val) : x(val), y(val), z(val) {}
	Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
	double x;
	double y;
	double z;
};

class CURVES_LIB ICurve
{
public:
	virtual Point3D getPoint(double t) = 0;
	virtual Point3D getDerivativeAt(double t) = 0;

	virtual ~ICurve() = default;

};


class CURVES_LIB Circle : public ICurve
{
public:
	Circle(double r) ;
	Point3D getPoint(double t) override;
	Point3D getDerivativeAt(double t) override;
	double getRadius() { return radius; }
private:
	double radius;
};

class CURVES_LIB Ellipse : public ICurve
{
public:
	Ellipse(double a, double b);
	Point3D getPoint(double t) override;
	Point3D getDerivativeAt(double t) override;
	
private:
	double radiusA;
	double radiusB;
};

class CURVES_LIB Helix : public ICurve
{
public:
	Helix(double r, double s);
	Point3D getPoint(double t) override;
	Point3D getDerivativeAt(double t) override;
private:
	double radius;
	double step;
};

class CURVES_LIB RandomCurveGenerator {
	unsigned int m_seed;
public:
	RandomCurveGenerator(unsigned int seed) : m_seed(seed) {}
	std::shared_ptr<ICurve> buildCircle(double r);
	std::shared_ptr<ICurve> buildEllipse(double a, double b);
	std::shared_ptr<ICurve> buildHelix(double r, double s);
	std::vector<std::shared_ptr<ICurve>> buildRandomCurves();
};



