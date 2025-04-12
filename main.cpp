#include "data.hpp"

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

// Get HOME EVVAR
std::string getHomeDir() {
    const char* homeDir = std::getenv("HOME");
    return homeDir ? std::string(homeDir) : std::string();
}

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

// Function to draw Animation
void drawAnim(WINDOW *AnimWin) {
    werase(AnimWin);
    box(AnimWin, 0, 0);
}

// Function to draw Tabs
void drawTabs(WINDOW *TabsWin, const TabData &data, int selectedTab) {
    werase(TabsWin);
    // Redraw borders
    box(TabsWin, 0, 0);
    // Define the column width and margin
    const int columnWidth = 10; // 7 characters for the longest label + 3 for margin
    const int maxRows = TabsWin->_maxy - 1; // Subtract borders - this adds top margin
    // Print tab names inside the window, avoiding the borders
    for (int i = 0; i < data.tabNames.size(); i++) {
        int row = i % maxRows + 1; // Sequentially fill rows
        int col = (i / maxRows) * columnWidth + 4; // Move to next column after filling rows
        if (row > maxRows) break; // Stop if we exceed the window height
        if (i == selectedTab) {
            wattron(TabsWin, A_REVERSE); // Highlight selected tab
        }
        // Print tab name starting from row 1 to avoid overwriting borders
        mvwprintw(TabsWin, row, col, "%-7s", data.tabNames[i].c_str());
        if (i == selectedTab) {
            wattroff(TabsWin, A_REVERSE); // Remove highlight from selected tab
        }
    }
}

// Helper function to wrap and print a label within a given width
void wrapAndPrintLabel(WINDOW *win, int lineIndex, int startX, int width, const std::string &label, bool highlight = false) {
    size_t pos = 0;
    while (pos < label.size()) {
        size_t nextPos = label.find('\n', pos); // Check for newline
        if (nextPos == std::string::npos) {
            nextPos = label.size();
        }
        std::string line = label.substr(pos, nextPos - pos);
        if (line.size() > width) {
            // Split line into multiple parts if it's too long
            size_t splitPos = width;
            while (splitPos > 0 && !isspace(label[pos + splitPos])) {
                splitPos--;
            }
            if (splitPos == 0) {
                splitPos = width; // Force split if no space found
            }
            line = label.substr(pos, splitPos);
            pos += splitPos;
        } else {
            pos = nextPos;
        }
        // Print the line
        if (highlight) {
            wattron(win, A_REVERSE); // Highlight if needed
        }
        mvwprintw(win, lineIndex, startX, "%s", line.c_str());
        if (highlight) {
            wattroff(win, A_REVERSE); // Remove highlight
        }
        // Move to next line if not at the end of the label
        if (pos < label.size()) {
            lineIndex++;
        }
    }
}

// Function to draw Records with line wrapping
void drawRecords(WINDOW *RecWin, const TabData &data, int selectedTab, int selectedRecord, int offset) {
    // Clear the window's interior (not the borders)
    werase(RecWin);
    box(RecWin, 0, 0); // Redraw borders
    // Calculate available width for text
    int width = getmaxx(RecWin) - 2; // Subtract 2 for border width
    int maxRows = getmaxy(RecWin) - 2; // Reserve last two rows
    int lineIndex = 2; // Starting line index

    // Print records inside the window, avoiding the borders
    for (int i = offset; i < data.tabRecords[selectedTab].size(); i++) {
        if (data.tabRecords[selectedTab][i].type != ' ') {
            // Check if we are near the bottom
            if (lineIndex >= maxRows) break;

            mvwprintw(RecWin, lineIndex, 6, "[%c]  ", data.tabRecords[selectedTab][i].type);

            // Print record label
            std::string label = data.tabRecords[selectedTab][i].label;
            int labelStartX = 11; // Starting x position for label
            bool highlight = (i == selectedRecord); // Highlight if selected

            // Check for sufficient space to print the label
            if (width - labelStartX > 0) {
                wrapAndPrintLabel(RecWin, lineIndex, labelStartX, width - labelStartX, label, highlight);

                // Move to the next line after printing a record
                int linesUsed = std::ceil(label.size() / static_cast<double>(width - labelStartX));
                lineIndex += linesUsed;

                // Check again if we are near the bottom after printing
                if (lineIndex >= maxRows) break;
            } else {
                // Skip drawing if there is no space
                lineIndex++;
            }
        } else { // Print section labels
            // Check if we are near the bottom
            if (lineIndex >= maxRows) break;

            std::string label = data.tabRecords[selectedTab][i].label;
            int labelStartX = 4; // Starting x position for label

            // Check for sufficient space to print the label
            if (width - labelStartX > 0) {
                wrapAndPrintLabel(RecWin, lineIndex, labelStartX, width - labelStartX, label);

                // Move to the next line after printing a record
                int linesUsed = std::ceil(label.size() / static_cast<double>(width - labelStartX));
                lineIndex += linesUsed;

            } else {
                // Skip drawing if there is no space
                lineIndex++;
            }
        }
    }
    // Refresh the window
    wrefresh(RecWin);
}

// Function to draw Comments
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
void drawFullView(WINDOW* AnimWin, WINDOW* TabsWin, WINDOW* RecWin, WINDOW* CommWin, TabData& data, int selectedTab, int selectedRecord, int offset) {
    drawAnim(AnimWin);
    drawTabs(TabsWin, data, selectedTab);
    drawRecords(RecWin, data, selectedTab, selectedRecord, offset);
    drawComments(CommWin, data, selectedTab, selectedRecord);
}
void drawRecordsAndComments(WINDOW* RecWin, WINDOW* CommWin, TabData& data, int selectedTab, int selectedRecord, int offset) {
    drawRecords(RecWin, data, selectedTab, selectedRecord, offset);
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
                // Create a script in ~/.temp
                std::string scriptPath = getHomeDir() + "/.temp/ncurses_script.sh";
                std::ofstream scriptFile(scriptPath);

                if (scriptFile.is_open()) {
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
                    // Make the script executable
                    std::string chmodCmd = "chmod +x " + scriptPath;
                    system(chmodCmd.c_str());
                    // Run the script
                    std::string runCmd = scriptPath;
                    system(runCmd.c_str());
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

                // Create a script in ~/.temp
                std::string scriptPath = getHomeDir() + "/.temp/append_script.sh";
                std::ofstream scriptFile(scriptPath);
                if (scriptFile.is_open()) {
                    // Write the script content
                    scriptFile << "#!/bin/bash\n";
                    scriptFile << "echo \"" << command << "\" >> ~/.bash_history\n";
                    scriptFile << "history -a\n";
                    scriptFile << "exit\n";
                    scriptFile.close();
                    // Make the script executable
                    std::string chmodCmd = "chmod +x " + scriptPath;
                    system(chmodCmd.c_str());
                    // Run the script
                    std::string runCmd = scriptPath;
                    system(runCmd.c_str());
                    // Remove the script
                    std::string rmCmd = "rm " + scriptPath;
                    system(rmCmd.c_str());
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


