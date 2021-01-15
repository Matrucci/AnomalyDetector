#include "CLI.h"

CLI::CLI(DefaultIO* dio) {

    this->dio = dio;
}

void CLI::start(){
    int size = 6;
    float option;
    bool run = true;
    Command** menu = new Command*[size];
    menu[0] = new uploadTS(dio);
    menu[1] = new settings(dio);
    menu[2] = new detectAnomalies(dio);
    menu[3] = new results(dio);
    menu[4] = new uploadAndAnalyze(dio);
    menu[5] = new exitProgram(dio);
    //this->commands = *menu;
    while (run) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");

        for (int i = 0; i < size; i++) {
            menu[i]->printDes();
        }
        dio->read(&option);
        if (floorf(option) == option && option > 0 && option < 7) {
            int iOption = int(option);
            menu[iOption - 1]->execute();
            if (iOption == 6) {
                run = false;
            }
        }
        //cout << "DONE" << endl;
    }
    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");

    for (int i = 0; i < size; i++) {
        menu[i]->printDes();
    }
    dio->read(&option);
    if (floorf(option) == option && option > 0 && option < 7) {
        int iOption = int(option);
        menu[iOption - 1]->execute();
    }
}


CLI::~CLI() {
}

