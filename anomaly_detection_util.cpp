/**************************
 * Author: Matan Saloniko
 * ID: 31857076-9
 *************************/

#include "anomaly_detection_util.h"
#include <math.h>
#include <iostream>

using namespace std;

float avg(float* x, int size) {
    float avg = 0;
    for (int i = 0; i < size; i++) {
        avg += x[i];
    }
    avg = avg / size;
    return avg;
}

//returns the variance of X and Y
float var(float* x, int size) {
    float sumSqr = 0, miu = 0;
    for (int i = 0; i < size; i++) {
        miu += x[i];
        sumSqr += x[i] * x[i];
    }
    sumSqr = sumSqr / size;
    miu = miu / size;
    miu = miu * miu;
    return sumSqr - miu;
}

//returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float xyAvg = 0, xAvg = 0, yAvg = 0;
    for (int i = 0; i < size; i++) {
        xyAvg += x[i] * y[i];
        xAvg += x[i];
        yAvg += y[i];
    }
    xyAvg = xyAvg / size;
    xAvg = xAvg / size;
    yAvg = yAvg / size;
    return xyAvg - (xAvg * yAvg);
}

//returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    return (cov(x, y, size) / sqrt(var(x, size) * var(y, size)));
}

//performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    float *x = new float[size];
    float *y = new float[size];
    float a, b;
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    a = cov(x, y, size) / var(x, size);
    b = avg(y, size) - a * avg(x, size);
    return Line(a, b);
}

//returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
    Line l = linear_reg(points, size);
    return dev(p, l);
}

//returns the deviation between point p and the line
float dev(Point p, Line l) {
    float expectedValue = l.f(p.x);
    float dis = expectedValue - p.y;
    if (dis < 0) {
        return -1 * dis;
    }
    return dis;
}

