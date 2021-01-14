

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    //void midPearson(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p);
    void detectMid(int i, int j, vector<string> *features, vector<vector<float>> vectors,
                   vector<AnomalyReport> *ar, vector<correlatedFeatures> cf);
    void midPearson(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p);
    virtual float distanceBetween(Point p, correlatedFeatures cf);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
