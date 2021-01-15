/**************************
 * Author: Matan Saloniko
 * ID: 31857076-9
 *************************/


#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	Point* circleCenter;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	vector<correlatedFeatures> cf;
	float thresholdLimit = 0.9;
public:
    SimpleAnomalyDetector();
	explicit SimpleAnomalyDetector(float limit);
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual void learn(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p);
    virtual float distanceBetween(Point p, correlatedFeatures cf);

	vector<correlatedFeatures> getNormalModel(){
		return this->cf;
	}
	float getThresholdLimit() {
	    return this->thresholdLimit;
	}
    void setNormalModel(vector<correlatedFeatures> newCF){
        cf = newCF;
    }

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
