
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

/************************************
 * Creating an array from the vector.
 * @param a - float array.
 * @param toArrayA - float vector.
 ***********************************/
void createArray(float* a, vector<float> toArrayA) {
    int i = 0;
    for (auto element: toArrayA) {
        a[i] = toArrayA[i];
        i++;
    }
}
/*
void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    vector<string> features = ts.getFeatures();
    vector<vector<float>> vectors;
    //Getting all the vectors from ts.
    for (int i = 0; i < features.size(); i++) {
        vectors.push_back(ts.getVectorByFeature(features[i]));
    }
    //Number of lines we have to learn.
    int sizeV = vectors[0].size();
    float a[sizeV], b[sizeV];
    float pAB;

    //Going over every 2 (not equal) columns.
    for (int i = 0; i < vectors.size() - 1; i++) {
        createArray(a, vectors[i]);
        for (int j = i + 1; j < vectors.size(); j++) {
            createArray(b,vectors[j]);
            //Checking the correlation.
            pAB = abs(pearson(a, b, vectors[i].size()));
            //Strong correlation.
            if (pAB >= 0.9) {
                SimpleAnomalyDetector::learn(vectors, &features, i, j, pAB);
            } else {
                if (pAB > 0.5) {
                    HybridAnomalyDetector::midPearson(vectors, &features, i, j, pAB);
                }
            }
        }
    }
}
*/


vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    return SimpleAnomalyDetector::detect(ts);
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
/*
void HybridAnomalyDetector::midPearson(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p) {
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
*/


float HybridAnomalyDetector::distanceBetween(Point p, correlatedFeatures cf) {
    if (cf.corrlation >= 0.9) {
        return SimpleAnomalyDetector::distanceBetween(p, cf);
    }
    return distacneBetween(*cf.circleCenter, p);
}

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

