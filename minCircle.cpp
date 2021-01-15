/**************************
 * Author: Matan Saloniko
 * ID: 31857076-9
 *************************/

#include "minCircle.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

/************************************************
 * @param a - The first point.
 * @param b - The second point.
 * @return - The distance between the 2 points.
 ***********************************************/
float distacneBetween(Point a, Point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

/************************************
 * Building a circle from 2 points.
 * @param a - The first point.
 * @param b - The second point.
 * @return - A circle.
 **********************************/
Circle buildFromTwoPoints(Point a, Point b) {
    float radius = distacneBetween(a, b) / 2;
    Point center = {(a.x + b.x) / 2, (a.y + b.y) / 2};
    return {center, radius};
}

/*******************************************************
 * Checking if the point is in the circle.
 * @param c - The circle.
 * @param a - A point.
 * @return - Whether the point is in the circle or not.
 *******************************************************/
bool isPointInCircle(Circle c, Point a) {
    return c.radius >= distacneBetween(a, c.center);
}

/******************************************************
 * Checking if all the points given are in the circle.
 * @param c - The circle.
 * @param p - A vector of points.
 * @return - Whether all the points are in the circle.
 ******************************************************/
bool isValidCircle(Circle c, vector<Point> p) {
    for (auto point: p) {
        if (!isPointInCircle(c, point)) {
            return false;
        }
    }
    return true;
}

/***************************************************************
 * @param a
 * @param b
 * @param c
 * @param d
 * @return - The circle center from the values of the points.
 ***************************************************************/
Point getCircleCenter(float a, float b, float c, float d) {
    float t = a * a + b * b;
    float s = c * c + d * d;
    float v = a * d - b * c;
    return {(d * t - b * s) / (2 * v), (a * s - c * t) / (2 * v)};
}

/*********************************************************
 * Building a circle from 3 points.
 * @param a - The first point.
 * @param b - The second point.
 * @param c - The third point.
 * @return - The minimum circle containing all 3 points.
 *********************************************************/
Circle getCircleFromThreePoints(Point a, Point b, Point c) {
    Point p = getCircleCenter(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
    p.x += a.x;
    p.y += a.y;
    return {p, distacneBetween(p, a)};
}

/*****************************************************
 * Building a circle according to the trivial method.
 * @param points - vector of points.
 * @return - A circle.
 *****************************************************/
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

/*******************************************************
 * Using Welzl's recursive algorithm.
 * @param points - An array of pointers to points.
 * @param R - The points that are in the circle.
 * @param size - The size of the array.
 * @return - The minimum circle containing all points.
 ******************************************************/
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

/*******************************************************
 * @param points - An array of pointers to points.
 * @param size - The array size.
 * @return - The minimum circle containing all points.
 *****************************************************/
Circle findMinCircle(Point** points,size_t size){
    vector<Point> temp;
    return welzl(points, temp, size);
}
