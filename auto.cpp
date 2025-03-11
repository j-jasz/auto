#include <ncurses.h>
#include <vector>
#include <string>
//~ #include <cstdlib>  // For system()
//~ #include <unistd.h> // For fork() and exec()
//~ #include <sys/types.h>  // For pid_t
//~ #include <sys/wait.h>   // For wait()

// needed for dynamic row amount
#include <stdlib.h>
#include <string.h>

// Add tabs
const char *tabNames[] = {
    "Utils",
    "Image",
    "Blender",
    "Stable Diffusion",
    "Video",
    "PDFs",
    "Sound",
    "Templates"
};

 // Add commands for each tab
const char *commands1[] = {"Cmd1", "Cmd2", "test1"};
const char *commands2[] = {"Cmd4", "Cmd5test2"};
const char *commands3[] = {"Cmd6", "Cmd7", "sertghCmd8", "Cmd9"};
const char *commands4[] = {"Cmd10"};
const char *commands5[] = {"Cmd11", "Cmd12", "Cmeargd13", "Cmd14", "Cmd15"};
const char *commands6[] = {"Cmdaerg16"};
const char *commands7[] = {"Cmd17", "Cmdh4t5ty518"};
const char *commands8[] = {"Cmd19", "Cmd20", "Cm35efefd21"};

// Structure to hold tab and command data
typedef struct {
    char **tabs;
    int numTabs;
    char ***commands;
    int *numCommands;
    int numCommandSets;
} TabData;

// Function to initialize tab data
TabData *initTabData(int numTabs, int numCommandSets) {
    TabData *data = new TabData;
    data->tabs = new char *[numTabs];
    data->numTabs = numTabs;
    data->commands = new char **[numCommandSets];
    data->numCommands = new int[numCommandSets];
    data->numCommandSets = numCommandSets;

    return data;
}

// Function to add a tab
void addTab(TabData *data, int tabIndex, const char *tabName) {
    data->tabs[tabIndex] = strdup(tabName); // Allocate memory for the tab name
}

// Function to add commands to a command set
void addCommands(TabData *data, int commandSetIndex, int numCommands, const char **commands) {
    data->commands[commandSetIndex] = new char *[numCommands];
    data->numCommands[commandSetIndex] = numCommands;
    for (int i = 0; i < numCommands; i++) {
        data->commands[commandSetIndex][i] = strdup(commands[i]);
    }
}

// Function to draw tabs
void drawTabs(WINDOW *leftWin, TabData *data, int selectedTab) {
    werase(leftWin); // Clear the window
    for (int i = 0; i < data->numTabs; i++) {
        if (i == selectedTab) {
            wattron(leftWin, A_REVERSE); // Highlight selected tab
        }
        mvwprintw(leftWin, i, 1, "%s", data->tabs[i]);
        if (i == selectedTab) {
            wattroff(leftWin, A_REVERSE); // Turn off highlighting
        }
    }
    wrefresh(leftWin); // Refresh the window
}

// Function to draw commands
void drawCommands(WINDOW *rightWin, TabData *data, int selectedTab, int selectedCommand) {
    werase(rightWin); // Clear the window
    for (int i = 0; i < data->numCommands[selectedTab]; i++) {
        if (i == selectedCommand) {
            wattron(rightWin, A_REVERSE); // Highlight selected command
        }
        mvwprintw(rightWin, i, 1, "%s", data->commands[selectedTab][i]);
        if (i == selectedCommand) {
            wattroff(rightWin, A_REVERSE); // Turn off highlighting
        }
    }
    wrefresh(rightWin); // Refresh the window
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

    TabData *data = initTabData(8, 8); // Initialize with 8 tabs and command sets

    // Add tabs
    for (int i = 0; i < data->numTabs; i++) {
        addTab(data, i, tabNames[i]);
    }

    addCommands(data, 0, sizeof(commands1) / sizeof(commands1[0]), commands1);
    addCommands(data, 1, sizeof(commands2) / sizeof(commands2[0]), commands2);
    addCommands(data, 2, sizeof(commands3) / sizeof(commands3[0]), commands3);
    addCommands(data, 3, sizeof(commands4) / sizeof(commands4[0]), commands4);
    addCommands(data, 4, sizeof(commands5) / sizeof(commands5[0]), commands5);
    addCommands(data, 5, sizeof(commands6) / sizeof(commands6[0]), commands6);
    addCommands(data, 6, sizeof(commands7) / sizeof(commands7[0]), commands7);
    addCommands(data, 7, sizeof(commands8) / sizeof(commands8[0]), commands8);

    // Initial Tab and Cmd state
    int selectedTab = 0;
    int selectedCommand = 0;

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
            selectedTab = (selectedTab + 1) % data->numTabs;
            selectedCommand = 0; // Reset command selection
            drawTabs(leftWin, data, selectedTab);
            drawCommands(rightWin, data, selectedTab, selectedCommand);
        } else if (c == KEY_UP) {
            selectedCommand = (selectedCommand - 1 + data->numCommands[selectedTab]) % data->numCommands[selectedTab];
            drawCommands(rightWin, data, selectedTab, selectedCommand);
        } else if (c == KEY_DOWN) {
            selectedCommand = (selectedCommand + 1) % data->numCommands[selectedTab];
            drawCommands(rightWin, data, selectedTab, selectedCommand);
        }
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < data->numTabs; i++) {
        delete[] data->tabs[i]; // Assuming tabs[i] was allocated with new[]
    }
    delete[] data->tabs;

    for (int i = 0; i < data->numCommandSets; i++) {
        for (int j = 0; j < data->numCommands[i]; j++) {
            delete[] data->commands[i][j]; // Assuming commands[i][j] was allocated with new[]
        }
        delete[] data->commands[i];
    }
    delete[] data->commands;
    delete[] data->numCommands;
    delete data;

    endwin();
    return 0;
}


