// https://www.codingame.com/ide/puzzle/don't-panic-episode-1

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int nb_floors; // number of floors
    int width; // width of the area
    int nb_rounds; // maximum number of rounds
    int exit_floor; // floor on which the exit is found
    int exit_pos; // position of the exit on its floor
    int nb_total_clones; // number of generated clones
    int nb_additional_elevators; // ignore (always zero)
    int nb_elevators; // number of elevators
    cin >> nb_floors >> width >> nb_rounds >> exit_floor >> exit_pos >> nb_total_clones >> nb_additional_elevators >> nb_elevators; cin.ignore();

    unordered_map<int, int> elevators;

    cerr << "Elevators count: " << nb_elevators << endl;
    for (int i = 0; i < nb_elevators; i++) {
        int elevator_floor; // floor on which this elevator is found
        int elevator_pos; // position of the elevator on its floor
        cin >> elevator_floor >> elevator_pos; cin.ignore();

        elevators[elevator_floor] = elevator_pos;
    }

    bool hasLeadingClone{ false };
    bool isLeadingCloneReady{ false };
    // game loop
    while (1) {
        int clone_floor; // floor of the leading clone
        int clone_pos; // position of the leading clone on its floor
        string direction; // direction of the leading clone: LEFT or RIGHT
        cin >> clone_floor >> clone_pos >> direction; cin.ignore();

        if (direction == "NONE")
        {
            cout << "WAIT" << endl;
            isLeadingCloneReady = false;
            hasLeadingClone = false;
            continue;
        }
        isLeadingCloneReady = hasLeadingClone;
        hasLeadingClone = true;

        int desiredDirection{ clone_floor == exit_floor ? exit_pos - clone_pos : elevators[clone_floor] - clone_pos };
        if (desiredDirection != 0)
        {
            desiredDirection /= abs(desiredDirection);
        }

        const int leadingDirection{ direction == "RIGHT" ? 1 : -1 };
        if (desiredDirection != 0 && desiredDirection != leadingDirection && isLeadingCloneReady)
        {
            cout << "BLOCK" << endl;
        }
        else
        {
            cout << "WAIT" << endl;
        }
    }
}