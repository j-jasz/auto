
#include "script_utils.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <fstream>

void createScriptFile(std::ofstream& scriptFile, std::string& scriptPath) {
    scriptPath = getHomeDir() + "/.temp/temp_auto_script.sh";
    scriptFile.open(scriptPath);
}

void appendBashHistoryT(std::ofstream& scriptFile, const std::string& command) {
    // Write the script content
    scriptFile << "#!/bin/bash\n";
    // Use wrapper to capture command history
    scriptFile << "wrapper() {\n";
    scriptFile << "    text=\"$1\"\n";
    scriptFile << "    read -e -p \"\" -i \"$text\" edited_text\n";
    scriptFile << "    echo \"$edited_text\" >> ~/.bash_history\n"; // Append edited command to history
    scriptFile << "    eval \"$edited_text\"\n"; // Execute the command
    scriptFile << "}\n";
    scriptFile << "wrapper \"" << command << "\"\n"; // Call the wrapper function
    scriptFile << "exit\n";  // Ensure the shell exits cleanly after execution
    scriptFile.close();
}

void appendBashHistoryC(std::ofstream& scriptFile, const std::string& command) {
    scriptFile << "#!/bin/bash\n";
    scriptFile << "echo \"" << command << "\" >> ~/.bash_history\n";
    scriptFile << "history -a\n";
    scriptFile << "exit\n";
    scriptFile.close();
}

void executeAndRemoveScript(const std::string& scriptPath) {
    std::string chmodCmd = "chmod +x " + scriptPath;
    system(chmodCmd.c_str());
    system(scriptPath.c_str());
    std::string rmCmd = "rm " + scriptPath;
    system(rmCmd.c_str());
}

