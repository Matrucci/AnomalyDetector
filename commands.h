

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

class uploadTS: public Command {
    string description = "1.upload a time series csv file\n";
public:
    uploadTS(DefaultIO* dio):Command(dio) {};
    virtual void execute() {
        dio->write("Please upload your local train CSV file.\n");
        ofstream trainFile("anomalyTrain.csv");
        if (trainFile.is_open()) {
            //cout << "FUCK THIS SHIT!!!" << endl;
            string trainline = dio->read();
            while (trainline != "done") {
                //cout << trainline << endl;
                //cout << "FUCK THIS SHIT!!!" << endl;
                trainFile << trainline;
                trainline = dio->read();
            }
            trainFile.close();
            dio->write("Upload complete.\n");
        }
        //cout << "DONEEEEEEE!!!!" << endl;
        dio->write("Please upload your local test CSV file.\n");
        ofstream testFile("anomalyTest.csv");
        if (testFile.is_open()) {
            string testline = dio->read();
            while (testline != "done") {
                //cout << "I'M OUT!" << endl;
                testFile << testline;
                testline = dio->read();
            }
            testFile.close();
            dio->write("Upload complete.\n");
        }
    }
    virtual void printDes() {
        dio->write(description);
    }
};


class settings: public Command {
    string description = "2.algorithm settings\n";
    float threshold = 0.9;
public:
    settings(DefaultIO* dio):Command(dio) {};
    virtual void execute() {
        dio->write("The current correlation threshold is " + std::to_string(threshold) + "\n");
        float limit;
        dio->read(&limit);
        while (limit < 0 || limit > 1) {
            dio->write("please choose a value between 0 and 1.\n");
            dio->read(&limit);
        }
        threshold = limit;
    }
    float getThreshold() {
        return threshold;
    }
    virtual void printDes() {
        dio->write(description);
    }
};

class detectAnomalies: public Command {
    string description = "3.detect anomalies\n";
    HybridAnomalyDetector had;
public:
    detectAnomalies(DefaultIO* dio):Command(dio) {};
    virtual void execute() {
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");
        had.learnNormal(train);
        had.detect(test);
    }
    HybridAnomalyDetector getDetector() {
        return this->had;
    }
    virtual void printDes() {
        dio->write(description);
    }
};

class results: public Command {
    string description = "4.display results\n";
public:
    results(DefaultIO* dio):Command(dio) {};
    virtual void execute() {

    }
    virtual void printDes() {
        dio->write(description);
    }
};

class uploadAndAnalyze: public Command {
    string description = "5.upload anomalies and analyze results\n";
public:
    uploadAndAnalyze(DefaultIO* dio):Command(dio) {};
    virtual void execute() {

    }
    virtual void printDes() {
        dio->write(description);
    }
};

class exitProgram: public Command {
    string description = "6.exit\n";
public:
    exitProgram(DefaultIO* dio):Command(dio) {};
    virtual void execute() {

    }
    virtual void printDes() {
        dio->write(description);
    }
};

#endif /* COMMANDS_H_ */
