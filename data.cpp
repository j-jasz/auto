#include "records.hpp"
#include <vector>

std::vector<Record> getAllRecords() {
    std::vector<Record> structs;

// ==============================================
// APT
    Record section1;
    section1.tab = 0;
    section1.type = ' ';
    section1.label = "APT";
    structs.push_back(section1);
    // ------
    Record a1;
    a1.tab = 0;
    a1.type = 'C';
    a1.label = "Full update";
    a1.command = "sudo apt update && sudo apt upgrade && sudo apt dist-upgrade && sudo apt --fix-broken install && sudo apt --fix-missing install && sudo apt autoremove && sudo apt autoclean && sudo apt purge $(dpkg -l | grep '^rc' | awk '{print $2}')";
    a1.comment = "Update, upgrade, dist-upgrade, --fix-broken, --fix missing, autoremove, autoclean, purge.";
    a1.color = "Blue";
    structs.push_back(a1);

    Record a2;
    a2.tab = 0;
    a2.type = 'T';
    a2.label = "sudo apt install [...]";
    a2.command = "sudo apt install ";
    a2.comment = "Install template.";
    a2.color = "Blue";
    structs.push_back(a2);

    Record a3;
    a3.tab = 0;
    a3.type = 'C';
    a3.label = "sudo nano /etc/apt/sources.list";
    a3.command = "sudo nano /etc/apt/sources.list";
    a3.comment = "Edit APT sources.";
    a3.color = "Blue";
    structs.push_back(a3);

    Record a4;
    a4.tab = 0;
    a4.type = 'T';
    a4.label = "debtree [...]";
    a4.command = "debtree ";
    a4.comment = "Lists package dependencies.";
    a4.color = "Blue";
    structs.push_back(a4);

    Record a8;
    a8.tab = 0;
    a8.type = 'T';
    a8.label = "sudo apt-cache showpkg [...]";
    a8.command = "sudo apt-cache showpkg ";
    a8.comment = "Lists package dependencies.";
    a8.color = "Blue";
    structs.push_back(a8);

    Record a5;
    a5.tab = 0;
    a5.type = 'C';
    a5.label = "sudo apt-mark showhold";
    a5.command = "sudo apt-mark showhold";
    a5.comment = "Show packages on hold.";
    a5.color = "Blue";
    structs.push_back(a5);

    Record a6;
    a6.tab = 0;
    a6.type = 'T';
    a6.label = "sudo apt-mark hold [...]";
    a6.command = "sudo apt-mark hold ";
    a6.comment = "Hold package.";
    a6.color = "Blue";
    structs.push_back(a6);

    Record a7;
    a7.tab = 0;
    a7.type = 'T';
    a7.label = "sudo apt-mark unhold [...]";
    a7.command = "sudo apt-mark unhold ";
    a7.comment = "Unhold package.";
    a7.color = "Blue";
    structs.push_back(a7);

    Record a9;
    a9.tab = 0;
    a9.type = 'C';
    a9.label = "sudo apt-mark unhold $(apt-mark showhold)";
    a9.command = "sudo apt-mark unhold $(apt-mark showhold)";
    a9.comment = "Automatically unhold all packages.";
    a9.color = "Blue";
    structs.push_back(a9);

// ==============================================
// DESKTOP
    Record section2;
    section2.tab = 1;
    section2.type = ' ';
    section2.label = "Desktop";
    structs.push_back(section2);
    // ------
    Record d1;
    d1.tab = 1;
    d1.type = 'C';
    d1.label = "Reload XFCE panels";
    d1.command = "xfce4-panel --quit && xfce4-panel &";
    d1.comment = "Reloads XFCE panels.";
    d1.color = "Blue";
    structs.push_back(d1);

    Record d2;
    d2.tab = 1;
    d2.type = 'C';
    d2.label = "Reload XFCE desktop";
    d2.command = "xfdesktop --replace";
    d2.comment = "Reloads XFCE desktop.";
    d2.color = "Blue";
    structs.push_back(d2);

    Record d3;
    d3.tab = 1;
    d3.type = 'C';
    d3.label = "Reload XFCE WM";
    d3.command = "xfwm4 --replace &";
    d3.comment = "Reloads XFCE Window Manager.";
    d3.color = "Blue";
    structs.push_back(d3);

// ==============================================
// BLENDER
    Record section3;
    section3.tab = 2;
    section3.type = ' ';
    section3.label = "Blender";
    structs.push_back(section3);
    // ------
    Record b1;
    b1.tab = 2;
    b1.type = 'C';
    b1.label = "Run Blender";
    b1.command = "./blender";
    b1.comment = "";
    b1.color = "Red";
    structs.push_back(b1);

    Record b2;
    b2.tab = 2;
    b2.type = 'C';
    b2.label = "./blender --factory-startup --debug-all --verbose 5";
    b2.command = "./blender --factory-startup --debug-all --verbose 5";
    b2.comment = "Run factory setup for debugging";
    b2.color = "Blue";
    structs.push_back(b2);

    Record b3;
    b3.tab = 2;
    b3.type = 'C';
    b3.label = "./blender --factory-startup --debug-memory --verbose 5";
    b3.command = "./blender --factory-startup --debug-memory --verbose 5";
    b3.comment = "Run factory setup for memory debugging";
    b3.color = "Blue";
    structs.push_back(b3);

// ==============================================
//

    return structs;
}

