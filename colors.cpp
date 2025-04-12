
#include "colors.hpp"

#include <ncurses.h>

// Define color constants
const short C_BG = 1;
const short C_ANIM_FG = 2;
const short C_MENU_FG = 3;
const short C_SEL_TAB_BG = 4;
const short C_TAB_FG = 5;
const short C_COMM_FG = 6;
const short C_REC_FG = 7;
const short C_REC_BG = 8;
const short C_S_FG = 9;
const short C_T_FG = 10;
const short C_C_FG = 11;
const short C_SEC_FG = 12;
const short C_COM_FG = 13;
const short C_RED = 14;

// Define color pair constants
const short C_ANIM = 1;
const short C_MENU = 2;
const short C_TABS = 3;
const short C_SEL_TAB = 4;
const short C_COMM = 5;
const short C_REC = 6;
const short C_REC_S = 7;
const short C_REC_T = 8;
const short C_REC_C = 9;
const short C_SEC = 10;
const short C_SEL_COM = 11;
const short C_COM = 12;

void initColors() {
    start_color(); // Enable color support
    // bg
    init_color(C_BG, 106, 106, 106);
    // Anim
    init_color(C_ANIM_FG, 324, 300, 246);
    // Menu left
    init_color(C_MENU_FG, 324, 300, 246);
    init_color(C_SEL_TAB_BG, 78, 78, 78);
    init_color(C_TAB_FG, 521, 482, 396);
    init_color(C_COMM_FG, 324, 300, 246);
    // Records
    init_color(C_REC_FG, 521, 482, 396);
    init_color(C_REC_BG, 156, 156, 156);
    init_color(C_S_FG, 365, 255, 0);
    init_color(C_T_FG, 455, 317, 0);
    init_color(C_C_FG, 741, 561, 156);
    init_color(C_SEC_FG, 780, 753, 682);
    init_color(C_COM_FG, 650, 604, 494);
    init_color(C_RED, 984, 286, 204);
    // Initialize color pairs
    init_pair(C_ANIM, C_ANIM_FG, C_BG);
    init_pair(C_MENU, C_MENU_FG, C_BG);
    init_pair(C_TABS, C_TAB_FG, C_BG);
    init_pair(C_SEL_TAB, C_RED, C_SEL_TAB_BG);
    init_pair(C_COMM, C_COMM_FG, C_BG);
    init_pair(C_REC, C_REC_FG, C_REC_BG);
    init_pair(C_REC_S, C_S_FG, C_REC_BG);
    init_pair(C_REC_T, C_T_FG, C_REC_BG);
    init_pair(C_REC_C, C_C_FG, C_REC_BG);
    init_pair(C_SEC, C_SEC_FG, C_REC_BG);
    init_pair(C_SEL_COM, C_RED, C_BG);
    init_pair(C_COM, C_COM_FG, C_REC_BG);
}
