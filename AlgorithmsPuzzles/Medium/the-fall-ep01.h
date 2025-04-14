/*
# PUZZLE
https://www.codingame.com/ide/puzzle/the-fall-episode-1

# GOAL
Your objective is to write a program capable of predicting the route Indy will take on his way down a tunnel.

# RULES
The tunnel consists of a patchwork of square rooms of different types.
The rooms can be accessed and activated by computer using an ancient RS232 serial port (because Mayans aren't very technologically advanced, as is to be expected...).

There is a total of 14 room types (6 base shapes extended to 14 through rotations).
Upon entering a room, depending on the type of the room and Indy's entrance point (TOP,LEFT, or RIGHT)
he will either exit the room through a specific exit point, suffer a lethal collision or lose momentum and get stuck.

Indy is perpetually drawn downwards: he cannot leave a room through the top.

At the start of the game, you are given the map of the tunnel in the form of a rectangular grid of rooms. Each room is represented by its type.

For this first mission, you will familiarize yourself with the tunnel system,
the rooms have all been arranged in such a way that Indy will have a safe continuous route between his starting point (top of the temple) and the exit area (bottom of the temple).

Each game turn:
You receive Indy's current position
Then you specify what Indy's position will be next turn.
Indy will then move from the current room to the next according to the shape of the current room.

Victory Conditions
Indy reaches the exit

Lose Conditions
You assert an incorrect position for Indy

# INPUT
Initialization input
Line 1: 2 space separated integers W H specifying the width and height of the grid.
Next H lines: each line represents a line in the grid and contains W space separated integers T. T specifies the type of the room.
Last line: 1 integer EX specifying the coordinate along the X axis of the exit.

Input for one game turn
Line 1: XI YI POS
(XI, YI) two integers to indicate Indy's current position on the grid.
POS a single word indicating Indy's entrance point into the current room: TOP if Indy enters from above, LEFT if Indy enters from the left and RIGHT if Indy enters from the right.

# OUTPUT
For one game turn
A single line with 2 integers: X Y representing the (X, Y) coordinates of the room in which you believe Indy will be on the next turn.

# CONSTRAINTS
0 < W <= 20
0 < H <= 20
0 <= T <= 13
0 <= EX < W
0 <= XI, X < W
0 <= YI, Y < H

Response time for one game <= 150ms
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

tuple<int, int> ResolveCellTranslation(int cellType, tuple<int, int> entranceDir)
{
    cerr << "Cell Type: " << cellType;
    switch (cellType)
    {
    case 1: return { 0, 1 };
    case 2: return entranceDir;
    case 3: return { 0, 1 };
    case 4:
    {
        auto& [xDir, yDir] = entranceDir;
        if (yDir == -1)
            return { -1, 0 };
        if (xDir == -1)
            return { 0, 1 };
        break;
    }
    case 5:
    {
        auto& [xDir, yDir] = entranceDir;
        if (yDir == -1)
            return { 1, 0 };
        if (xDir == 1)
            return { 0, 1 };
        break;
    }
    case 6:
    {
        auto& [xDir, yDir] = entranceDir;
        if (yDir != 0)
            return { 0, 0 };
        else
            return entranceDir;
    }
    case 7: return { 0, 1 };
    case 8: return { 0, 1 };
    case 9: return { 0, 1 };
    case 10:
    {
        auto& [xDir, yDir] = entranceDir;
        if (xDir != 0)
            return { 0, 0 };

        return { -1, 0 };
    }
    case 11:
    {
        auto& [xDir, yDir] = entranceDir;
        if (xDir != 0)
            return { 0, 0 };

        return { 1, 0 };
    }
    case 12:
    case 13: return { 0, 1 };
    }

    return {};
}

int the_fall_ep01_main()
{
    int w; // number of columns.
    int h; // number of rows.
    cin >> w >> h; cin.ignore();

    vector<vector<int>> grid(h);
    for (int i = 0; i < h; i++) {
        string line;
        getline(cin, line); // represents a line in the grid and contains W integers. Each integer represents one room of a given type.

        // Parse room types at level i
        vector<int>& row = grid[i];
        int pos = line.find(' ');
        while (pos != -1)
        {
            row.push_back(stoi(line.substr(0, pos)));
            line = line.substr(pos + 1);
            pos = line.find(' ');
        }
        row.push_back(stoi(line));
    }

    int ex; // the coordinate along the X axis of the exit (not useful for this first mission, but must be read).
    cin >> ex; cin.ignore();

    // game loop
    while (1) {
        int xi;
        int yi;
        string pos;
        cin >> xi >> yi >> pos; cin.ignore();

        tuple<int, int> dir;
        if (pos == "TOP")
        {
            dir = { 0, -1 };
        }
        else if (pos == "LEFT")
        {
            dir = { 1, 0 };
        }
        else if (pos == "RIGHT")
        {
            dir = { -1, 0 };
        }

        const int cellType{ grid[yi][xi] };

        tuple<int, int> translation{ ResolveCellTranslation(cellType, dir) };
        auto& [xTranslation, yTranslation] = translation;

        // One line containing the X Y coordinates of the room in which you believe Indy will be on the next turn.
        cout << xi + xTranslation << " " << yi + yTranslation << endl;
    }
}
