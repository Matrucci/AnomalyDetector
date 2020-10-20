//
// Created by idang on 20/10/2020.
//

#include "anomaly_detection_util.h"

float var(float* x, int size) {
    float divert = 1 / size;
    float xPow = 0, xAdd =0;
    for(int i = 0; i < size; i++) {
        xAdd = xAdd + x[i];
        xPow = xPow + (x[i] * x[i]);
    }
    xAdd = xAdd * divert;
    xAdd = xAdd * xAdd;
    xPow = xPow * divert;
    return xPow - xAdd;
}
/*float avg(float* x, int size) {
    float avg = 0;
    for(int i = 0; i < size; i++) {
        avg = avg + x[i];
    }
    avg = avg / size;
    return avg;
}
 */

float cov(float* x, float* y, int size) {
    float xA = 0, yA = 0, xyA = 0;
    for(int i = 0; i < size; i++) {
        xA = xA + x[i];
        yA = yA + y[i];
        xyA = xyA + x[i] * y[i];
    }
    xA = xA / size;
    yA = yA / size;
    xyA = xyA / size;
    return xyA - xA * yA;
}