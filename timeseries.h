/**************************
 * Author: Matan Saloniko
 * ID: 31857076-9
 *************************/


#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <map>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class TimeSeries{
    std::map<string, vector<float>> dataMap;
    vector<string> headers;
public:
    TimeSeries(const char* CSVfileName);
    float getFeatureOnTime(string feature, float time);
    void addRow(vector<float> newRow);
    vector<string> getFeatures() const;
    vector<float> getVectorByFeature(string feature) const;
    vector<string> splitTxt(string toSplit);

};

#endif /* TIMESERIES_H_ */
