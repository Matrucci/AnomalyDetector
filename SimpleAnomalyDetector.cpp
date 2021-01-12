
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

/************************************
 * Creating an array from the vector.
 * @param a - float array.
 * @param toArrayA - float vector.
 ***********************************/
void buildArray(float* a, vector<float> toArrayA) {
    int i = 0;
    for (auto element: toArrayA) {
        a[i] = toArrayA[i];
        i++;
    }
}

/**
 *
 * @param a
 * @param b
 * @return
 */
Point** createPoints(vector<float> *a, vector<float> *b) {
    int sizeV = a->size();
    Point** points = new Point*[sizeV];
    for (int s = 0; s < sizeV; s++) {
        points[s] = new Point((*a)[s], (*b)[s]);
    }
    return points;
}


float getMaxForHigh(vector<float> *a, vector<float> *b, Point** points, Line* reg) {
    int sizeV = a->size();
    float maxRange = 0, currentDev;
    //Checking the max distance from the reg line.
    for (int s = 0; s < sizeV; s++) {
        currentDev = dev(*points[s], *reg);
        if (currentDev > maxRange) {
            maxRange = currentDev;
        }
    }
    return maxRange;
}

/******************************************************
 * Offline learning of the normal data.
 * Setting the parameters for the anomaly detection.
 * @param ts - TimeSeries variable containing the data.
 ******************************************************/
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
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
    float maxRange;
    Point** points;
    //Going over every 2 (not equal) columns.
    for (int i = 0; i < vectors.size() - 1; i++) {
        buildArray(a, vectors[i]);
        for (int j = i + 1; j < vectors.size(); j++) {
            buildArray(b,vectors[j]);
            //Checking the correlation.
            pAB = abs(pearson(a, b, vectors[i].size()));
            //Strong correlation.
            if (pAB > 0.9) {
                points = createPoints(&vectors[i], &vectors[j]);
                Line reg = linear_reg(points, sizeV);
                maxRange = 1.2 * getMaxForHigh(&vectors[i], &vectors[j], points, &reg);
                //Leaving some "wiggle room" for errors since an anomaly is a bigger difference.
                cf.push_back({features[i], features[j], pAB, reg, maxRange});
            }
        }
    }
}

/**************************************************
 * Getting the index of the title in the vector.
 * @param vec - vector of strings.
 * @param str - The string we're looking for.
 * @return - int - the index of the string.
 *************************************************/
int getIndex(vector<string> vec, string str) {
    for (int i = 0; i < vec.size(); i++) {
        if (str.compare(vec[i]) == 0) {
            return i;
        }
    }
    return -1; //Not found.
}

/***********************************************************
 * Going over TimeSeries and checking if there's an anomaly.
 * @param ts - The TimeSeries variable containing the data.
 * @return - A vector with all anomalies found in the data.
 **********************************************************/
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> ar;
    //Building the parameters.
    vector<string> features = ts.getFeatures();
    vector<vector<float>> vectors;
    for (int i = 0; i < features.size(); i++) {
        vectors.push_back(ts.getVectorByFeature(features[i]));
    }
    //For every row of data, check with the correlatedFeatures vector to see if there's an anomaly.
    for (int i = 0; i < vectors[0].size(); i++) {
        for (int j = 0; j < cf.size(); j++) {
            //Getting the index of the features.
            int indexI = getIndex(features, cf[j].feature1);
            int indexJ = getIndex(features, cf[j].feature2);
            //Getting the values of the row.
            float x = vectors[indexI][i];
            float y = vectors[indexJ][i];
            Point p = Point(x, y);
            float currentDev = dev(p, cf[j].lin_reg);
            //Checking if there's an anomaly.
            if (currentDev > cf[j].threshold) {
                string desc = cf[j].feature1 + "-" + cf[j].feature2;
                AnomalyReport anomalyReport = AnomalyReport(desc, i + 1);
                ar.push_back(anomalyReport);
            }
        }
    }
    return ar;
}

