#include "records.hpp"

Record getSpacer() {
    return Record {
        .type = ' ',
        .label = "",
        .command = "",
        .comment = "",
        .color = ""
    };
}

// Tab 1
Record getRecord1() {
    return Record {
        .type = 'S',
        .label = "Label1",
        .command = "Command1",
        .comment = "This is a comment for Label1.",
        .color = "Blue"
    };
}

Record getRecord2() {
    return Record {
        .type = 'T',
        .label = "Label2",
        .command = "Command2",
        .comment = "This is a comment for Label2 Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
        .color = "Blue"
    };
}

Record getRecord3() {
    return Record {
        .type = 'C',
        .label = "Label3",
        .command = "Command3",
        .comment = "This is a comment for Label3 Lorem Ipsum",
        .color = "Red"
    };
}

Record getRecord4() {
    return Record {
        .type = 'S',
        .label = "Label4",
        .command = "Command4",
        .comment = "This is a comment for Label4 Lorem Ipsum",
        .color = "Blue"
    };
}

// Tab 2
Record getRecord5() {
    return Record {
        .type = 'S',
        .label = "Label5",
        .command = "Command5",
        .comment = "This is a comment for Label5 Lorem Ipsum",
        .color = "Red"
    };
};

Record getRecord6() {
    return Record {
        .type = 'T',
        .label = "Label6",
        .command = "Command6",
        .comment = "This is a comment for Label6 Lorem Ipsum",
        .color = "Blue"
    };
};

// Tab 3
Record getRecord7() {
    return Record {
        .type = 'C',
        .label = "Label7",
        .command = "Command7",
        .comment = "This is a comment for Label7 Lorem Ipsum",
        .color = "Red"
    };
};

Record getRecord8() {
    return Record {
        .type = 'S',
        .label = "Label8",
        .command = "Command8",
        .comment = "This is a comment for Label8 Lorem Ipsum",
        .color = "Blue"
    };
};
