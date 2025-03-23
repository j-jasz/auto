#ifndef RECORDS_H
#define RECORDS_H

#include <string>
#include <vector>

struct Record {
    int tab;
    char type;
    std::string label;
    std::string command;
    std::string comment;
    std::string color;
};

std::vector<Record> getAllRecords();

#endif
