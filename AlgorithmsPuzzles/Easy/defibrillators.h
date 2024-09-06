// https://www.codingame.com/ide/puzzle/defibrillators

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <cmath>

using namespace std;

int main()
{
    string lon;
    cin >> lon; cin.ignore();
    string lat;
    cin >> lat; cin.ignore();
    int n;
    cin >> n; cin.ignore();

    const double userLat{ stod(lat.replace(lat.find(","), 1, ".")) };
    const double userLon{ stod(lon.replace(lon.find(","), 1, ".")) };

    long long closestDist{ LONG_MAX };
    string closestDefib;
    for (int i = 0; i < n; i++) {
        string defib;
        getline(cin, defib);

        int searchPos{ -1 };
        searchPos = defib.find_last_of(';', searchPos);
        string latStr{ defib.substr(searchPos + 1) };
        const double dLat{ stod(latStr.replace(latStr.find(","), 1, ".")) };

        int oldSearchPos{ searchPos };
        searchPos = defib.find_last_of(';', searchPos - 1);
        string lonStr{ defib.substr(searchPos + 1, oldSearchPos - searchPos - 1) };
        const double dLon{ stod(lonStr.replace(lonStr.find(","), 1, ".")) };

        const double x{ (dLon - userLon) * cos((userLat + dLat) * 0.5) };
        const double y{ dLat - userLat };
        const double dist{ sqrt(x * x + y * y) * 6371 };

        if (dist < closestDist)
        {
            closestDist = dist;

            searchPos = defib.find_first_of(';');
            oldSearchPos = searchPos;
            searchPos = defib.find_first_of(';', searchPos + 1);

            closestDefib = defib.substr(oldSearchPos + 1, searchPos - oldSearchPos - 1);
        }
    }

    cout << closestDefib << endl;
}
