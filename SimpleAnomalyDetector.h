

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
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual void highPearson(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p);
    virtual void detectHigh(int i, int j, vector<string> *features, vector<vector<float>> vectors,
                            vector<AnomalyReport> *ar);
    //virtual void midPearson(vector<vector<float>> vectors, vector<string> *features, int i, int j, float p);
    virtual float distanceBetween(Point p, correlatedFeatures cf);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
    void setNormalModel(vector<correlatedFeatures> newCF){
        cf = newCF;
    }

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
