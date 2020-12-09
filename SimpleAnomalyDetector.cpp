
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

void buildArray(float* a, vector<float> toArrayA) {
    int i = 0;
    for (auto element: toArrayA) {
        a[i] = toArrayA[i];
        i++;
    }
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    vector<string> features = ts.getFeatures();
    vector<vector<float>> vectors;
    for (int i = 0; i < features.size(); i++) {
        vectors.push_back(ts.getVectorByFeature(features[i]));
    }

    int sizeV = vectors[0].size();
    float a[sizeV], b[sizeV];
    float pAB;
    Point** points = new Point*[sizeV];
    for (int i = 0; i < vectors.size() - 1; i++) {
        buildArray(a, vectors[i]);
        for (int j = i + 1; j < vectors.size(); j++) {
            buildArray(b,vectors[j]);
            pAB = abs(pearson(a, b, vectors[i].size()));
            if (pAB > 0.9) {
                for (int s = 0; s < sizeV; s++) {
                    points[s] = new Point(vectors[i][s], vectors[j][s]);
                }
                Line reg = linear_reg(points, sizeV);
                float maxRange = 0, currentDev;
                for (int s = 0; s < sizeV; s++) {
                    currentDev = dev(*points[s], reg);
                    if (currentDev > maxRange) {
                        maxRange = currentDev;
                    }
                }
                maxRange = maxRange * 1.2;
                cf.push_back({features[i], features[j], pAB, reg, maxRange});
            }
        }
    }
}

int getIndex(vector<string> vec, string str) {
    for (int i = 0; i < vec.size(); i++) {
        if (str.compare(vec[i]) == 0) {
            return i;
        }
    }
    return -1;
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> ar;
    //Building the parameters.
    vector<string> features = ts.getFeatures();
    vector<vector<float>> vectors;
    for (int i = 0; i < features.size(); i++) {
        vectors.push_back(ts.getVectorByFeature(features[i]));
    }
    for (int i = 0; i < vectors[0].size(); i++) {
        for (int j = 0; j < cf.size(); j++) {
            int indexI = getIndex(features, cf[j].feature1);
            int indexJ = getIndex(features, cf[j].feature2);
            float x = vectors[indexI][i];
            float y = vectors[indexJ][i];
            Point p = Point(x, y);
            float currentDev = dev(p, cf[j].lin_reg);
            if (currentDev > cf[j].threshold) {
                string desc = cf[j].feature1 + "-" + cf[j].feature2;
                cout << desc << endl;
                cout << currentDev << ", " << cf[j].threshold << ", " << i + 1 << endl;
                AnomalyReport anomalyReport = AnomalyReport(desc, i + 1);
                ar.push_back(anomalyReport);
            }
        }
    }
    return ar;
}

