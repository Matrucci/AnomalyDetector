

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <map>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class TimeSeries{

public:
    std::map<string, vector<float>> map;
	TimeSeries(const char* CSVfileName){
	    vector<string> headers;
	    ifstream ip(CSVfileName);
	    if (!ip.is_open()) {
	        std::cout << "ERROR: File could not be opened" << '\n';
	    }
	    string title;
	    if (ip.good()) {
            while (getline(ip, title, ',')) {
                headers.push_back(title);
            }
	    }
	    vector<float> features[headers.size()];
	    string data;
	    float parsed;
	    while (ip.good()) {
            for(int i = 0; i < headers.size(); i++) {
                getline(ip, data, ',');
                parsed = std::stof(data);
                features[i].push_back(parsed);
            }
	    }
	    for (int i = 0; i < headers.size(); i++) {
	        map.insert(std::pair<string, vector<float>>(headers[i], features[i]));
	    }
	}
};

#endif /* TIMESERIES_H_ */
