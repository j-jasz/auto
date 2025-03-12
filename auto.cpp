#include <ncurses.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

// Add tabs
const std::string tabNames[] = {
    "Tab 1",
    "Tab 2",
    "Templates",
};

// Add commands
std::vector<std::string> commands1 = {
    "sudo apt update",
    "Cmd2",
    "Cmd3",
    "Cmd4"
};
std::vector<std::string> commands2 = {
    "Cmd5",
    "Cmd6"
};
std::vector<std::string> commands3 = {
    "Cmd7",
    "Cmd8",
    "sudo apt update"
};

// Structure to hold tab and command data
struct TabData {
    std::vector<std::string> tabs;
    std::vector<std::vector<std::string>> commands;
};

// Function to initialize tab data
TabData initTabData(int numTabs, int numCommandSets) {
    TabData data;
    data.tabs.resize(numTabs);
    data.commands.resize(numCommandSets);
    return data;
}

// Function to add a tab
void addTab(TabData &data, int tabIndex, const std::string &tabName) {
    data.tabs[tabIndex] = tabName;
}

// Function to add commands to a command set
void addCommands(TabData &data, int commandSetIndex, const std::vector<std::string> &commands) {
    data.commands[commandSetIndex] = commands;
}

// Function to draw tabs
void drawTabs(WINDOW *leftWin, const TabData &data, int selectedTab) {
    werase(leftWin);
    for (int i = 0; i < data.tabs.size(); i++) {
        if (i == selectedTab) {
            wattron(leftWin, A_REVERSE);
        }
        mvwprintw(leftWin, i, 1, "%s", data.tabs[i].c_str());
        if (i == selectedTab) {
            wattroff(leftWin, A_REVERSE);
        }
    }
    wrefresh(leftWin);
}

// Function to draw commands
void drawCommands(WINDOW *rightWin, const TabData &data, int selectedTab, int selectedCommand) {
    werase(rightWin);
    for (int i = 0; i < data.commands[selectedTab].size(); i++) {
        if (i == selectedCommand) {
            wattron(rightWin, A_REVERSE);
        }
        mvwprintw(rightWin, i, 1, "%s", data.commands[selectedTab][i].c_str());
        if (i == selectedCommand) {
            wattroff(rightWin, A_REVERSE);
        }
    }
    wrefresh(rightWin);
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    int height = LINES;
    int leftWidth = 20;
    int rightWidth = COLS - leftWidth;

    WINDOW *leftWin = newwin(height, leftWidth, 0, 0);
    WINDOW *rightWin = newwin(height, rightWidth, 0, leftWidth);

    keypad(leftWin, TRUE);
    keypad(rightWin, TRUE);

    // Initialize a TabData object with 3 tabs and 3 command sets.
    TabData data = initTabData(3, 3);

    // Add tabs
    for (int i = 0; i < data.tabs.size(); i++) {
        addTab(data, i, tabNames[i]);
    }

    // Add commands to each tab.
    addCommands(data, 0, commands1);
    addCommands(data, 1, commands2);
    addCommands(data, 2, commands3);

    // Initialize the initial state of the tab and command selection.
    int selectedTab = 0;
    int selectedCommand = 0;

    // DO NOT REMOVE
    refresh();

    // Initial draw
    drawTabs(leftWin, data, selectedTab);
    drawCommands(rightWin, data, selectedTab, selectedCommand);

    wrefresh(leftWin);
    wrefresh(rightWin);

    //~ napms(2000); // Optional delay to observe initial draw

    // Key handling
    int c;
    while ((c = getch()) != 'q') {
        if (c == '\t') {
            selectedTab = (selectedTab + 1) % data.tabs.size();
            selectedCommand = 0;
            drawTabs(leftWin, data, selectedTab);
            drawCommands(rightWin, data, selectedTab, selectedCommand);
            wrefresh(leftWin);
            wrefresh(rightWin);
        } else if (c == KEY_UP) {
            selectedCommand = (selectedCommand - 1 + data.commands[selectedTab].size()) % data.commands[selectedTab].size();
            drawCommands(rightWin, data, selectedTab, selectedCommand);
            wrefresh(rightWin);
        } else if (c == KEY_DOWN) {
            selectedCommand = (selectedCommand + 1) % data.commands[selectedTab].size();
            drawCommands(rightWin, data, selectedTab, selectedCommand);
            wrefresh(rightWin);
        } else if (c == '\n') { // Enter key pressed
            if (selectedTab == data.tabs.size() - 1) { // Last tab (Templates)
                endwin(); // Exit ncurses mode permanently
                std::string command = data.commands[selectedTab][selectedCommand];

                // Create a script in /tmp
                std::string scriptPath = "/tmp/ncurses_script.sh";
                std::ofstream scriptFile(scriptPath);

                if (scriptFile.is_open()) {
                    // Write the script content
                    scriptFile << "#!/bin/bash\n";
                    scriptFile << "text=\"" << command << "\"\n";
                    scriptFile << "read -e -p \"\" -i \"$text\" edited_text\n";
                    scriptFile << "eval \"$edited_text\"\n";
                    scriptFile << "exit\n";  // Ensure the shell exits cleanly after execution
                    scriptFile.close();

                    // Make the script executable
                    std::string chmodCmd = "chmod +x " + scriptPath;
                    system(chmodCmd.c_str());

                    // Run the script
                    std::string runCmd = "./" + scriptPath.substr(scriptPath.find_last_of('/') + 1);
                    system(("cd /tmp && " + runCmd).c_str());

                    // Optionally remove the script
                    std::string rmCmd = "rm " + scriptPath;
                    system(rmCmd.c_str());
                } else {
                    std::cerr << "Failed to create script file." << std::endl;
                }
                return 0; // Exit the program
            } else { // Execute command directly for non-template tabs
                endwin(); // Exit ncurses mode
                std::string command = data.commands[selectedTab][selectedCommand];
                system(command.c_str()); // Execute the command in the terminal
                return 0; // Exit the program
            }
        }
    }

    delwin(leftWin);
    delwin(rightWin);

    endwin();
    return 0;
}


