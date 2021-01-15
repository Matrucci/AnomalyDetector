/**************************
 * Author: Matan Saloniko
 * ID: 31857076-9
 *************************/

#include <sstream>
#include "timeseries.h"

using namespace std;

/************************************************
 * Splitting a string by commas.
 * @param toSplit - the string we want to split.
 * @return - a vector with the splitted string.
 ************************************************/
vector<string> TimeSeries::splitTxt(string toSplit){
    vector<string> line;
    istringstream ss(toSplit);
    string substring;
    while(getline(ss, substring, ',')){
        line.push_back(substring);
    }
    return line;
}

/***********************************************************************************************
 * Constructor.
 * Parsing the data from the CSV and adding it to the map with the title and the data (column).
 * @param CSVfileName - The csv input.
 ************************************************************************************************/
TimeSeries::TimeSeries(const char *CSVfileName) {
    ifstream ip(CSVfileName);
    if (ip.is_open()) {
        string newLine;
        vector<string> splittedLine;
        getline(ip, newLine);
        splittedLine = splitTxt(newLine);
        //Reading only the first line for titles.
        for(int i = 0; i < splittedLine.size(); i++){
            headers.push_back(splittedLine[i]);
        }
        vector<float> features[headers.size()];
        //Reading the rest of the file for the data.
        while (getline(ip, newLine)){
            splittedLine = splitTxt(newLine);
            for(int i = 0; i < splittedLine.size(); i++){
                features[i].push_back(stof(splittedLine[i]));
            }
        }
        //Inserting everything to the map.
        for (int i = 0; i < headers.size(); i++) {
            dataMap.insert(std::pair<string, vector<float>>(headers[i], features[i]));
        }
    }
    ip.close();
}


/***
 * Getting the data from column "feature" in the given time.
 * @param feature - The title (column).
 * @param time - the time.
 * @return - the data in that cell.
 */
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

/********************************************
 * Adding a new row of data.
 * @param newRow - new float vector to add.
 ********************************************/
void TimeSeries::addRow(vector<float> newRow) {
    for (int i = 0; i < headers.size(); i++) {
        if (dataMap.count(headers[i]) == 1) {
            dataMap[headers[i]].push_back(newRow[i]);
        }
    }
}

/************************
 * @return - The titles.
 ***********************/
vector<string> TimeSeries::getFeatures() const {
    vector<string> headersCopy;
    for (const auto& element: headers) {
        headersCopy.push_back(element);
    }
    return headersCopy;
}

/*******************************************
 * Getting the correct column by the title.
 * @param feature - The title.
 * @return - The correct column as a vector.
 *******************************************/
vector<float> TimeSeries::getVectorByFeature(string feature) const {
    vector<float> copy;
    for(auto element: dataMap.find(feature)->second) {
        copy.push_back(element);
    }
    return copy;
}