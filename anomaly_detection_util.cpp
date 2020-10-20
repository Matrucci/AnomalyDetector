//
// Created by Matan Saloniko on 20/10/2020.
//

#include "anomaly_detection_util.h"

float var(float* x, int size) {
    float sumSqrt = 0;
    for (int i = 0; i < size; i++) {
        sumSqrt += x[i] * x[i];
    }

    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }

    float miu = sum / size;
    return (sumSqrt / size) - miu;
}

