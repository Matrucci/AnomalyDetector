

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

	// you may add additional methods here
};

// you may add here helper classes


// you may edit this class
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

// implement here your command classes

class UploadTS: public Command {
    string description = "1.upload a time series csv file\n";
public:
    UploadTS(DefaultIO* dio): Command(dio) {};
    virtual void execute() {
        dio->write("Please upload your local train CSV file.\n");
        ofstream trainFile("anomalyTrain.csv");
        //trainFile.open("anomalyTrain.csv");
        string trainline = dio->read() + '\n';
        while (trainline != "done\n") {
            trainFile << trainline;
            trainline = dio->read() + '\n';
        }
        trainFile.close();
        dio->write("Upload complete.\n");

        dio->write("Please upload your local test CSV file.\n");
        ofstream testFile("anomalyTest.csv");
        //testFile.open("anomalyTest.csv");
        string testline = dio->read() + '\n';
        while (testline != "done\n") {
            testFile << testline;
            testline = dio->read() + '\n';
        }
        testFile.close();
        dio->write("Upload complete.\n");
    }
    virtual void printDes() {
        dio->write(description);
    }
};


class Settings: public Command {
    string description = "2.algorithm settings\n";
    float threshold = 0.9;
public:
    Settings(DefaultIO* dio): Command(dio) {};
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
    float *getThreshold() {
        return &this->threshold;
    }
    virtual void printDes() {
        dio->write(description);
    }
};

class DetectAnomalies: public Command {
    string description = "3.detect anomalies\n";
    HybridAnomalyDetector had;
    vector<AnomalyReport> report;
public:
    DetectAnomalies(DefaultIO* dio): Command(dio) {};
    virtual void execute() {
        //TimeSeries train("anomalyTrain.csv");
        //TimeSeries test("anomalyTest.csv");
        had.learnNormal(TimeSeries("anomalyTrain.csv"));
        report = had.detect(TimeSeries("anomalyTest.csv"));
        dio->write("anomaly detection complete.\n");
    }
    void setThreshold(float *limit) {
        had = HybridAnomalyDetector(*limit);
    }
    vector<AnomalyReport> *getAnomalies() {
        return &this->report;
    }
    virtual void printDes() {
        dio->write(description);
    }
};

class Results: public Command {
    string description = "4.display results\n";
    vector<AnomalyReport> *ar;
public:
    Results(DefaultIO* dio): Command(dio) {};
    virtual void execute() {

        for(AnomalyReport a: *ar) {
            dio->write(std::to_string(a.timeStep) + "\t" + a.description + "\n");
        }
        dio->write("Done.\n");

    }
    void setReport(vector<AnomalyReport> *report) {
        this->ar = report;
    }
    virtual void printDes() {
        dio->write(description);
    }
};

class UploadAndAnalyze: public Command {
    string description = "5.upload anomalies and analyze results\n";
    vector<AnomalyReport> *ar;
public:
    UploadAndAnalyze(DefaultIO* dio): Command(dio) {};
    virtual void execute() {

        vector<int> timeSteps;
        vector<int> anomalies;
        float P = 0;
        float tp = 0;
        float sum = 0;
        float fp = 0;
        //timeSteps.push_back(0);
        int j = 0;
        dio->write("Please upload your local anomalies file.\n");
        string line = dio->read() + '\n';
        while (line != "done\n") {
            timeSteps.push_back(0);
            for (int i = 0; line[i] != '\n'; i++) {
                if (line[i] == ',') {
                    j++;
                    timeSteps.push_back(0);
                } else {
                    //cout << (line[i] - 48) << endl;
                    timeSteps[j] = timeSteps[j] * 10 + (line[i] - 48);
                }
            }
            j++;
            P++;
            line = dio->read() + '\n';
        }
        dio->write("Upload complete.\n");
        int currentTime = ar->at(0).timeStep;
        vector<int> group;
        group.push_back(ar->at(0).timeStep);

        /*for (int i = 0; i < ar->size() - 1; i++) {
            if (ar->at(i + 1).timeStep != currentTime + 1 || ar->at(i).description != ar->at(i + 1).description) {
                group.push_back(anomalies[i]);
            }
            currentTime = ar->at(i + 1).timeStep;
            cout<<i<<endl;
        }*/

        for (AnomalyReport a: *ar) {
            anomalies.push_back(a.timeStep);
        }


        /*for (int i = 0; i < anomalies.size() - 1; i++) {
            if (anomalies[i + 1] != currentTime + 1 ) {
                group.push_back(anomalies[i]);
            }
            currentTime = anomalies[i];
        }*/


        for (int i = 0; i < timeSteps.size(); i += 2) {
            for (int k = 0; k < anomalies.size(); k++) {
                if (timeSteps[i] <= anomalies[k] && anomalies[k] <= timeSteps[i + 1]) {
                    tp++;
                    break;
                }
            }
        }
        /*
        for (int a: timeSteps) {
            cout << a << "," << endl;
        }
        cout << "-------------------" << endl;
        */
        for (int i = 0; i < timeSteps.size() - 1; i++) {
            sum += (timeSteps[i + 1] - timeSteps[i]) + 1;
        }
        //cout << ((P - tp) / (200 - sum)) << endl;
        dio->write("True Positive Rate: " + std::to_string(tp / P) + '\n');
        dio->write("False Positive Rate: " + std::to_string((P - tp) / (200 - sum)) + '\n');
    }
    virtual void printDes() {
        dio->write(description);
    }
    void setReport(vector<AnomalyReport> *report) {
        this->ar = report;
    }
};

class ExitProgram: public Command {
    string description = "6.exit\n";
public:
    ExitProgram(DefaultIO* dio): Command(dio) {};
    virtual void execute() {

    }
    virtual void printDes() {
        dio->write(description);
    }
};

#endif /* COMMANDS_H_ */
