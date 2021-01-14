

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    //virtual void learnNormal(const TimeSeries& ts);
    virtual void learn(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    void midPearson(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p);
    virtual float distanceBetween(Point p, correlatedFeatures cf);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
