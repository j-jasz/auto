#include "records.hpp"

#include <ncurses.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

// Add tabs
const std::string tabNames[] = {
    "APT",
    "DESKTOP",
    "BLENDER"
};

// Get all records dynamically
std::vector<Record> allRecords = getAllRecords();

struct TabData {
    std::vector<std::string> tabNames;
    std::vector<std::vector<Record>> tabRecords;
};

// Function to determine the number of tabs based on records
int determineNumTabs(const std::vector<Record> &records) {
    int numTabs = 0;
    for (const auto &record : records) {
        if (record.tab >= numTabs) {
            numTabs = record.tab + 1;
        }
    }
    return numTabs;
}

// Initialize TabData with the correct number of tabs
TabData initTabData(int numTabs) {
    TabData data;
    data.tabNames.resize(numTabs);
    data.tabRecords.resize(numTabs);
    return data;
}

TabData initTabDataWithRecords(const std::vector<Record> &records) {
    return initTabData(determineNumTabs(records));
}

// Function to add a tab
void addTabName(TabData &data, int tabIndex, const std::string &tabName) {
    if (tabIndex >= 0 && tabIndex < data.tabNames.size()) {
        data.tabNames[tabIndex] = tabName;
    }
}

void assignTabNames(TabData &data, const std::string tabNames[], int numTabs) {
    for (int i = 0; i < numTabs; i++) {
        addTabName(data, i, tabNames[i]);
    }
}

// Function to add records to a tab
void addRecordsToTab(TabData &data, int tabIndex, const std::vector<Record> &records) {
    if (tabIndex >= 0 && tabIndex < data.tabRecords.size()) {
        data.tabRecords[tabIndex] = records;
    }
}

void assignRecordsToTabs(TabData &data, const std::vector<Record> &records) {
    for (int i = 0; i < data.tabNames.size(); i++) {
        std::vector<Record> tabRecords;
        for (const auto &record : records) {
            if (record.tab == i) {
                tabRecords.push_back(record);
            }
        }
        addRecordsToTab(data, i, tabRecords);
    }
}

// Function to draw tabs
void drawTabs(WINDOW *TabsWin, const TabData &data, int selectedTab) {
    werase(TabsWin);
    // Redraw borders
    box(TabsWin, 0, 0);
    // Print tab names inside the window, avoiding the borders
    for (int i = 0; i < data.tabNames.size(); i++) {
        if (i == selectedTab) {
            wattron(TabsWin, A_REVERSE); // Highlight selected tab
        }
        // Print tab name starting from row 1 to avoid overwriting borders
        mvwprintw(TabsWin, i + 2, 4, "%s", data.tabNames[i].c_str());
        if (i == selectedTab) {
            wattroff(TabsWin, A_REVERSE); // Remove highlight from selected tab
        }
    }
}

// Function to draw records
void drawRecords(WINDOW *RightWin, const TabData &data, int selectedTab, int selectedRecord) {
    // Clear the window's interior (not the borders)
    werase(RightWin);
    // Redraw borders
    box(RightWin, 0, 0);
    // Print records inside the window, avoiding the borders
    for (int i = 0; i < data.tabRecords[selectedTab].size(); i++) {
        if (data.tabRecords[selectedTab][i].type != ' ') {
            // Print type without highlighting
            mvwprintw(RightWin, i + 2, 6, "%c ", data.tabRecords[selectedTab][i].type);
            //~ mvwprintw(RightWin, i + 2, 4, "%c ", data.tabRecords[selectedTab][i].type);
            if (i == selectedRecord) {
                wattron(RightWin, A_REVERSE); // Highlight selected record
            }
            // Print label with highlighting if selected
            mvwprintw(RightWin, i + 2, 9, "%s", data.tabRecords[selectedTab][i].label.data());
            if (i == selectedRecord) {
                wattroff(RightWin, A_REVERSE); // Remove highlight from selected record
            }
        } else {
            // If type is ' ', print label with offset without highlighting
            mvwprintw(RightWin, i + 2, 4, "%s", data.tabRecords[selectedTab][i].label.data());
        }
    }
}
// Function to draw comments
void drawComments(WINDOW *CommWin, const TabData &data, int selectedTab, int selectedRecord) {
    // Clear the window's interior (not the borders)
    werase(CommWin);
    // Redraw borders
    box(CommWin, 0, 0);
    // Get the dimensions of the window
    int max_y, max_x;
    getmaxyx(CommWin, max_y, max_x);
    // Adjust max_x to account for the border and margin
    max_x -= 2; // Subtract 2 for both borders
    // Print the comment with wrapping
    std::string comment = data.tabRecords[selectedTab][selectedRecord].comment;
    int y = 1; // Start from the second line to avoid the top border
    int x = 1; // Start from the second column to avoid the left border
    while (!comment.empty()) {
        // Find the next character that fits on this line
        size_t nextChar = std::min<size_t>(static_cast<size_t>(max_x - x), comment.size());
        // Print the part of the comment that fits on this line
        std::string line = comment.substr(0, nextChar);
        mvwprintw(CommWin, y, x, "%s", line.c_str());
        // Move to the next line
        y++;
        x = 1; // Reset x for the next line
        // Remove the printed part from the comment
        comment.erase(0, nextChar);
        // Check if we've reached the bottom of the window
        if (y >= max_y - 1) {
            break; // Stop printing if we reach the bottom border
        }
    }
}

// Templates and wrapper functions
template<typename... Args>
void wrapRefresh(Args... args) {
    (wrefresh(args), ...);
}
template<typename... Args>
void wrapDelete(Args... args) {
    (delwin(args), ...);
}
template<typename... Args>
void wrapBox(Args... args) {
    (box(args, 0, 0), ...);
}
void drawFullView(WINDOW* TabsWin, WINDOW* RightWin, WINDOW* CommWin, TabData& data, int selectedTab, int selectedRecord) {
    drawTabs(TabsWin, data, selectedTab);
    drawRecords(RightWin, data, selectedTab, selectedRecord);
    drawComments(CommWin, data, selectedTab, selectedRecord);
}
void drawRecordsAndComments(WINDOW* RightWin, WINDOW* CommWin, TabData& data, int selectedTab, int selectedRecord) {
    drawRecords(RightWin, data, selectedTab, selectedRecord);
    drawComments(CommWin, data, selectedTab, selectedRecord);
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    refresh();

    // Get the dimensions of the terminal
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    // Create windows
    WINDOW *RightWin = newwin(max_y, max_x - 30, 0, 30);
    WINDOW *AnimWin = newwin(13, 30, 0, 0);
    WINDOW *TabsWin = newwin(13, 30, 13, 0);
    int row3Height = max_y - 26; // Assuming row1 and row2 take up 26 lines
    WINDOW *CommWin = newwin(row3Height, 30, 26, 0);

    wrapBox(RightWin, AnimWin, TabsWin, CommWin);
    wrapRefresh(RightWin, AnimWin, TabsWin, CommWin);

    keypad(TabsWin, TRUE);
    keypad(RightWin, TRUE);

    TabData data = initTabDataWithRecords(allRecords);
    assignTabNames(data, tabNames, data.tabNames.size());
    assignRecordsToTabs(data, allRecords);

    // Initialize the initial state of the tab and command selection.
    int selectedTab = 0;
    int selectedRecord = 1;

    // Initial draw
    drawFullView(TabsWin, RightWin, CommWin, data, selectedTab, selectedRecord);

    wrapRefresh(RightWin, AnimWin, TabsWin, CommWin);

    //~ napms(2000); // Optional delay to observe initial draw

    // Key handling
    int c;
    while ((c = getch()) != 'q') {
        if (c == KEY_RESIZE) {
            // Handle resize
            endwin(); // End curses mode temporarily
            refresh(); // Refresh to get new terminal size
            initscr(); // Restart curses mode

            // Get new terminal dimensions
            getmaxyx(stdscr, max_y, max_x);
            // Recreate windows with new dimensions
            wrapDelete(RightWin, AnimWin, TabsWin, CommWin);

            RightWin = newwin(max_y, max_x - 30, 0, 30);
            AnimWin = newwin(13, 30, 0, 0);
            TabsWin = newwin(13, 30, 13, 0);
            row3Height = max_y - 26; // Update row3Height
            CommWin = newwin(row3Height, 30, 26, 0);

            wrapBox(RightWin, AnimWin, TabsWin, CommWin);
            drawFullView(TabsWin, RightWin, CommWin, data, selectedTab, selectedRecord);
            wrapRefresh(RightWin, AnimWin, TabsWin, CommWin);
        } else if (c == '\t') {
            selectedTab = (selectedTab + 1) % data.tabNames.size();
            selectedRecord = 1;
            drawFullView(TabsWin, RightWin, CommWin, data, selectedTab, selectedRecord);
            wrapRefresh(RightWin, TabsWin, CommWin);
        } else if (c == '`') {
            selectedTab = (selectedTab - 1 + data.tabNames.size()) % data.tabNames.size();
            selectedRecord = 1;
            drawFullView(TabsWin, RightWin, CommWin, data, selectedTab, selectedRecord);
            wrapRefresh(RightWin, TabsWin, CommWin);
        } else if (c == KEY_UP) {
            do {
                selectedRecord = (selectedRecord - 1 + data.tabRecords[selectedTab].size()) % data.tabRecords[selectedTab].size();
            } while (data.tabRecords[selectedTab][selectedRecord].type == ' '); // Skip spacers
            drawRecordsAndComments(RightWin, CommWin, data, selectedTab, selectedRecord);
            wrapRefresh(RightWin, CommWin);
        } else if (c == KEY_DOWN) {
            do {
                selectedRecord = (selectedRecord + 1) % data.tabRecords[selectedTab].size();
            } while (data.tabRecords[selectedTab][selectedRecord].type == ' '); // Skip spacers
            drawRecordsAndComments(RightWin, CommWin, data, selectedTab, selectedRecord);
            wrapRefresh(RightWin, CommWin);
        } else if (c == '\n') { // Enter key pressed
            if (data.tabRecords[selectedTab][selectedRecord].type == 'T') {
                endwin(); // Exit ncurses mode permanently
                std::cout << std::endl; // Print empty line
                std::string command = data.tabRecords[selectedTab][selectedRecord].command;
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
                    // Remove the script
                    std::string rmCmd = "rm " + scriptPath;
                    system(rmCmd.c_str());
                } else {
                    std::cerr << "Failed to create script file." << std::endl;
                }
                return 0; // Exit the program
            } else { // Execute command directly for non-template tabs
                endwin(); // Exit ncurses mode
                std::string command = data.tabRecords[selectedTab][selectedRecord].command;
                system(command.c_str()); // Execute the command in the terminal
                return 0; // Exit the program
            }
        }
    }

    wrapDelete(RightWin, AnimWin, TabsWin, CommWin);

    endwin();
    return 0;
}


