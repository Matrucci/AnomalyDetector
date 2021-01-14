

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
    virtual void printDes();
    virtual void execute() = 0;
    virtual ~Command(){}
};

// implement here your command classes

class uploadTS: public Command {
    string description = "1. upload a time series csv file\n";
public:
    uploadTS();
    virtual void execute() {
        dio->write("Please upload your local train CSV file.");
        ofstream trainFile("anomalyTrain.csv");
        if (trainFile.is_open()) {
            string trainline = dio->read();
            while (trainline != "done\n") {
                trainFile << trainline;
            }
            trainFile.close();
            dio->write("Upload complete.");
        }
        dio->write("Please upload your local test CSV file.");
        ofstream testFile("anomalyTest.csv");
        if (testFile.is_open()) {
            string testline = dio->read();
            while (testline != "done\n") {
                testFile << testline;
            }
            testFile.close();
            dio->write("Upload complete.");
        }
    }
    virtual void printDes() {
        dio->write(description);
    }
};


class settings: public Command {
    string description = "2. algorithm settings\n";
    float threshold = 0.9;
public:
    settings();
    virtual void execute() {
        dio->write("The current correlation threshold is " + std::to_string(threshold));
        float limit;
        dio->read(&limit);
        while (limit < 0 || limit > 1) {
            dio->write("please choose a value between 0 and 1.");
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
    string description = "3. detect anomalies\n";
    HybridAnomalyDetector had;
public:
    detectAnomalies();
    virtual void execute() {
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");
        had.learnNormal(train);
        had.detect(test);
    }
    virtual void printDes() {
        dio->write(description);
    }
};

class results: public Command {
    string description = "4. display results\n";
public:
    results();
    virtual void execute() {

    }
    virtual void printDes() {
        dio->write(description);
    }
};

class uploadAndAnalyze: public Command {
    string description = "5. upload anomalies and analyze results\n";
public:
    uploadAndAnalyze();
    virtual void execute() {

    }
    virtual void printDes() {
        dio->write(description);
    }
};

class exitProgram: public Command {
    string description = "6. exit\n";
public:
    exitProgram();
    virtual void execute() {

    }
    virtual void printDes() {
        dio->write(description);
    }
};

#endif /* COMMANDS_H_ */
