
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
void buildArray1(float* a, vector<float> toArrayA) {
    int i = 0;
    for (auto element: toArrayA) {
        a[i] = toArrayA[i];
        i++;
    }
}

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
        buildArray1(a, vectors[i]);
        for (int j = i + 1; j < vectors.size(); j++) {
            buildArray1(b,vectors[j]);
            //Checking the correlation.
            pAB = abs(pearson(a, b, vectors[i].size()));
            //Strong correlation.
            if (pAB >= 0.9) {
                SimpleAnomalyDetector::highPearson(vectors, &features, i, j, pAB);
            } else {
                if (pAB > 0.5) {
                    HybridAnomalyDetector::midPearson(vectors, &features, i, j, pAB);
                }
            }
        }
    }
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    /*vector<correlatedFeatures> cf = this->getNormalModel();
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


            if (cf[j].circleCenter == nullptr) {
                SimpleAnomalyDetector::detectHigh(i, j, &features, vectors, &ar);
            } else {
                HybridAnomalyDetector::detectMid(i, j, &features, vectors, &ar, cf);
            }

        }
    }
    return ar; */
    return SimpleAnomalyDetector::detect(ts);
}

/**************************************************
 * Getting the index of the title in the vector.
 * @param vec - vector of strings.
 * @param str - The string we're looking for.
 * @return - int - the index of the string.
 *************************************************/
int getIndex1(vector<string> vec, string str) {
    for (int i = 0; i < vec.size(); i++) {
        if (str.compare(vec[i]) == 0) {
            return i;
        }
    }
    return -1; //Not found.
}

/*
void HybridAnomalyDetector::detectMid(int i, int j, vector<string> *features, vector<vector<float>> vectors,
               vector<AnomalyReport> *ar, vector<correlatedFeatures> cf) {
    int indexI = getIndex1(*features, cf[j].feature1);
    int indexJ = getIndex1(*features, cf[j].feature2);
    //Getting the values of the row.
    float x = vectors[indexI][i];
    float y = vectors[indexJ][i];
    Point p = Point(x, y);
    Circle c = Circle(*(cf[j].circleCenter), cf[j].threshold);
    //float currentDev = dev(p, cf[j].lin_reg);
    //Checking if there's an anomaly.
    float dis = distacneBetween(p, *cf[j].circleCenter);
    //cout << "X:Y:: " << p.x <<":" << p.y << " dis:: " << dis << " radius:: " << c.radius << endl;
    if (!isPointInCircle(c, p)) {
        //cout << "^ ANOMALY ^" << endl;
        string desc = cf[j].feature1 + "-" + cf[j].feature2;
        AnomalyReport anomalyReport = AnomalyReport(desc, i + 1);
        (*ar).push_back(anomalyReport);
    }
}
*/

/**
 *
 * @param a
 * @param b
 * @return
 */
Point** createPoints1(vector<float> *a, vector<float> *b) {
    int sizeV = a->size();
    Point** points = new Point*[sizeV];
    for (int s = 0; s < sizeV; s++) {
        points[s] = new Point((*a)[s], (*b)[s]);
    }
    return points;
}

void HybridAnomalyDetector::midPearson(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p) {
    vector<correlatedFeatures> cf = getNormalModel();
    Point** points = createPoints1(&vectors[i], &vectors[j]);
    int sizeV = vectors[0].size();
    Line reg = linear_reg(points, sizeV);
    Circle minCir = findMinCircle(points, sizeV);
    Point *center = new Point(minCir.center.x, minCir.center.y);
    float maxRange = minCir.radius;
    maxRange *= 1.1;

    cf.push_back({(*features)[i], (*features)[j], p, reg, maxRange, center});
    SimpleAnomalyDetector::setNormalModel(cf);
}



float HybridAnomalyDetector::distanceBetween(Point p, correlatedFeatures cf) {
    if (cf.corrlation >= 0.9) {
        return SimpleAnomalyDetector::distanceBetween(p, cf);
    }
    return distacneBetween(*cf.circleCenter, p);
}

