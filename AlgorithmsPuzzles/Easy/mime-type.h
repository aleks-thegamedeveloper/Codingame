// https://www.codingame.com/training/easy/mime-type

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
    int n; // Number of elements which make up the association table.
    cin >> n; cin.ignore();
    int q; // Number Q of file names to be analyzed.
    cin >> q; cin.ignore();

    unordered_map<string, string> definition;
    for (int i = 0; i < n; i++) {
        string ext; // file extension
        string mt; // MIME type.
        cin >> ext >> mt; cin.ignore();

        std::transform(ext.begin(), ext.end(), ext.begin(),
            [](unsigned char c) { return std::tolower(c); });
        definition[ext] = mt;
    }
    for (int i = 0; i < q; i++) {
        string fname;
        getline(cin, fname); // One file name per line.

        if (size_t idx{ fname.find_last_of('.') };
            idx != fname.npos)
        {
            string extension{ fname.substr(idx + 1) };
            std::transform(extension.begin(), extension.end(), extension.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (definition.contains(extension))
            {
                cout << definition[extension] << endl;
                continue;
            }

        }
        cout << "UNKNOWN" << endl;
    }
}
