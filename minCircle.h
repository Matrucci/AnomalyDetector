// 318570769	315902239

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ------------ DO NOT CHANGE -----------
class Point{
public:
    float x,y;
    Point(float x,float y):x(x),y(y){}
};

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

float distacneBetween(Point a, Point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

Circle buildFromTwoPoints(Point a, Point b) {
    float radius = distacneBetween(a, b) / 2;
    Point center = {(a.x + b.x) / 2, (a.y + b.y) / 2};
    return {center, radius};
}

bool isPointInCircle(Circle c, Point a) {
    return c.radius >= distacneBetween(a, c.center);
}

bool isValidCircle(Circle c, vector<Point> p) {
    for (auto point: p) {
        if (!isPointInCircle(c, point)) {
            return false;
        }
    }
    return true;
}

Point getCircleCenter(float a, float b, float c, float d) {
    float t = a * a + b * b;
    float s = c * c + d * d;
    float v = a * d - b * c;
    return {(d * t - b * s) / (2 * v), (a * s - c * t) / (2 * v)};
}

Circle getCircleFromThreePoints(Point a, Point b, Point c) {
    Point p = getCircleCenter(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
    p.x += a.x;
    p.y += a.y;
    return {p, distacneBetween(p, a)};
}

Circle trivialCircle(vector<Point> points) {
    if (points.size() == 0) {
        return {{0, 0}, 0};
    }
    else if (points.size() == 1) {
        return {points[0], 0};
    }
    else if(points.size() == 2) {
        return buildFromTwoPoints(points[0], points[1]);
    }
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle temp = buildFromTwoPoints(points[i], points[j]);
            if (isValidCircle(temp, points)) {
                return temp;
            }
        }
    }
    return getCircleFromThreePoints(points[0], points[1], points[2]);
}

Circle welzl(Point** points, vector<Point> R, int size) {
    if (size == 0 || R.size() == 3) {
        return trivialCircle(R);
    }
    Point p = *points[size - 1];
    Circle temp = welzl(points, R, size - 1);
    if (isPointInCircle(temp, p)) {
        return temp;
    }

    R.push_back(p);
    return welzl(points, R, size - 1);
}

// implement
Circle findMinCircle(Point** points,size_t size){
    vector<Point> temp;
    return welzl(points, temp, size);
}

// you may add helper functions here


#endif /* MINCIRCLE_H_ */
