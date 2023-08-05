
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <memory>
#include <omp.h>
#include "../CurvesApi/curves.h"



int main()
{
    Circle c(1);
    Ellipse e(1, 2);
    Helix h(1, 2);
    double t = M_PI / 4;

    std::vector<std::shared_ptr<ICurve>> curves = RandomCurveGenerator(2).buildRandomCurves();

    for (auto curve : curves)
    {
        std::cout << curve->getPoint(t).x << " " << curve->getPoint(t).y << " " << curve->getPoint(t).z << "\n";
        std::cout << curve->getDerivativeAt(t).x << " " << curve->getDerivativeAt(t).y << " " << curve->getDerivativeAt(t).z << "\n";
    }

    std::vector<std::shared_ptr<Circle>> circles;
    // reserving memory for circles
    int neededSize = 0;
    for (auto curve : curves)
    {
		auto c = std::dynamic_pointer_cast<Circle>(curve);
        if (c)
        {
			neededSize++;
		}
	}
    circles.reserve(neededSize);

    for (auto curve : curves)
    {
        auto c = std::dynamic_pointer_cast<Circle>(curve);
        if (c)
        {
            circles.push_back(c);
        }
    }

    std::sort(circles.begin(), circles.end(), [](std::shared_ptr<Circle> a, std::shared_ptr<Circle> b) { return a->getRadius() < b->getRadius(); });

    for (auto circle : circles)
    {
        std::cout << circle->getRadius() << "\n";
    }

    double totalSumOfRadii = 0;
    int T = omp_get_thread_num();
    int maxT = omp_get_max_threads();
    std::cout << T << " " << maxT << "\n";
    #pragma omp parallel for reduction(+:totalSumOfRadii)
    for (int i = 0; i < circles.size(); i++)
    {
        std::cout << "Thread " << omp_get_thread_num() << " is working on circle " << i << "\n";
        totalSumOfRadii += circles[i]->getRadius();
    }

    std::cout << totalSumOfRadii << "\n";

    return 0;
}