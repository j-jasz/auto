
#include "records.hpp"

#include <vector>

std::vector<Record> getAllRecords() {
    std::vector<Record> structs;

// ============================================================================================================================
// APT
    Record section1;
    section1.tab = "APT";
    section1.type = ' ';
    section1.label = "APT";
    structs.push_back(section1);

    Record a1;
    a1.tab = "APT";
    a1.type = 'C';
    a1.label = "Full update";
    a1.command = "sudo apt update && sudo apt upgrade && sudo apt dist-upgrade && sudo apt --fix-broken install && sudo apt --fix-missing install && sudo apt autoremove && sudo apt autoclean && sudo apt purge $(dpkg -l | grep '^rc' | awk '{print $2}')";
    a1.comment = "Update, upgrade, dist-upgrade, --fix-broken, --fix missing, autoremove, autoclean, purge.";
    a1.color = "Blue";
    structs.push_back(a1);

    Record a2;
    a2.tab = "APT";
    a2.type = 'T';
    a2.label = "sudo apt install [...]";
    a2.command = "sudo apt install ";
    a2.comment = "Install template.";
    a2.color = "Blue";
    structs.push_back(a2);

    Record a3;
    a3.tab = "APT";
    a3.type = 'C';
    a3.label = "sudo nano /etc/apt/sources.list";
    a3.command = "sudo nano /etc/apt/sources.list";
    a3.comment = "Edit APT sources.";
    a3.color = "Blue";
    structs.push_back(a3);

    Record a4;
    a4.tab = "APT";
    a4.type = 'T';
    a4.label = "debtree [...]";
    a4.command = "debtree ";
    a4.comment = "Lists package dependencies.";
    a4.color = "Blue";
    structs.push_back(a4);

    Record a8;
    a8.tab = "APT";
    a8.type = 'T';
    a8.label = "sudo apt-cache showpkg [...]";
    a8.command = "sudo apt-cache showpkg ";
    a8.comment = "Lists package dependencies.";
    a8.color = "Blue";
    structs.push_back(a8);

    Record a5;
    a5.tab = "APT";
    a5.type = 'C';
    a5.label = "sudo apt-mark showhold";
    a5.command = "sudo apt-mark showhold";
    a5.comment = "Show packages on hold.";
    a5.color = "Blue";
    structs.push_back(a5);

    Record a6;
    a6.tab = "APT";
    a6.type = 'T';
    a6.label = "sudo apt-mark hold [...]";
    a6.command = "sudo apt-mark hold ";
    a6.comment = "Hold package.";
    a6.color = "Blue";
    structs.push_back(a6);

    Record a7;
    a7.tab = "APT";
    a7.type = 'T';
    a7.label = "sudo apt-mark unhold [...]";
    a7.command = "sudo apt-mark unhold ";
    a7.comment = "Unhold package.";
    a7.color = "Blue";
    structs.push_back(a7);

    Record a9;
    a9.tab = "APT";
    a9.type = 'C';
    a9.label = "sudo apt-mark unhold $(apt-mark showhold)";
    a9.command = "sudo apt-mark unhold $(apt-mark showhold)";
    a9.comment = "Automatically unhold all packages.";
    a9.color = "Blue";
    structs.push_back(a9);

// ============================================================================================================================
// SYSTEM
    Record sectionS;
    sectionS.tab = "SYSTEM";
    sectionS.type = ' ';
    sectionS.label = "SYSTEM INFO";
    structs.push_back(sectionS);

    Record s1;
    s1.tab = "SYSTEM";
    s1.type = 'C';
    s1.label = "uname -a";
    s1.command = "uname -a";
    s1.comment = "Displays basic info about the OS.";
    s1.color = "Blue";
    structs.push_back(s1);

    Record s2;
    s2.tab = "SYSTEM";
    s2.type = 'C';
    s2.label = "conky -d";
    s2.command = "conky -d";
    s2.comment = "Starts conky daemon.";
    s2.color = "Blue";
    structs.push_back(s2);

    Record s4;
    s4.tab = "SYSTEM";
    s4.type = 'C';
    s4.label = "conky -V | head -1";
    s4.command = "conky -V | head -1";
    s4.comment = "Displays info about conky.";
    s4.color = "Blue";
    structs.push_back(s4);

    Record s5;
    s5.tab = "SYSTEM";
    s5.type = 'C';
    s5.label = "groups";
    s5.command = "groups";
    s5.comment = "Displays list of user groups.";
    s5.color = "Blue";
    structs.push_back(s5);

    Record s6;
    s6.tab = "SYSTEM";
    s6.type = 'C';
    s6.label = "groups $(whoami)";
    s6.command = "groups $(whoami)";
    s6.comment = "Displays groups that current user is in.";
    s6.color = "Blue";
    structs.push_back(s6);

    Record s7;
    s7.tab = "SYSTEM";
    s7.type = 'C';
    s7.label = "pstree";
    s7.command = "pstree";
    s7.comment = "Displays process tree.";
    s7.color = "Blue";
    structs.push_back(s7);

    Record s8;
    s8.tab = "SYSTEM";
    s8.type = 'C';
    s8.label = "sudo service --status-all";
    s8.command = "sudo service --status-all";
    s8.comment = "Displays status of the services.";
    s8.color = "Blue";
    structs.push_back(s8);

    Record s3;
    s3.tab = "SYSTEM";
    s3.type = 'C';
    s3.label = "grep Huge /proc/meminfo";
    s3.command = "grep Huge /proc/meminfo";
    s3.comment = "Display info about Huge Pages.";
    s3.color = "Blue";
    structs.push_back(s3);

    Record s10;
    s10.tab = "SYSTEM";
    s10.type = 'C';
    s10.label = "sudo lynis audit system";
    s10.command = "sudo lynis audit system";
    s10.comment = "Audits the system with Lynis tool.";
    s10.color = "Blue";
    structs.push_back(s10);
// ============================================
// GRUB & BOOT.
    Record sectionGB;
    sectionGB.tab = "SYSTEM";
    sectionGB.type = ' ';
    sectionGB.label = "GRUB & BOOT";
    structs.push_back(sectionGB);

    Record s11;
    s11.tab = "SYSTEM";
    s11.type = 'C';
    s11.label = "sudo nano /etc/default/grub";
    s11.command = "sudo nano /etc/default/grub";
    s11.comment = "Edit GRUB config.";
    s11.color = "Blue";
    structs.push_back(s11);

    Record s12;
    s12.tab = "SYSTEM";
    s12.type = 'C';
    s12.label = "sudo update-grub";
    s12.command = "sudo update-grub";
    s12.comment = "Updates GRUB.";
    s12.color = "Blue";
    structs.push_back(s12);

    Record s13;
    s13.tab = "SYSTEM";
    s13.type = 'C';
    s13.label = "sudo nano /etc/fstab";
    s13.command = "sudo nano /etc/fstab";
    s13.comment = "Edit fstab config.";
    s13.color = "Blue";
    structs.push_back(s13);

    Record s14;
    s14.tab = "SYSTEM";
    s14.type = 'C';
    s14.label = "sudo update-initramfs -u -k all";
    s14.command = "sudo update-initramfs -u -k all";
    s14.comment = "Update initramfs.";
    s14.color = "Blue";
    structs.push_back(s14);
// ============================================
// FIRMWARE
    Record sectionFW;
    sectionFW.tab = "SYSTEM";
    sectionFW.type = ' ';
    sectionFW.label = "FIRMWARE";
    structs.push_back(sectionFW);

    Record s15;
    s15.tab = "SYSTEM";
    s15.type = 'C';
    s15.label = "fwupdmgr get-devices";
    s15.command = "fwupdmgr get-devices";
    s15.comment = "Displays device fw info.";
    s15.color = "Blue";
    structs.push_back(s15);

    Record s16;
    s16.tab = "SYSTEM";
    s16.type = 'C';
    s16.label = "fwupdmgr refresh";
    s16.command = "fwupdmgr refresh";
    s16.comment = "Refresh fw metadata.";
    s16.color = "Blue";
    structs.push_back(s16);

    Record s17;
    s17.tab = "SYSTEM";
    s17.type = 'C';
    s17.label = "fwupdmgr get-updates";
    s17.command = "fwupdmgr get-updates";
    s17.comment = "Check for fw updates.";
    s17.color = "Blue";
    structs.push_back(s17);

    Record s18;
    s18.tab = "SYSTEM";
    s18.type = 'C';
    s18.label = "fwupdmgr update";
    s18.command = "fwupdmgr update";
    s18.comment = "Apply fw updates.";
    s18.color = "Blue";
    structs.push_back(s18);
// ============================================
// HARDWARE
    Record sectionHW;
    sectionHW.tab = "SYSTEM";
    sectionHW.type = ' ';
    sectionHW.label = "HARDWARE";
    structs.push_back(sectionHW);

    Record s28;
    s28.tab = "SYSTEM";
    s28.type = 'C';
    s28.label = "sudo dmesg";
    s28.command = "sudo dmesg";
    s28.comment = "Displays kernel messages, typically used for debugging hardware issues.";
    s28.color = "Blue";
    structs.push_back(s28);

    Record s22;
    s22.tab = "SYSTEM";
    s22.type = 'C';
    s22.label = "sudo dmidecode";
    s22.command = "sudo dmidecode";
    s22.comment = "Retrieves detailed hardware information from the system's BIOS/firmware.";
    s22.color = "Blue";
    structs.push_back(s22);

    Record s23;
    s23.tab = "SYSTEM";
    s23.type = 'C';
    s23.label = "sudo lspci";
    s23.command = "sudo lspci";
    s23.comment = "Lists all PCI devices on the system.";
    s23.color = "Blue";
    structs.push_back(s23);

    Record s25;
    s25.tab = "SYSTEM";
    s25.type = 'C';
    s25.label = "sudo lshw";
    s25.command = "sudo lshw";
    s25.comment = "Provides detailed information about the hardware configuration of the system.";
    s25.color = "Blue";
    structs.push_back(s25);

    Record s26;
    s26.tab = "SYSTEM";
    s26.type = 'C';
    s26.label = "sudo lshw -short";
    s26.command = "sudo lshw -short";
    s26.comment = "Outputs a concise summary of hardware details.";
    s26.color = "Blue";
    structs.push_back(s26);

    Record s27;
    s27.tab = "SYSTEM";
    s27.type = 'C';
    s27.label = "sudo lshw -short -C memory";
    s27.command = "sudo lshw -short -C memory";
    s27.comment = "Displays a short summary of memory-related hardware.";
    s27.color = "Blue";
    structs.push_back(s27);

    Record s24;
    s24.tab = "SYSTEM";
    s24.type = 'C';
    s24.label = "sudo lsusb";
    s24.command = "sudo lsusb";
    s24.comment = "Displays information about USB devices connected to the system.";
    s24.color = "Blue";
    structs.push_back(s24);

    Record s20;
    s20.tab = "SYSTEM";
    s20.type = 'C';
    s20.label = "sudo modprobe usb_storage";
    s20.command = "sudo modprobe usb_storage";
    s20.comment = "Loads the kernel module for USB storage devices.";
    s20.color = "Blue";
    structs.push_back(s20);

    Record s21;
    s21.tab = "SYSTEM";
    s21.type = 'C';
    s21.label = "sudo lsmod | grep usb";
    s21.command = "sudo lsmod | grep usb";
    s21.comment = "Displays loaded kernel modules, filtering for USB-related ones.";
    s21.color = "Blue";
    structs.push_back(s21);

    Record s19;
    s19.tab = "SYSTEM";
    s19.type = 'C';
    s19.label = "xsetwacom --list devices";
    s19.command = "xsetwacom --list devices";
    s19.comment = "Lists all Wacom tablet devices connected to the system.";
    s19.color = "Blue";
    structs.push_back(s19);
// ============================================
// POWER
    Record sectionP;
    sectionP.tab = "SYSTEM";
    sectionP.type = ' ';
    sectionP.label = "POWER";
    structs.push_back(sectionP);

    Record s30;
    s30.tab = "SYSTEM";
    s30.type = 'C';
    s30.label = "sudo powertop";
    s30.command = "sudo powertop";
    s30.comment = "Analyzes power usage and provides suggestions for optimization.";
    s30.color = "Blue";
    structs.push_back(s30);

    Record s29;
    s29.tab = "SYSTEM";
    s29.type = 'C';
    s29.label = "sudo upower -i `upower -e | grep 'BAT'`";
    s29.command = "sudo upower -i `upower -e | grep 'BAT'`";
    s29.comment = "Shows detailed information about the battery.";
    s29.color = "Blue";
    structs.push_back(s29);
// ============================================
// LOGS
    Record sectionL;
    sectionL.tab = "SYSTEM";
    sectionL.type = ' ';
    sectionL.label = "LOGS";
    structs.push_back(sectionL);

    Record s31;
    s31.tab = "SYSTEM";
    s31.type = 'C';
    s31.label = "sudo cat /var/log/syslog";
    s31.command = "sudo cat /var/log/syslog";
    s31.comment = "Outputs the system log file containing general system activity.";
    s31.color = "Blue";
    structs.push_back(s31);

    Record s32;
    s32.tab = "SYSTEM";
    s32.type = 'C';
    s32.label = "sudo cat history.log";
    s32.command = "sudo cat history.log";
    s32.comment = "Shows a log of package installation and removal activities.";
    s32.color = "Blue";
    structs.push_back(s32);

    Record s33;
    s33.tab = "SYSTEM";
    s33.type = 'C';
    s33.label = "sudo cat term.log";
    s33.command = "sudo cat term.log";
    s33.comment = "Displays terminal logs related to package management.";
    s33.color = "Blue";
    structs.push_back(s33);

// ============================================================================================================================
// DESKTOP
    Record section2;
    section2.tab = "DESKTOP";
    section2.type = ' ';
    section2.label = "XFCE";
    structs.push_back(section2);

    Record d1;
    d1.tab = "DESKTOP";
    d1.type = 'C';
    d1.label = "xfce4-panel --quit";
    d1.command = "xfce4-panel --quit";
    d1.comment = "Kills XFCE panels.";
    d1.color = "Blue";
    structs.push_back(d1);

    Record d11;
    d11.tab = "DESKTOP";
    d11.type = 'C';
    d11.label = "xfce4-panel &";
    d11.command = "xfce4-panel &";
    d11.comment = "Starts XFCE panels.";
    d11.color = "Blue";
    structs.push_back(d11);

    Record d2;
    d2.tab = "DESKTOP";
    d2.type = 'C';
    d2.label = "xfdesktop --replace";
    d2.command = "xfdesktop --replace";
    d2.comment = "Reloads XFCE desktop.";
    d2.color = "Blue";
    structs.push_back(d2);

    Record d3;
    d3.tab = "DESKTOP";
    d3.type = 'C';
    d3.label = "xfwm4 --replace &";
    d3.command = "xfwm4 --replace &";
    d3.comment = "Reloads XFCE Window Manager.";
    d3.color = "Blue";
    structs.push_back(d3);
// ============================================
// X11, DM, Screen, Peripherals
    Record sectionx11;
    sectionx11.tab = "DESKTOP";
    sectionx11.type = ' ';
    sectionx11.label = "X11, DM, Screen, Peripherals";
    structs.push_back(sectionx11);

    Record x1;
    x1.tab = "DESKTOP";
    x1.type = 'C';
    x1.label = "startx";
    x1.command = "startx";
    x1.comment = "Starts X server.";
    x1.color = "Blue";
    structs.push_back(x1);

    Record x2;
    x2.tab = "DESKTOP";
    x2.type = 'C';
    x2.label = "cat /etc/X11/default-display-manager";
    x2.command = "cat /etc/X11/default-display-manager";
    x2.comment = "Displays the default display manager.";
    x2.color = "Blue";
    structs.push_back(x2);

    Record x3;
    x3.tab = "DESKTOP";
    x3.type = 'C';
    x3.label = "xrandr";
    x3.command = "xrandr";
    x3.comment = "Configures and queries display settings such as resolution, orientation, and refresh rate.";
    x3.color = "Blue";
    structs.push_back(x3);

    Record x4;
    x4.tab = "DESKTOP";
    x4.type = 'C';
    x4.label = "xrandr --verbose";
    x4.command = "xrandr --verbose";
    x4.comment = "Provides detailed information about connected displays and their configurations.";
    x4.color = "Blue";
    structs.push_back(x4);

    Record x5;
    x5.tab = "DESKTOP";
    x5.type = 'C';
    x5.label = "xset q";
    x5.command = "xset q";
    x5.comment = "Show keyboard, mouse and screen settings. Queries current settings of user preferences in the X server.";
    x5.color = "Blue";
    structs.push_back(x5);

    Record x6;
    x6.tab = "DESKTOP";
    x6.type = 'C';
    x6.label = "xset r rate 160 20";
    x6.command = "xset r rate 160 20";
    x6.comment = "The first number (160) specifies the delay (in milliseconds) before a key starts repeating when held down. The second number (20) sets the repeat rate (keys per second).";
    x6.color = "Blue";
    structs.push_back(x6);

// ============================================================================================================================

    return structs;
}

