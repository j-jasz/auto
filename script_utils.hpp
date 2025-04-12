
#ifndef SCRIPT_UTILS_HPP
#define SCRIPT_UTILS_HPP

#include <string>
#include <fstream>

// Function declarations
void createScriptFile(std::ofstream& scriptFile, std::string& scriptPath);
void appendBashHistoryT(std::ofstream& scriptFile, const std::string& command);
void appendBashHistoryC(std::ofstream& scriptFile, const std::string& command);
void executeAndRemoveScript(const std::string& scriptPath);

#endif // SCRIPT_UTILS_H
