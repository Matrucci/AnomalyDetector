
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void buildArray(float* a, vector<float> toArrayA) {
    int i = 0;
    for (auto element: toArrayA) {
        a[i] = toArrayA[i];
        i++;
        //cout << a[i] << ", " << endl;
    }
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    vector<string> features = ts.getFeatures();
    vector<vector<float>> vectors;
    //cout << "TESTTTT:: " << features.size() << endl;
    for (int i = 0; i < features.size(); i++) {
        vectors.push_back(ts.getVectorByFeature(features[i]));
    }

    int sizeV = vectors[0].size();
    float a[sizeV], b[sizeV];
    float pAB, maxAB = -1;
    int indexI, indexJ;
    Point** points = new Point*[sizeV];
    for (int i = 0; i < vectors.size() - 1; i++) {
        buildArray(a, vectors[i]);
        for (int j = i + 1; j < vectors.size(); j++) {
            buildArray(b,vectors[j]);
            pAB = abs(pearson(a, b, vectors[i].size()));
            //cout << features[i] << " " << features[j] << endl;
            //cout << "PEARSON: " << pAB << endl;
            if (pAB > maxAB) {
                maxAB = pAB;
                indexI = i;
                indexJ = j;
            }
        }
        if (maxAB > 0.9) {
            for (int j = 0; j < sizeV; j++) {
                points[j] = new Point(vectors[indexI][j], vectors[indexJ][j]);
            }

            Line reg = linear_reg(points, sizeV);
            float maxRange = 0, currentDev;
            for (int j = 0; j < sizeV; j++) {
                currentDev = dev(*points[i], reg);
                if (currentDev > maxRange) {
                    maxRange = currentDev;
                }
            }
            cf.push_back({features[indexI], features[indexJ], maxAB, reg, maxRange});
        }
        maxAB = -1;
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
            float currentDev = abs(dev(p, cf[i].lin_reg));
            if (currentDev > cf[i].threshold) {
                string desc = cf[j].feature1 + "-" + cf[j].feature2;
                AnomalyReport anomalyReport = AnomalyReport(desc, i + 1);
                ar.push_back(anomalyReport);
            }
        }
    }
    return ar;
}

