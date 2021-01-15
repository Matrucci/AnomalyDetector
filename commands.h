/**************************
 * Author: Matan Saloniko
 * ID: 31857076-9
 *************************/

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};


class Command{
protected:
    DefaultIO* dio;
public:
    Command(DefaultIO* dio):dio(dio){}
    Command(){}
    virtual void printDes() {};
    virtual void execute() = 0;
    virtual ~Command(){}
};

/********************************************
 * Command class for uploading the csv files
 *******************************************/
class UploadTS: public Command {
    string description = "1.upload a time series csv file\n";
    int rows = 0;
public:
    UploadTS(DefaultIO* dio): Command(dio) {};

    /************************************************
     * Executing.
     * Getting a train csv file and a test csv file.
     ***********************************************/
    virtual void execute() {
        //Getting the train csv
        dio->write("Please upload your local train CSV file.\n");
        ofstream trainFile("anomalyTrain.csv");
        string trainline = dio->read() + '\n';
        while (trainline != "done\n") {
            trainFile << trainline;
            trainline = dio->read() + '\n';
        }
        trainFile.close();
        dio->write("Upload complete.\n");

        //Getting the test csv
        dio->write("Please upload your local test CSV file.\n");
        ofstream testFile("anomalyTest.csv");
        string testline = dio->read() + '\n';
        while (testline != "done\n") {
            this->rows++;
            testFile << testline;
            testline = dio->read() + '\n';
        }
        testFile.close();
        this->rows--;
        dio->write("Upload complete.\n");
    }

    /******************************************
     * Printing the description of the class.
     *****************************************/
    virtual void printDes() {
        dio->write(description);
    }

    /**************************************************************************
     * @return The number of rows in the test file (not including the titles).
     **************************************************************************/
    int* getRows() {
        return &this->rows;
    }
};

/*******************************************************
 * Command class for changing the correlation threshold
 *******************************************************/
class Settings: public Command {
    string description = "2.algorithm settings\n";
    float threshold = 0.9;
public:
    Settings(DefaultIO* dio): Command(dio) {};

    /*************************************************************************************************
     * Executing.
     * Displaying the current threshold (0.9 by default) and giving the user an option to change it.
     ************************************************************************************************/
    virtual void execute() {
        dio->write("The current correlation threshold is " + std::to_string(threshold) + "\n");
        dio->write("Type a new threshold\n");
        float limit;
        dio->read(&limit);
        while (limit < 0 || limit > 1) {
            dio->write("please choose a value between 0 and 1.\n");
            dio->read(&limit);
        }
        threshold = limit;
    }

    /**************************************
     * @return A pointer to the threshold.
     *************************************/
    float *getThreshold() {
        return &this->threshold;
    }

    /******************************************
     * Printing the description of the class.
     *****************************************/
    virtual void printDes() {
        dio->write(description);
    }
};

/********************************************
 * Command class for detecting the anomalies.
 ********************************************/
class DetectAnomalies: public Command {
    string description = "3.detect anomalies\n";
    HybridAnomalyDetector had;
    vector<AnomalyReport> report;
    float* limit;
public:
    DetectAnomalies(DefaultIO* dio): Command(dio) {};

    /******************************************************
     * Executing.
     * Learning and detecting with the changed threshold.
     *****************************************************/
    virtual void execute() {
        had = HybridAnomalyDetector(*limit);
        had.learnNormal(TimeSeries("anomalyTrain.csv"));
        report = had.detect(TimeSeries("anomalyTest.csv"));
        dio->write("anomaly detection complete.\n");
    }

    /**************************************************************
     * @param nLimit - the new threshold for the anomaly detector.
     **************************************************************/
    void setThreshold(float *nLimit) {
        this->limit = nLimit;
    }

    /***************************************
     * @return - The anomaly report vector.
     **************************************/
    vector<AnomalyReport> *getAnomalies() {
        return &this->report;
    }
    /******************************************
     * Printing the description of the class.
     *****************************************/
    virtual void printDes() {
        dio->write(description);
    }
};

/********************************************
 * Command class for displaying the results.
 ********************************************/
class Results: public Command {
    string description = "4.display results\n";
    vector<AnomalyReport> *ar;
public:
    Results(DefaultIO* dio): Command(dio) {};

    /***************************************
     * Executing.
     * Displaying all anomalies detected.
     ***************************************/
    virtual void execute() {
        for(AnomalyReport a: *ar) {
            dio->write(std::to_string(a.timeStep) + "\t" + a.description + "\n");
        }
        dio->write("Done.\n");
    }

    /****************************************
     * @param report - the anomalies vector.
     ****************************************/
    void setReport(vector<AnomalyReport> *report) {
        this->ar = report;
    }

    /******************************************
     * Printing the description of the class.
     *****************************************/
    virtual void printDes() {
        dio->write(description);
    }
};

/**************************************************************************
 * Command class for getting anomalies timesteps and analyzing the results.
 **************************************************************************/
class UploadAndAnalyze: public Command {
    string description = "5.upload anomalies and analyze results\n";
    vector<AnomalyReport> *ar;
    int *rows;
public:
    UploadAndAnalyze(DefaultIO* dio): Command(dio) {};

    /************************************************************************
     * Executing.
     * Getting time steps ranges and analyzing the results of the detection.
     ***********************************************************************/
    virtual void execute() {
        vector<int> timeSteps; //Reported anomalies from the user.
        vector<int> anomalies; //Anomalies found from the detection.
        float P = 0; //The number of time step ranges given.
        float tp = 0; //Number of true positives.
        float sum = 0; //Number of anomalies reported by the user.
        float fp = 0; //Number of false positives
        int j = 0;
        //Getting the file from the user.
        dio->write("Please upload your local anomalies file.\n");
        string line = dio->read() + '\n';
        while (line != "done\n") {
            timeSteps.push_back(0);
            for (int i = 0; line[i] != '\n'; i++) {
                if (line[i] == ',') {
                    j++;
                    timeSteps.push_back(0);
                } else {
                    timeSteps[j] = timeSteps[j] * 10 + (line[i] - 48);
                }
            }
            j++;
            P++;
            line = dio->read() + '\n';
        }
        dio->write("Upload complete.\n");


        for (AnomalyReport a: *ar) {
            anomalies.push_back(a.timeStep);
        }

        //Creating a vector that combines a series of anomalies with following times.
        int currentTime = ar->at(0).timeStep;
        vector<int> group;
        group.push_back(ar->at(0).timeStep);

        for (int i = 0; i < ar->size() - 1; i++) {
            if (ar->at(i + 1).timeStep != currentTime + 1 || ar->at(i).description != ar->at(i + 1).description) {
                group.push_back(anomalies[i]);
                group.push_back(anomalies[i + 1]);
            }
            currentTime = ar->at(i + 1).timeStep;
        }
        group.push_back(anomalies[anomalies.size() - 1]);

        //Calculating the true positives.
        for (int i = 0; i < timeSteps.size(); i += 2) {
            for (int k = 0; k < anomalies.size(); k++) {
                if (timeSteps[i] <= anomalies[k] && anomalies[k] <= timeSteps[i + 1]) {
                    tp++;
                    break;
                }
            }
        }

        //Calculating the number of anomalies reported.
        for (int i = 0; i < timeSteps.size() - 1; i+=2) {
            sum += (timeSteps[i + 1] - timeSteps[i]) + 1;
        }

        //Calculating true positive rate.
        float tpr = (tp / P);
        //Making sure the rate only shows 3 digits after decimal point.
        tpr = tpr * 1000;
        tpr = floorf(tpr);
        tpr = tpr / 1000;
        fp = (group.size() / 2) - tp;
        //Calculating false positive rate.
        float fpr = fp / (*this->rows - sum);
        //Making sure the rate only shows 3 digits after decimal point.
        fpr = fpr * 1000;
        fpr = floorf(fpr);
        fpr = fpr / 1000;
        //Printing to the user.
        dio->write("True Positive Rate: " + removeZero(tpr) + '\n');
        dio->write("False Positive Rate: " + removeZero(fpr) + '\n');
    }

    /*****************************************************************
     * Removing trailing zeros from the float and returning a string.
     * @param num - the float we want to remove trailing zeros from.
     * @return - a string without trailing zeros.
     ****************************************************************/
    string removeZero(float num) {
        //Just to remove any chance of getting -0.
        if (num == 0) {
            return "0";
        }
        string str = std::to_string(num);
        int breakPoint;
        for (int i = str.size() - 1; i >= 0; i--) {
            if (str[i] != '0' && str[i] != '.') {
                breakPoint = i;
                break;
            }
        }
        string strZ = str.substr(0, breakPoint + 1);
        return strZ;
    }

    /******************************************
     * Printing the description of the class.
     *****************************************/
    virtual void printDes() {
        dio->write(description);
    }

    /*****************************************
     * @param report - the anomalies vector.
     ****************************************/
    void setReport(vector<AnomalyReport> *report) {
        this->ar = report;
    }

    /********************************************************************************
     * @param fileRows - the number of rows in the test file (not including titles).
     ********************************************************************************/
    void setRows(int* fileRows) {
        this->rows = fileRows;
    }
};

class ExitProgram: public Command {
    string description = "6.exit\n";
public:
    ExitProgram(DefaultIO* dio): Command(dio) {};
    virtual void execute() {

    }
    /******************************************
     * Printing the description of the class.
     *****************************************/
    virtual void printDes() {
        dio->write(description);
    }
};

#endif /* COMMANDS_H_ */
