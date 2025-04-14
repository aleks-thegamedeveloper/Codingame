/*
# PUZZLE: https://www.codingame.com/ide/puzzle/russian-dolls
# GOAL
Russian nesting dolls, also known as matryoshka dolls, are typical souvenirs from Russia.
These dolls can be pulled apart in the middle to reveal increasingly smaller versions of the same doll, one within another.
In the inner most there is the smallest doll made of a solid piece of wood.

A craftsman came up with an innovative idea that a mother doll can encapsulate not only one child doll.
Multiple children is also possible. So he is starting to design these dolls.

Representation of designs
To represent the hierarchical designs we use an array of non-zero integers like this:
-5 -4 -3 -2 -1 1 2 3 4 5

Two integers of opposite signs are used to represent a doll. -5 and 5 are the lower and upper parts of a size #5 doll.
This doll encapsulates a size #4 doll which is composed of -4 and 4, the lower and upper parts.
Going several layers deeper, in the middle of the dolls set there is a size #1 doll, -1 and 1,
without another doll inside indicating this doll is a solid wood. This is a traditional nesting dolls set.

To standardize the representation, the negative number of a doll size should always be placed before the positive number.
All the parts must be arranged according to their nesting order.

An innovative doll set could be like this:

-10 -3 -2 2 3 -1 1 -5 -4 -1 1 4 5 10
 |   |  |_| |  |_|  |  |  |_| | | |
 |   |______|       |  |______| | |
 |                  |___________| |
 |________________________________|

A size #10 doll is encapsulating three smaller dolls, sizes #3, #1 and #5. The size #1 doll is a solid wood.
The size #3 doll has a size #2 solid doll inside. The size #5 doll has a size #4 doll inside which is holding a size #1 solid doll.

Pay attention to the sizes.
A mother doll of size x can encapsulate children dolls of sizes x1, x2, x3...xn if and only if x1 + x2 + x3 +...+ xn < x

Remark that this rule applies to only the direct children dolls. The sizes of grand- or grand-grand-children do not count.

Detecting errors
There are thousands of ways for things to go wrong. Extend your imagination.
Programmers in the real world are expected to detect all kinds of errors from human inputs.
Here are a few examples, non-exhaustive, of invalid representations of designs:

-10 -6 6 -4 4 10 (mother doll not big enough)
-10 -6 -3 6 3 10 (wrong nesting order)
-10 6 -6 -3 3 10 (-6 should come before 6)
-10 0 10         (zero not allowed)
-10 10 -9 9      (two separate dolls without a mother)
-10              (a doll missing the upper part)
#10              (wrong format)
-10 1O           (wrong character)


Your task is to verify some nesting dolls designs.
If a design is valid and correct in representation, count how many solid dolls are in the set.

# INPUT
Line 1: An integer n for the number of test cases to follow.
Next n lines: Each line is an independent test case.
Each test case is an array of space-separated values representing all the parts in one nesting dolls set.
These values are supposed to be non-zero integers in valid designs.

# OUTPUT
Write n lines corresponding to the input test cases.
For each line,
- if a dolls set design is invalid or violating the descriptions in the statement, write -1
- otherwise, write how many solid dolls are in the set.

# CONSTRAINTS
1 <= n <= 200
-200 <= doll size (lower/upper parts) <= 200, exclude 0
length of input line < 256
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool ValidateDollDefinition(
    const string& definition, int startingIdx, int dollSize, int& outSmallestDollCount, int& outInternalDollSizes)
{
    outInternalDollSizes = 0;

    int elementEndPos{};
    do {
        // Extract the entry that 'opens' a doll definition
        elementEndPos = definition.find(' ', startingIdx);
        const int subdollStart{ stoi(definition.substr(startingIdx, elementEndPos)) };
        int subdollStartIdx{ elementEndPos != -1 ? elementEndPos + 1 : elementEndPos };
        if (subdollStart >= 0 || subdollStartIdx == -1)
        {
            return false;
        }

        int subdollEnd{};
        int subdollEndIdx{};
        int nextElementStart{ subdollStartIdx };
        // Try to find the entry that 'closes' the same doll definition
        do
        {
            elementEndPos = definition.find(' ', nextElementStart);
            subdollEnd = stoi(definition.substr(nextElementStart, elementEndPos - nextElementStart));

            subdollEndIdx = nextElementStart - 1;
            nextElementStart = elementEndPos + 1;

        } while (subdollEnd != abs(subdollStart) && elementEndPos != -1);

        if (subdollEnd != abs(subdollStart))
        {
            // Incomplete doll definition found
            return false;
        }

        if (subdollEndIdx > subdollStartIdx)
        {
            // Process internal doll definitions
            const string& internalDollDefinitions{ definition.substr(subdollStartIdx, subdollEndIdx - subdollStartIdx) };
            int internalDollSizes{};
            if (!ValidateDollDefinition(internalDollDefinitions, 0, subdollEnd, outSmallestDollCount, internalDollSizes))
            {
                return false;
            }

            if (subdollEnd <= internalDollSizes)
            {
                return false;
            }
        }
        else
        {
            outSmallestDollCount += 1;
        }

        outInternalDollSizes += subdollEnd;
        startingIdx = nextElementStart;
    } while (elementEndPos != -1);

    return true;
}

int russian_doll_main()
{
    int n;
    cin >> n; cin.ignore();
    for (int lineIdx{ 0 }; lineIdx < n; ++lineIdx) {
        string line;
        getline(cin, line);

        bool isInvalid{ false };
        // Basic validation to catch incorrectly composed input string
        for (int i{ 0 }; i < line.size(); ++i)
        {
            const char c{ line[i] };
            if (c == '-' && !(i == 0 || line[i - 1] == ' '))
            {
                isInvalid = true;
                break;
            }

            if (c != '-' && !(c >= '0' && c <= '9') && c != ' ')
            {
                isInvalid = true;
                cerr << line << " index: " << i << endl;
                break;
            }
        }

        if (isInvalid)
        {
            cout << -1 << endl;
            continue;
        }

        const int firstElementEndIdx = line.find(' ');
        const int firstElement{ stoi(line.substr(0, firstElementEndIdx)) };

        const int lastElementStartIdx = line.find_last_of(' ') + 1;
        const int lastElement{ stoi(line.substr(lastElementStartIdx)) };

        if (abs(firstElement) != lastElement)
        {
            cout << -1 << endl;
            continue;
        }

        int smallestDollCount{};
        int internalDollSizes{};
        if (firstElementEndIdx + 1 < lastElementStartIdx)
        {
            if (!ValidateDollDefinition(
                    line.substr(firstElementEndIdx + 1, lastElementStartIdx - (firstElementEndIdx + 1) - 1),
                    0,
                    lastElement,
                    smallestDollCount,
                    internalDollSizes))
            {
                cout << -1 << endl;
                continue;
            }
        }
        else
        {
            smallestDollCount = 1;
        }

        if (internalDollSizes >= lastElement)
        {
            cout << -1 << endl;
            continue;
        }

        cout << smallestDollCount << endl;
    }
}
