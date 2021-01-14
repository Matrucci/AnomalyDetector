#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    int menuSize = 7;
    Command* menu[7];
    menu[0] = nullptr;
    menu[1] = new uploadTS;
    menu[2] = new settings;
    menu[3] = new detectAnomalies;
    menu[4] = new results;
    menu[5] = new uploadAndAnalyze;
    menu[6] = new exitProgram;
    this->commands = *menu;
    this->dio = dio;
}

void CLI::start(){
    Command* menu = this->commands;
    for (int i = 1; i <= 6; i++) {
        menu[i].printDes();
    }
    //dio->read()
}


CLI::~CLI() {
}

