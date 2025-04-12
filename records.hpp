
#ifndef RECORDS_HPP
#define RECORDS_HPP

#include <string>
#include <vector>

struct Record {
    std::string tab;
    char type;
    std::string label;
    std::string command;
    std::string comment;
    std::string color;
};

struct TabData {
    std::vector<std::string> tabNames;
    std::vector<std::vector<Record>> tabRecords;
};

std::vector<Record> getAllRecords();

#endif
