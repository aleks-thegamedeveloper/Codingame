#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    string message;
    getline(cin, message);

    string messageInBinary;
    for (const char c : message)
    {
        constexpr uint8_t mask = 0b1000000;
        for (int i{ 0 }; i < 7; ++i)
        {
            const int bit{ c & (mask >> i) };
            messageInBinary.push_back(bit ? '1' : '0');
        }
    }

    cerr << "Message in binary: " << messageInBinary << endl;

    char characterToMatch{ messageInBinary[0] };
    int characterCount{ 1 };
    for (int i{ 1 }; i < messageInBinary.size(); ++i)
    {
        if (messageInBinary[i] == characterToMatch)
        {
            ++characterCount;
        }
        else
        {
            cout << ((characterToMatch == '1') ? "0" : "00") << " " << string(characterCount, '0') << " ";
            characterToMatch = messageInBinary[i];
            characterCount = 1;
        }
    }
    cout << ((characterToMatch == '1') ? "0" : "00") << " " << string(characterCount, '0');
}
