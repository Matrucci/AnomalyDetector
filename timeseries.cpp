#include "timeseries.h"

using namespace std;

bool endsWith(const std::string &mainStr, const std::string &toMatch)
{
    if(mainStr.size() >= toMatch.size() &&
       mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
        return true;
    else
        return false;
}

TimeSeries::TimeSeries(const char *CSVfileName) {
    //vector<string> headers;
    ifstream ip(CSVfileName);
    if (!ip.is_open()) {
        std::cout << "ERROR: File could not be opened" << '\n';
    }
    string title;
    if (ip.good()) {
        getline(ip, title, '\n');
        string splitBy = ",";
        size_t pos = 0;
        std::string token;
        while ((pos = title.find(splitBy)) != std::string::npos) {
            token = title.substr(0, pos);
            headers.push_back(token);
            title.erase(0, pos + splitBy.length());
        }
        headers.push_back(title);
    }
    vector<float> features[headers.size()];
    string data;
    float parsed;
    while (ip.good()) {
        int index = 0;
        getline(ip, title, '\n');
        string splitBy = ",";
        size_t pos = 0;
        std::string token;
        while ((pos = title.find(splitBy)) != std::string::npos) {
            token = title.substr(0, pos);
            //cout << token << endl;
            parsed = std::stof(token);
            features[index].push_back(parsed);
            title.erase(0, pos + splitBy.length());
            index++;
        }
        //cout << title << endl;
        parsed = std::stof(title);
        features[index].push_back(parsed);
        /*
        for(int i = 0; i < headers.size(); i++) {
            getline(ip, data, ',');
            parsed = std::stof(data);
            features[i].push_back(parsed);
        }
         */
    }
    for (int i = 0; i < headers.size(); i++) {
        dataMap.insert(std::pair<string, vector<float>>(headers[i], features[i]));
    }
}

float TimeSeries::getFeatureOnTime(string feature, float time) {
    if (dataMap.count(feature) == 1) {
        int i = 0;
        for (auto element: dataMap[headers[0]]) {
            if (element == time) {
                return dataMap[feature][i];
            }
            i++;
        }
    }
    return -1; //ERROR
}

void TimeSeries::addRow(vector<float> newRow) {
    for (int i = 0; i < headers.size(); i++) {
        if (dataMap.count(headers[i]) == 1) {
            dataMap[headers[i]].push_back(newRow[i]);
        }
    }
}
vector<string> TimeSeries::getFeatures() const {
    vector<string> headersCopy;
    for (const auto& element: headers) {
        headersCopy.push_back(element);
    }
    /*
    for (int i = 0; i < headersCopy.size(); i++) {
        cout << headersCopy[i] << endl;
    }
     */
    return headersCopy;
}

vector<float> TimeSeries::getVectorByFeature(string feature) const {
    vector<float> copy;
    for(auto element: dataMap.find(feature)->second) {
        copy.push_back(element);
    }
    return copy;
}