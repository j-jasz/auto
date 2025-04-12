#include "records.hpp"
#include "draw.hpp"
#include "utils.hpp"
#include "script_utils.hpp"
#include "colors.hpp"

#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>

int offset = 0;

std::string homeDirectory = getHomeDir();
std::ofstream scriptFile;
std::string scriptPath;

std::vector<Record> allRecords = getAllRecords();

int determineNumTabs(const std::vector<Record> &records) {
    std::unordered_map<std::string, int> tabIndices;
    int numTabs = 0;
    for (const auto &record : records) {
        if (tabIndices.find(record.tab) == tabIndices.end()) {
            tabIndices[record.tab] = numTabs++;
        }
    }
    return numTabs;
}

TabData initTabData(int numTabs) {
    TabData data;
    data.tabNames.resize(numTabs);
    data.tabRecords.resize(numTabs);
    return data;
}

void assignRecordsToTabs(TabData &data, const std::vector<Record> &records, const std::unordered_map<std::string, int> &tabIndices) {
    for (const auto &record : records) {
        int tabIndex = tabIndices.at(record.tab);
        data.tabRecords[tabIndex].push_back(record);
    }
}

TabData initTabDataWithRecords(const std::vector<Record> &records) {
    std::unordered_map<std::string, int> tabIndices;
    int numTabs = determineNumTabs(records);
    TabData data = initTabData(numTabs);
    int tabIndex = 0;
    for (const auto &record : records) {
        if (tabIndices.find(record.tab) == tabIndices.end()) {
            tabIndices[record.tab] = tabIndex++;
            data.tabNames[tabIndices[record.tab]] = record.tab;
        }
    }
    assignRecordsToTabs(data, records, tabIndices);
    return data;
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

int main() {
    initscr();
    initColors();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    refresh();

    // Get the dimensions of the terminal
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    // Create windows
    WINDOW *RecWin = newwin(max_y, max_x - 30, 0, 30);
    WINDOW *AnimWin = newwin(13, 30, 0, 0);
    WINDOW *TabsWin = newwin(13, 30, 13, 0);
    int row3Height = max_y - 26; // Assuming row1 and row2 take up 26 lines
    WINDOW *CommWin = newwin(row3Height, 30, 26, 0);

    wrapBox(RecWin, AnimWin, TabsWin, CommWin);
    wrapRefresh(RecWin, AnimWin, TabsWin, CommWin);

    keypad(TabsWin, TRUE);
    keypad(RecWin, TRUE);

    TabData data = initTabDataWithRecords(allRecords);

    // Initialize the initial state of the tab and command selection.
    int selectedTab = 0;
    int selectedRecord = 1;

    // Initial draw
    drawFullView(AnimWin, TabsWin, RecWin, CommWin, data, selectedTab, selectedRecord, offset);
    wrapRefresh(RecWin, AnimWin, TabsWin, CommWin);

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
            wrapDelete(RecWin, AnimWin, TabsWin, CommWin);

            RecWin = newwin(max_y, max_x - 30, 0, 30);
            AnimWin = newwin(13, 30, 0, 0);
            TabsWin = newwin(13, 30, 13, 0);
            row3Height = max_y - 26; // Update row3Height
            CommWin = newwin(row3Height, 30, 26, 0);

            wrapBox(RecWin, AnimWin, TabsWin, CommWin);
            drawFullView(AnimWin, TabsWin, RecWin, CommWin, data, selectedTab, selectedRecord, offset);
            wrapRefresh(RecWin, AnimWin, TabsWin, CommWin);
        // For the tab key
        } else if (c == '\t') {
            if (selectedTab == data.tabNames.size() - 1) {
                // Move to the first tab
                selectedTab = 0;
            } else {
                selectedTab++;
            }
            selectedRecord = 1;
            drawFullView(AnimWin, TabsWin, RecWin, CommWin, data, selectedTab, selectedRecord, offset);
            wrapRefresh(RecWin, TabsWin, CommWin);
        // For the backtick (`) key
        } else if (c == '`') {
            if (selectedTab == 0) {
                // Move to the last tab
                selectedTab = data.tabNames.size() - 1;
            } else {
                selectedTab--;
            }
            selectedRecord = 1;
            drawFullView(AnimWin, TabsWin, RecWin, CommWin, data, selectedTab, selectedRecord, offset);
            wrapRefresh(RecWin, TabsWin, CommWin);
        } else if (c == KEY_UP) {
            do {
                selectedRecord = (selectedRecord - 1 + data.tabRecords[selectedTab].size()) % data.tabRecords[selectedTab].size();
            } while (data.tabRecords[selectedTab][selectedRecord].type == ' '); // Skip spacers
            // Check if we are at the first record in the list
            if (selectedRecord == 1) {
                // Scroll up by two lines if possible
                if (offset > 1) {
                    offset -= 2;
                } else {
                    offset = 0; // Ensure offset doesn't go negative
                }
            } else {
                // Adjust offset to ensure selectedRecord is visible
                if (selectedRecord < offset) {
                    offset = selectedRecord; // Scroll up to show the selected record
                } else if (selectedRecord >= offset + (getmaxy(RecWin) - 4)) {
                    offset = selectedRecord - (getmaxy(RecWin) - 4) + 1; // Adjust for top and bottom reserved rows
                }
            }
            drawRecordsAndComments(RecWin, CommWin, data, selectedTab, selectedRecord, offset);
            wrapRefresh(RecWin, CommWin);
        } else if (c == KEY_DOWN) {
            do {
                selectedRecord = (selectedRecord + 1) % data.tabRecords[selectedTab].size();
            } while (data.tabRecords[selectedTab][selectedRecord].type == ' '); // Skip spacers
            // Adjust offset to ensure selectedRecord is visible
            if (selectedRecord >= offset + (getmaxy(RecWin) - 4)) {
                offset = selectedRecord - (getmaxy(RecWin) - 4) + 1; // Scroll down to show the selected record
            } else if (selectedRecord < offset) {
                offset = selectedRecord; // Adjust for reverse scrolling
            }
            if (selectedRecord == 1) {
                offset = std::max(0, offset - 1); // Move offset up by one line
            }
            drawRecordsAndComments(RecWin, CommWin, data, selectedTab, selectedRecord, offset);
            wrapRefresh(RecWin, CommWin);
        } else if (c == '\n') { // Enter key pressed
            if (data.tabRecords[selectedTab][selectedRecord].type == 'T') {
                endwin(); // Exit ncurses mode permanently
                std::string command = data.tabRecords[selectedTab][selectedRecord].command;
                std::cout << std::endl; // Print empty line
                createScriptFile(scriptFile, scriptPath);
                if (scriptFile.is_open()) {
                    appendBashHistoryT(scriptFile, command);
                    executeAndRemoveScript(scriptPath);
                } else {
                    std::cerr << "Failed to create script file." << std::endl;
                }
                return 0; // Exit the program
            } else { // Execute command directly for non-template tabs
                endwin(); // Exit ncurses mode
                std::string command = data.tabRecords[selectedTab][selectedRecord].command;
                system(command.c_str()); // Execute the command in the terminal
                createScriptFile(scriptFile, scriptPath);
                if (scriptFile.is_open()) {
                    appendBashHistoryC(scriptFile, command);
                    executeAndRemoveScript(scriptPath);
                } else {
                    std::cerr << "Failed to create script file." << std::endl;
                }
                return 0; // Exit the program
            }
        }
    }

    wrapDelete(RecWin, AnimWin, TabsWin, CommWin);

    endwin();
    return 0;
}


