#include "draw.hpp"
#include "records.hpp"
#include "colors.hpp"

#include <ncurses.h>
#include <cmath>

// Function to draw Animation
void drawAnim(WINDOW *AnimWin) {
    werase(AnimWin);
    wbkgd(AnimWin, COLOR_PAIR(C_ANIM));
    box(AnimWin, 0, 0);
}

// Function to draw Tabs
void drawTabs(WINDOW *TabsWin, const TabData &data, int selectedTab) {
    werase(TabsWin);
    wbkgd(TabsWin, COLOR_PAIR(C_MENU));
    box(TabsWin, 0, 0);

    // Define the column width and margin
    const int columnWidth = 10; // 7 characters for the longest label + 3 for margin
    const int maxRows = TabsWin->_maxy - 1; // Subtract borders - this adds top margin

    // Print tab names inside the window, avoiding the borders
    for (int i = 0; i < data.tabNames.size(); i++) {
        int row = i % maxRows + 1; // Sequentially fill rows
        int col = (i / maxRows) * columnWidth + 4; // Move to next column after filling rows

        if (row > maxRows) break; // Stop if we exceed the window height

        int colorPair = (i == selectedTab) ? C_SEL_TAB : C_TABS;
        wattron(TabsWin, COLOR_PAIR(colorPair));
        mvwprintw(TabsWin, row, col, "%-7s", data.tabNames[i].c_str());
        wattroff(TabsWin, COLOR_PAIR(colorPair));
    }
}

// Helper function to get the color pair based on record type and selection status
int getColorPair(char type, bool isSelected) {
    if (isSelected) {
        return C_SEL_COM;
    } else {
        switch (type) {
            case 'S':
                return C_REC_S;
            case 'T':
                return C_REC_T;
            default:
                return C_REC_C;
        }
    }
}

// Helper function to wrap and print a label within a given width
void wrapAndPrintLabel(WINDOW *win, int lineIndex, int startX, int width, const std::string &label, bool highlight) {
    size_t pos = 0;
    while (pos < label.size()) {
        size_t nextPos = label.find('\n', pos);
        if (nextPos == std::string::npos) {
            nextPos = label.size();
        }
        std::string line = label.substr(pos, nextPos - pos);
        // Wrap the line if it exceeds the width
        if (line.size() > width) {
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
        // Print the line with optional highlighting
        if (highlight) {
            wattron(win, COLOR_PAIR(C_SEL_COM));
        }
        mvwprintw(win, lineIndex, startX, "%s", line.c_str());
        if (highlight) {
            wattroff(win, COLOR_PAIR(C_SEL_COM));
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
    wbkgd(RecWin, COLOR_PAIR(C_REC)); // Set background for this specific window
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

            // Print record type with appropriate color
            int colorPair = getColorPair(data.tabRecords[selectedTab][i].type, i == selectedRecord);
            wattron(RecWin, COLOR_PAIR(colorPair));
            mvwprintw(RecWin, lineIndex, 6, "[%c]  ", data.tabRecords[selectedTab][i].type);
            wattroff(RecWin, COLOR_PAIR(colorPair));

            // Print record label
            std::string label = data.tabRecords[selectedTab][i].label;
            int labelStartX = 11; // Starting x position for label
            bool highlight = (i == selectedRecord); // Highlight if selected

            // Check for sufficient space to print the label
            if (width - labelStartX > 0) {
                wattron(RecWin, COLOR_PAIR(C_COM));
                wrapAndPrintLabel(RecWin, lineIndex, labelStartX, width - labelStartX, label, highlight);
                wattroff(RecWin, COLOR_PAIR(C_COM));

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
                wattron(RecWin, COLOR_PAIR(C_SEC));
                wrapAndPrintLabel(RecWin, lineIndex, labelStartX, width - labelStartX, label);
                wattroff(RecWin, COLOR_PAIR(C_SEC));

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
    wbkgd(CommWin, COLOR_PAIR(C_COMM)); // Set background for this specific window
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
