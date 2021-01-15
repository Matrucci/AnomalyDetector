#include "CLI.h"

CLI::CLI(DefaultIO* dio) {

    this->dio = dio;
}

void CLI::start(){
    int size = 6;
    float option;
    bool run = true;
    Command** menu = new Command*[size];
    UploadTS *uploadTs = new UploadTS(dio);
    Settings *settings = new Settings(dio);
    DetectAnomalies *detectAnomalies = new DetectAnomalies(dio);
    detectAnomalies->setThreshold(settings->getThreshold());
    Results *results = new Results(dio);
    results->setReport(detectAnomalies->getAnomalies());
    UploadAndAnalyze *uploadAndAnalyze = new UploadAndAnalyze(dio);
    uploadAndAnalyze->setReport(detectAnomalies->getAnomalies());
    uploadAndAnalyze->setRows(uploadTs->getRows());
    menu[0] = uploadTs;
    menu[1] = settings;
    menu[2] = detectAnomalies;
    menu[3] = results;
    menu[4] = uploadAndAnalyze;
    menu[5] = new ExitProgram(dio);
    //this->commands = *menu;
    while (run) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        //cout << "TEST" << endl;
        for (int i = 0; i < size; i++) {
            menu[i]->printDes();
        }
        dio->read(&option);
        if (floorf(option) == option && option > 0 && option < 7) {
            int iOption = int(option);
            if (iOption == 6) {
                run = false;
                break;
            }
            menu[iOption - 1]->execute();
        }
        else {
            break;
        }
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

