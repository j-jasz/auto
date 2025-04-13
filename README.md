![out](https://github.com/user-attachments/assets/fd281b91-7996-4783-9b57-918085ad36d7)

# auto
CLI automation tool for Linux with Ncurses interface.

3 command type handling:
  - [S] - executes script from $PATH
  - [T] - editable shell command templates
  - [C] - regular shell commands

Usage:
  - [Tab] / [`] - select Tab
  - [Up] / [Down] - select command
  - [Enter] - executes command
  - [Q] - quit

Compile with `g++ auto.cpp draw.cpp records.cpp colors.cpp script_utils.cpp utils.cpp -o auto -lncurses`

![auto](https://github.com/user-attachments/assets/bef1303c-f81b-4913-8ec7-90e6f6a111a3)
