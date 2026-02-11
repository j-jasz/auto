
#include "colors.hpp"
#include "draw.hpp"
#include "records.hpp"
#include "script_utils.hpp"
#include "utils.hpp"

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>

int offset = 0;

std::string homeDirectory = getHomeDir();
std::ofstream scriptFile;
std::string scriptPath;

// Fetching all the records from an external source or function
std::vector<Record> allRecords = getAllRecords();
/**
 * @brief Initializes TabData with records categorized by their 'tab' field.
 * This function takes a vector of records, groups them based on the 'tab' field of the Record,
 * and organizes them into a TabData structure. Each unique 'tab' value will have its own list
 * of records within the TabData.
 * @param records The vector of records to categorize.
 * @return TabData The TabData object populated with categorized records.
 */
TabData initTabDataWithRecords(const std::vector<Record> &records) {
    // TabData object that will hold the categorized data
    TabData data;
    // Map to associate each unique 'tab' string to a unique index in the tabRecords vector
    std::unordered_map<std::string, int> tabIndices;
    // Loop over each record to categorize them by their 'tab' field
    for (const auto &record : records) {
        // If the 'tab' value does not exist in the tabIndices map, it's a new tab
        if (tabIndices.find(record.tab) == tabIndices.end()) {
            // Assign a new index to this tab and store it in the map
            int index = tabIndices.size();
            tabIndices[record.tab] = index;
            // Add the tab name to the list of tab names
            data.tabNames.push_back(record.tab);
            // Initialize an empty vector for this tab's records
            data.tabRecords.emplace_back(); // one vector per tab
        }
        // Add the current record to the appropriate tab's vector in tabRecords
        data.tabRecords[tabIndices[record.tab]].push_back(record);
    }
    // Return the populated TabData structure
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
	    endwin();   // End curses mode temporarily
	    refresh();  // Refresh to get new terminal size
	    initscr();  // Restart curses mode

	    // Get new terminal dimensions
	    getmaxyx(stdscr, max_y, max_x);

	    // Recompute row3Height using a const local expression
	    const int newRow3Height = max_y - 26;

	    // Only recreate windows if size actually changed
	    static int prev_max_y = 0;
	    static int prev_max_x = 0;
	    if (max_y != prev_max_y || max_x != prev_max_x) {
		prev_max_y = max_y;
		prev_max_x = max_x;

		wrapDelete(RecWin, AnimWin, TabsWin, CommWin);

		RecWin = newwin(max_y, max_x - 30, 0, 30);
		AnimWin = newwin(13, 30, 0, 0);
		TabsWin = newwin(13, 30, 13, 0);
		CommWin = newwin(newRow3Height, 30, 26, 0);

		wrapBox(RecWin, AnimWin, TabsWin, CommWin);
		drawFullView(AnimWin, TabsWin, RecWin, CommWin, data, selectedTab, selectedRecord, offset);
		wrapRefresh(RecWin, AnimWin, TabsWin, CommWin);
	    }

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

            if (data.tabRecords[selectedTab][selectedRecord].type == 'S') { // Check if the record type is 'S' (for Script)
                endwin(); // Exit ncurses mode permanently before executing the script

                // Get the command string from the selected record
                std::string command = data.tabRecords[selectedTab][selectedRecord].command;

                // Fork a new process to run the script
                pid_t pid = fork();
                if (pid == -1) {
                    // If fork fails, print error and exit
                    perror("fork failed");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // ---- Child process ----
                    // Get and set necessary environment variables for the Xfce session
                    const char *display = getenv("DISPLAY");
                    const char *dbus = getenv("DBUS_SESSION_BUS_ADDRESS");
                    const char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
                    const char *xauth = getenv("XAUTHORITY");

                    // Set environment variables if they are present
                    if (display) setenv("DISPLAY", display, 1);
                    if (dbus) setenv("DBUS_SESSION_BUS_ADDRESS", dbus, 1);
                    if (xdg_runtime_dir) setenv("XDG_RUNTIME_DIR", xdg_runtime_dir, 1);
                    if (xauth) setenv("XAUTHORITY", xauth, 1);

                    // Execute the command via the shell (`/bin/sh`) to handle shell-specific syntax (pipes, redirects, etc.)
                    // This allows the command to be interpreted like it would be in a terminal.
                    execl("/bin/sh", "sh", "-c", command.c_str(), (char *)nullptr);

                    // If exec fails, this code will be executed
                    perror(("Failed to execute: " + command).c_str());
                    _exit(EXIT_FAILURE); // Exit the child process with a failure status
                } else {
                    // ---- Parent process ----
                    // Parent waits for the child process to finish
                    int status;
                    // Handle interrupted system calls (EINTR)
                    while (waitpid(pid, &status, 0) == -1 && errno == EINTR);

                    // Check if the child process exited normally
                    if (WIFEXITED(status)) {
                        std::cout << "Script exited with status " << WEXITSTATUS(status) << "\n";
                    } else {
                        std::cout << "Script terminated abnormally\n";
                    }
                }

                return 0; // Exit the program

            } else if (data.tabRecords[selectedTab][selectedRecord].type == 'T') { // Templates

                endwin(); // Exit ncurses mode permanently
                std::string command = data.tabRecords[selectedTab][selectedRecord].command;
                std::cout << std::endl; // Print empty line
                createScriptFile(scriptFile, scriptPath);

		// Immediately check if file creation succeeded
		if (!scriptFile.is_open()) {
		    endwin(); // Ensure curses is shut down properly
		    std::cerr << "Error: Could not open script file at path " << scriptPath << std::endl;
		    return 1; // Abort the program early
		}

		// Only continue if valid
		appendBashHistoryT(scriptFile, command);
		executeAndRemoveScript(scriptPath);

                return 0; // Exit the program

            } else { // Commands

                endwin(); // Exit ncurses mode
                std::string command = data.tabRecords[selectedTab][selectedRecord].command;
                system(command.c_str()); // Execute the command in the terminal
                createScriptFile(scriptFile, scriptPath);

		// Immediately check if file creation succeeded
		if (!scriptFile.is_open()) {
		    endwin(); // Ensure curses is shut down properly
		    std::cerr << "Error: Could not open script file at path " << scriptPath << std::endl;
		    return 1; // Abort the program early
		}

		// Only continue if valid
		appendBashHistoryC(scriptFile, command);
		executeAndRemoveScript(scriptPath);

                return 0; // Exit the program
            }
        }
    }

    wrapDelete(RecWin, AnimWin, TabsWin, CommWin);

    endwin();
    return 0;
}


