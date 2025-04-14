/*
# RUZZLE
https://www.codingame.com/ide/puzzle/conway-sequence

# GOAL
You mission is to print a specific line of the Conway sequence.

# RULES
Warning! This sequence can make you ill.
The reasoning is simple but unusual: Read a line aloud whilst looking at the line above and you will notice that each line (except the first) makes an inventory of the previous line.

1
1 1
2 1
1 2 1 1
1 1 1 2 2 1
3 1 2 2 1 1
...
- Line 3 shows 2 1 because the line 2 contains two 1, one after the other.
- Line 4 displays 1 2 1 1 because the line 3 contains one 2 followed by one 1.
- Line 5 displays 1 1 1 2 2 1 because the line 4 contains one 1 followed by one 2 followed by two 1.

Your mission is to write a program that will display the line L of this series on the basis of an original number R (R equals 1 in our example).

# INPUT
Line 1: The original number R of the sequence.
Line 2: The line L to display. The index of the first line is 1.

# OUTPUT
The line L of the sequence. Each element of the sequence is separated by a space.
Constraints
0 < R < 100
0 < L <= 25
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int conway_sequence_main()
{
    int r;
    cin >> r; cin.ignore();
    int l;
    cin >> l; cin.ignore();

    vector<int> prevLine{ r };

    while (--l)
    {
        vector<int> currentLine;

        int j{ 0 }, i{ 1 };
        int sequenceValue{ prevLine[0] };
        for (i; i < prevLine.size(); ++i)
        {
            if (sequenceValue != prevLine[i])
            {
                currentLine.emplace_back(i - j);
                currentLine.emplace_back(sequenceValue);

                sequenceValue = prevLine[i];
                j = i;
            }
        }
        currentLine.emplace_back(i - j);
        currentLine.emplace_back(sequenceValue);

        prevLine = currentLine;
    }

    int i{ 0 };
    for (i; i < prevLine.size() - 1; ++i)
    {
        cout << prevLine[i] << " ";
    }
    cout << prevLine[i];
}
