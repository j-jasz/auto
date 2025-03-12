#ifndef RECORDS_H
#define RECORDS_H

#include <string>
#include <vector>

struct Record {
    char type;
    std::string label;
    std::string command;
    std::string comment;
    std::string color;
};

Record getSpacer();

Record getRecord1();
Record getRecord2();
Record getRecord3();
Record getRecord4();
Record getRecord5();
Record getRecord6();
Record getRecord7();
Record getRecord8();

#endif
