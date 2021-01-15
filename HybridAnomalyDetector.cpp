/**************************
 * Author: Matan Saloniko
 * ID: 31857076-9
 *************************/

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector(float limit) : SimpleAnomalyDetector(limit) {}

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

/************************************************************
 * Taking 2 float vectors and creating a point array.
 * @param a - The first vector (the x value of the point).
 * @param b - The second vector (the y value of the point).
 * @return - An array of pointers to points.
 ************************************************************/
Point** buildPointArray(vector<float> *a, vector<float> *b) {
    int sizeV = a->size();
    Point** points = new Point*[sizeV];
    for (int s = 0; s < sizeV; s++) {
        points[s] = new Point((*a)[s], (*b)[s]);
    }
    return points;
}

/**************************************************************************************************
 * If the correlation is 0.9 and above, we want to check the dev from the line.
 * If the correlation is 0.5 or higher we want to know the distance from the center of the circle.
 * @param p - The point to check the distance from.
 * @param cf - The struct holding the correlation, the linear reg and the circle center.
 * @return float - the distance.
 **************************************************************************************************/
float HybridAnomalyDetector::distanceBetween(Point p, correlatedFeatures cf) {
    if (cf.corrlation >= this->getThresholdLimit()) {
        return SimpleAnomalyDetector::distanceBetween(p, cf);
    }
    return distacneBetween(*cf.circleCenter, p);
}

/***********************************************************************
 * Learning the correlations and adding to the cf vector.
 * This overrides the function from SimpleAnomalyDetector.
 * @param vectors - Vector holding all the vectors of all the features.
 * @param features - Vector holding the titles of the features
 * @param i - The first feature vector
 * @param j - The second feature vector
 * @param p - The pearson between the 2 features
 *************************************************************************/
void HybridAnomalyDetector::learn(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p) {
    if (p >= this->getThresholdLimit()) {
        SimpleAnomalyDetector::learn(vectors, features, i, j, p);
    } else if (p > 0.5) {
        vector<correlatedFeatures> cf = getNormalModel();
        Point** points = buildPointArray(&vectors[i], &vectors[j]);
        int sizeV = vectors[0].size();
        Line reg = linear_reg(points, sizeV);
        Circle minCir = findMinCircle(points, sizeV);
        Point *center = new Point(minCir.center.x, minCir.center.y);
        float maxRange = minCir.radius;
        maxRange *= 1.1;

        cf.push_back({(*features)[i], (*features)[j], p, reg, maxRange, center});
        SimpleAnomalyDetector::setNormalModel(cf);
    }
}

