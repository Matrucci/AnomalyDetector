// 318570769	315902239

/**************************
 * Author: Matan Saloniko
 * ID: 31857076-9
 *************************/

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

/************************************************
 * @param a - The first point.
 * @param b - The second point.
 * @return - The distance between the 2 points.
 ***********************************************/
float distacneBetween(Point a, Point b);

/************************************
 * Building a circle from 2 points.
 * @param a - The first point.
 * @param b - The second point.
 * @return - A circle.
 **********************************/
Circle buildFromTwoPoints(Point a, Point b);

/*******************************************************
 * Checking if the point is in the circle.
 * @param c - The circle.
 * @param a - A point.
 * @return - Whether the point is in the circle or not.
 *******************************************************/
bool isPointInCircle(Circle c, Point a);

/******************************************************
 * Checking if all the points given are in the circle.
 * @param c - The circle.
 * @param p - A vector of points.
 * @return - Whether all the points are in the circle.
 ******************************************************/
bool isValidCircle(Circle c, vector<Point> p);

/***************************************************************
 * @param a
 * @param b
 * @param c
 * @param d
 * @return - The circle center from the values of the points.
 ***************************************************************/
Point getCircleCenter(float a, float b, float c, float d);

/*********************************************************
 * Building a circle from 3 points.
 * @param a - The first point.
 * @param b - The second point.
 * @param c - The third point.
 * @return - The minimum circle containing all 3 points.
 *********************************************************/
Circle getCircleFromThreePoints(Point a, Point b, Point c);

/*****************************************************
 * Building a circle according to the trivial method.
 * @param points - vector of points.
 * @return - A circle.
 *****************************************************/
Circle trivialCircle(vector<Point> points);

/*******************************************************
 * Using Welzl's recursive algorithm.
 * @param points - An array of pointers to points.
 * @param R - The points that are in the circle.
 * @param size - The size of the array.
 * @return - The minimum circle containing all points.
 ******************************************************/
Circle welzl(Point** points, vector<Point> R, int size);

/*******************************************************
 * @param points - An array of pointers to points.
 * @param size - The array size.
 * @return - The minimum circle containing all points.
 *****************************************************/
Circle findMinCircle(Point** points,size_t size);

// you may add helper functions here


#endif /* MINCIRCLE_H_ */
