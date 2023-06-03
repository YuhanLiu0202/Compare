#ifndef __PLR_H__
#define __PLR_H__
#include <iostream>
#include <cstdio>

using namespace std;

class OptimalPiecewiseLinearModel {
public:
    OptimalPiecewiseLinearModel(int epsilon_) : epsilon(epsilon_) 
    {
        res = 0;
    }

    bool addPoint(uint64_t key, int value) 
    {
        Point point;
        point.x = key;
        point.y = value;

        if (res == 0) {
            s1.x = key;
            s1.y = value;
            res++;
            return true;
        }

        if (res == 1) {
            s2.x = key;
            s2.y = value;
            slope1 = calculate_slope(s1, s2, 2.0 * epsilon);
            slope2 = calculate_slope(s1, s2, -2.0 * epsilon);
            intercept1 = calculate_intercept(slope1, s1, 1.0 * epsilon);
            intercept2 = calculate_intercept(slope2, s1, -1.0 * epsilon);
            res++;
            return true;
        }

        if (!processing(point)) {
            res = 0;
            return false;
        } else {
            if (abs(value - slope1 * (point.x - s0.x) - s0.y) > epsilon) {
                slope1 = calculate_slope(s0, point, epsilon);
                intercept1 = calculate_intercept(slope1, s0, 0);
            }

            if (abs(value - slope2 * (point.x - s0.x) - s0.y) > epsilon) {
                slope2 = calculate_slope(s0, point, -1.0 * epsilon);
                intercept2 = calculate_intercept(slope2, s0, 0);
            }
            res++;
        }

        return true;
    }

    std::pair<double, double> GetMeta() 
    {
        switch (res)
        {
        case 1:
            slope = 0;
            intercept = s1.y;
            break;
        case 2:
            slope = calculate_slope(s1, s2, 0);
            intercept = calculate_intercept(slope, s1, 0);
            break;
        default:
            slope = (slope1 + slope2) / 2.0;
            intercept = calculate_intercept(slope, s0, 0);
            break;
        }
        return make_pair(slope, intercept);
    }
private:
    struct Point {
        double x;
        double y;
    };
    const int epsilon;
    int res;
    double slope = 0, slope1 = 0, slope2 = 0, intercept = 0, intercept1 = 0, intercept2 = 0;
    Point s0, s1, s2;
    
    double calculate_slope(Point a, Point b, double delta) 
    {
        return 1.0 * (a.y + delta - b.y) / (a.x - b.x);
    }

    double calculate_intercept(double slope, Point a, double delta) 
    {
        return static_cast<double> (a.y + delta - slope * a.x);
    }

    Point calculate_intersection(double slope1, double slope2, double intercept1, double intercept2) 
    {
        Point a;
        a.x = 1.0 * (intercept1 - intercept2) / (slope2 - slope1);
        a.y = slope1 * a.x + intercept1;
        return a;
    }

    bool processing(Point a) 
    {
        double y_min = 1.0 * slope1 * a.x + intercept1 - epsilon;
        double y_max = 1.0 * slope2 * a.x + intercept2 + epsilon;
        if (a.y <= y_max && y_min <= a.y) {
            return true;
        }
        return false;
    }
};

#endif