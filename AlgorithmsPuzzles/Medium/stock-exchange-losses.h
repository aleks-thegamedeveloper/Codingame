/*
# PUZZLE
https://www.codingame.com/ide/puzzle/stock-exchange-losses

# GOAL
A finance company is carrying out a study on the worst stock investments and would like to acquire a program to do so.
The program must be able to analyze a chronological series of stock values in order to show the largest loss that it is possible to make by buying a share at a given time t0 and by selling it at a later date t1.
The loss will be expressed as the difference in value between t0 and t1. If there is no loss, the loss will be worth 0.

# INPUT
Line 1: the number n of stock values available.
Line 2: the stock values arranged in order, from the date of their introduction on the stock market v1 until the last known value vn. The values are integers.

# OUTPUT
The maximal loss p, expressed negatively if there is a loss, otherwise 0.

# CONSTRAINTS
0 < n < 100000
0 < v < 231
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int n;
    cin >> n; cin.ignore();

    int currentPeak{ 0 };
    int currentValley{ 0 };
    int biggestPossibleLoss{ 0 };

    for (int i = 0; i < n; i++) {
        int v;
        cin >> v; cin.ignore();

        if (v > currentPeak)
        {
            biggestPossibleLoss = max(biggestPossibleLoss, currentPeak - currentValley);

            currentPeak = v;
            currentValley = v;
        }
        else if (v < currentValley)
        {
            currentValley = v;
        }
    }

    biggestPossibleLoss = max(biggestPossibleLoss, currentPeak - currentValley);

    cout << -biggestPossibleLoss << endl;
}
