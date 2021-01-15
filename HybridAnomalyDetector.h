

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	explicit HybridAnomalyDetector(float limit);
    HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void learn(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p);
    virtual float distanceBetween(Point p, correlatedFeatures cf);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
