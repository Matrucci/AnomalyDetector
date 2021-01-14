
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * @param a
 * @param b
 * @return
 */
Point** buildPointArray(vector<float> *a, vector<float> *b) {
    int sizeV = a->size();
    Point** points = new Point*[sizeV];
    for (int s = 0; s < sizeV; s++) {
        points[s] = new Point((*a)[s], (*b)[s]);
    }
    return points;
}

/**
 *
 * @param p
 * @param cf
 * @return
 */
float HybridAnomalyDetector::distanceBetween(Point p, correlatedFeatures cf) {
    if (cf.corrlation >= 0.9) {
        return SimpleAnomalyDetector::distanceBetween(p, cf);
    }
    return distacneBetween(*cf.circleCenter, p);
}

/**
 *
 * @param vectors
 * @param features
 * @param i
 * @param j
 * @param p
 */
void HybridAnomalyDetector::learn(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p) {
    if (p >= 0.9) {
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

