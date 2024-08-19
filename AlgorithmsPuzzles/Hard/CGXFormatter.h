#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

class CGXFormatter
{
public:
    static string CleanupMessage(const string& message)
    {
        bool isParsingString{ false };

        string cleanedUpMessage;
        for (const char c : message)
        {
            if ((c == ' ' || c == '	') && !isParsingString)
                continue;

            if (c == '\'')
            {
                isParsingString = !isParsingString;
            }

            cleanedUpMessage += c;
        }

        return cleanedUpMessage;
    }

    static void FormatCGX(const string& cgxString, int currentBlockDepth = 0)
    {
        for (size_t i{ 0 }; i <= cgxString.length() - 1; ++i)
        {
            if (cgxString[i] == ';')
            {
                cout << ";" << endl;
                continue;
            }

            if (cgxString[i] == '(')
            {
                string::const_iterator blockStartIt{ cgxString.begin() + i + 1 };
                string::const_iterator blockEndIt{ FindBlockEndIt(blockStartIt, cgxString.end()) };

                DisplayBlock(string(blockStartIt, blockEndIt), currentBlockDepth + 1);
                i = blockEndIt - cgxString.begin();

                continue;
            }
            if (cgxString[i] == '\'')
            {
                string::const_iterator valueBegin;
                if (IsNextTermKeyValuePair(cgxString.begin() + i, cgxString.end(), valueBegin))
                {
                    const string keyStr{ string(cgxString.begin() + i, valueBegin) };
                    DisplayPrimitive(keyStr, currentBlockDepth + 1);
                    if (*valueBegin == '(')
                    {
                        cout << endl;
                        string::const_iterator blockEndIt{ FindBlockEndIt(valueBegin + 1, cgxString.end()) };

                        DisplayBlock(string(valueBegin + 1, blockEndIt), currentBlockDepth + 1);
                        i = blockEndIt - cgxString.begin();
                    }
                    else
                    {
                        string primitiveStr;
                        if (IsNextTermPrimitive(valueBegin, cgxString.end(), primitiveStr))
                        {
                            DisplayPrimitive(primitiveStr);
                            i += keyStr.size() + primitiveStr.size();
                        }

                        if (i + 1 <= cgxString.length() - 1)
                        {
                            cout << ";" << endl;
                        }
                    }

                    continue;
                }
            }

            string primitiveStr;
            if (IsNextTermPrimitive(cgxString.begin() + i, cgxString.end(), primitiveStr))
            {
                DisplayPrimitive(primitiveStr, currentBlockDepth + 1);
                i += primitiveStr.size();
            }

            if (i + 1 <= cgxString.length() - 1)
            {
                cout << ";" << endl;
            }
        }
    }

private:
    static string::const_iterator FindBlockEndIt(string::const_iterator begin, string::const_iterator end)
    {
        size_t closingBracketsToFind{ 1 };
        while (begin != end)
        {
            const char c{ *begin };
            if (c == ')' && --closingBracketsToFind == 0)
            {
                return begin;
            }
            if (c == '(')
            {
                ++closingBracketsToFind;
            }

            ++begin;
        }
    }

    static bool IsNextTermPrimitive(
        string::const_iterator begin, string::const_iterator end, string& outBooleanPrimitiveString)
    {
        outBooleanPrimitiveString = "";
        smatch matches;

        if (regex_search(begin, end, matches, k_booleanPrimitiveRegex))
        {
            outBooleanPrimitiveString = matches[0];
            return true;
        }
        else if (regex_search(begin, end, matches, k_numberPrimitiveRegex))
        {
            outBooleanPrimitiveString = matches[0];
            return true;
        }
        else if (regex_search(begin, end, matches, k_stringPrimitiveRegex))
        {
            outBooleanPrimitiveString = matches[0];
            return true;
        }

        return false;
    }

    static  bool IsNextTermKeyValuePair(
        string::const_iterator begin, string::const_iterator end, string::const_iterator& outValueBegin) {
        outValueBegin = end;

        smatch matches;
        if (regex_search(begin, end, matches, regex(R"('[^']*'=)"))) {
            outValueBegin = begin + string(matches[0]).size();
            return true;
        }

        return false;
    }

    static void DisplayPrimitive(const string& primitiveStr, int currentBlockDepth = 0)
    {
        cout << string(currentBlockDepth * 4, ' ') << primitiveStr;
    }

    static void DisplayBlock(const string& blockStr, int currentBlockDepth = 0)
    {
        cout << string(currentBlockDepth * 4, ' ') << '(' << endl;

        if (blockStr.empty())
        {
            cout << string(currentBlockDepth * 4, ' ') << ')';
            return;
        }

        FormatCGX(blockStr, currentBlockDepth);

        cout << endl << string(currentBlockDepth * 4, ' ') << ')';

    }

    inline static const regex k_booleanPrimitiveRegex{ R"(^(true|false|null)\b)" }; // treat null as a special case here for simplicity
    inline static const regex k_numberPrimitiveRegex{ "^\\d+" };
    inline static const regex k_stringPrimitiveRegex{ R"(^'[^']*'(?!\=))" };

};

int main()
{
    int n;
    cin >> n; cin.ignore();
    string message;
    for (int i = 0; i < n; i++) {
        string cgxline;
        getline(cin, cgxline);
        message.append(cgxline);
    }

    cerr << "Original message:" << endl << message << endl;
    const string& cleanedUpMessage = CGXFormatter::CleanupMessage(message);
    cerr << "Cleaned up message:" << endl << cleanedUpMessage << endl;

    CGXFormatter::FormatCGX(cleanedUpMessage, -1);
}
