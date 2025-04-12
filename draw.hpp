#ifndef DRAW_HPP
#define DRAW_HPP

#include "records.hpp"

#include <ncurses.h>
#include <string>

void drawAnim(WINDOW *AnimWin);
void drawTabs(WINDOW *TabsWin, const TabData &data, int selectedTab);
int getColorPair(char type, bool isSelected);
void wrapAndPrintLabel(WINDOW *win, int lineIndex, int startX, int width, const std::string &label, bool highlight = false);
void drawRecords(WINDOW *RecWin, const TabData &data, int selectedTab, int selectedRecord, int offset);
void drawComments(WINDOW *CommWin, const TabData &data, int selectedTab, int selectedRecord);

void drawFullView(WINDOW* AnimWin, WINDOW* TabsWin, WINDOW* RecWin, WINDOW* CommWin, TabData& data, int selectedTab, int selectedRecord, int offset);
void drawRecordsAndComments(WINDOW* RecWin, WINDOW* CommWin, TabData& data, int selectedTab, int selectedRecord, int offset);

#endif // DRAW_HPP
