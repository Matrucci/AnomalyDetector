
#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

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

/************************************************************
 * Taking 2 float vectors and creating a point array.
 * @param a - The first vector (the x value of the point).
 * @param b - The second vector (the y value of the point).
 * @return - An array of pointers to points.
 ************************************************************/
Point** createPoints(vector<float> *a, vector<float> *b) {
    int sizeV = a->size();
    Point** points = new Point*[sizeV];
    for (int s = 0; s < sizeV; s++) {
        points[s] = new Point((*a)[s], (*b)[s]);
    }
    return points;
}

/***********************************************************************
 * Learning the correlations and adding to the cf vector.
 * @param vectors - Vector holding all the vectors of all the features.
 * @param features - Vector holding the titles of the features
 * @param i - The first feature vector
 * @param j - The second feature vector
 * @param p - The pearson between the 2 features
 *************************************************************************/
void SimpleAnomalyDetector::learn(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p) {
    if (p >= 0.9) {
        Point** points = createPoints(&vectors[i], &vectors[j]);
        int sizeV = vectors[0].size();
        Line reg = linear_reg(points, sizeV);
        float maxRange = 0, currentDev;
        //Checking the max distance from the reg line.
        for (int s = 0; s < sizeV; s++) {
            currentDev = dev(*points[s], reg);
            if (currentDev > maxRange) {
                maxRange = currentDev;
            }
        }
        maxRange *= 1.2;
        cf.push_back({(*features)[i], (*features)[j], p, reg, maxRange, nullptr});
    }
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

    //Going over every 2 (not equal) columns.
    for (int i = 0; i < vectors.size() - 1; i++) {
        buildArray(a, vectors[i]);
        for (int j = i + 1; j < vectors.size(); j++) {
            buildArray(b,vectors[j]);
            //Checking the correlation.
            pAB = abs(pearson(a, b, vectors[i].size()));
            learn(vectors, &features, i, j, pAB);
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
            //detectHigh(i, j, &features, vectors, &ar);
            int indexI = getIndex(features, cf[j].feature1);
            int indexJ = getIndex(features, cf[j].feature2);
            //Getting the values of the row.
            float x = vectors[indexI][i];
            float y = vectors[indexJ][i];
            Point p = Point(x, y);
            //float currentDev = dev(p, cf[j].lin_reg);
            float dis = distanceBetween(p, cf[j]);
            //Checking if there's an anomaly.
            if (dis > cf[j].threshold) {
                string desc = cf[j].feature1 + "-" + cf[j].feature2;
                AnomalyReport anomalyReport = AnomalyReport(desc, i + 1);
                ar.push_back(anomalyReport);
            }
        }
    }
    return ar;
}

/***********************************************************
 * Calculating the dev between the point and the line
 * @param p - The point to check
 * @param cf - Struct holding the line and the correlation
 * @return float - The dev
 ***********************************************************/
float SimpleAnomalyDetector::distanceBetween(Point p, correlatedFeatures cf) {
    return dev(p, cf.lin_reg);
}

