// 318570769	315902239

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------
class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

float distacneBetween(Point a, Point b);

Circle buildFromTwoPoints(Point a, Point b);

bool isPointInCircle(Circle c, Point a);

bool isValidCircle(Circle c, vector<Point> p);

Point getCircleCenter(float a, float b, float c, float d);

Circle getCircleFromThreePoints(Point a, Point b, Point c);

Circle trivialCircle(vector<Point> points);

Circle welzl(Point** points, vector<Point> R, int size);

// implement
Circle findMinCircle(Point** points,size_t size);

// you may add helper functions here


#endif /* MINCIRCLE_H_ */
