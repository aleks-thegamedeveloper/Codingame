/*
# PUZZLE
https://www.codingame.com/ide/puzzle/scrabble
# GOAL
When playing Scrabble©, each player draws 7 letters and must find a word that scores the most points using these letters.

A player doesn't necessarily have to make a 7-letter word; the word can be shorter.
The only constraint is that the word must be made using the 7 letters which the player has drawn.

For example, with the letters  etaenhs, some possible words are: ethane, hates, sane, ant.

Your objective is to find the word that scores the most points using the available letters (1 to 7 letters).

~# RULES
In Scrabble, each letter is weighted with a score depending on how difficult it is to place that letter in a word.
You will see below a table showing the points corresponding to each letter:

__________________________________________
| Letters                      | Points  |
|----------------------------------------|
| e, a, i, o, n, r, t, l, s, u |    1    |
| d, g                         |    2    |
| b, c, m, p                   |    3    |
| f, h, v, w, y                |    4    |
| k                            |    5    |
| j, x                         |    8    |
| q, z                         |   10    |
------------------------------------------

The word banjo earns you 3 + 1 + 1 + 8 + 1 = 14 points.

A dictionary of authorized words is provided as input for the program.
The program must find the word in the dictionary which wins the most points for the seven given letters (a letter can only be used once).
If two words win the same number of points, then the word which appears first in the order of the given dictionary should be chosen.


All words will only be composed of alphabetical characters in lower case. There will always be at least one possible word.

# GAME INPUT
Line 1: the number N of words in the dictionary
N following lines: the words in the dictionary. One word per line.
Last line: the 7 letters available.

# OUTPUT
The word that scores the most points using the available letters (1 to 7 letters).
The word must belong to the dictionary. Each letter must be used at most once in the solution. There is always a solution.

# CONSTRAINTS
0 < N < 100000
Words in the dictionary have a maximum length of 30 characters.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int calculateScore(const string& word)
{
    static const unordered_map<char, int> letterScoreMap{
        { 'e', 1 },  { 'a', 1 }, { 'i', 1 }, { 'o', 1 }, { 'n', 1 }, { 'r', 1 }, { 't', 1 }, { 'l', 1 }, { 's', 1 }, { 'u', 1 },
        { 'd', 2 },  { 'g', 2 },
        { 'b', 3 },  { 'c', 3 }, { 'm', 3 }, { 'p', 3 },
        { 'f', 4 },  { 'h', 4 }, { 'v', 4 }, { 'w', 4 }, { 'y', 4 },
        { 'k', 5 },
        { 'j', 8 },  { 'x', 8 },
        { 'q', 10 }, { 'z', 10 }
    };

    int score{ 0 };

    for (const char l : word)
    {
        score += letterScoreMap.at(l);
    }

    return score;
}

bool canConstructWord(const string& word, string letters)
{
    for (const char letter : word)
    {
        const size_t letterPos{ letters.find(letter) };
        if (letterPos == string::npos)
            return false;

        letters.erase(letterPos, 1);
    }

    return true;
}

int scrabble_main()
{
    int n;
    cin >> n; cin.ignore();

    struct WordScore
    {
        string word;
        int score;
    };

    vector<WordScore> wordScores;

    for (int i = 0; i < n; i++) {
        string w;
        getline(cin, w);

        wordScores.emplace_back(w, calculateScore(w));
    }

    string letters;
    getline(cin, letters);

    int bestScore{ -1 };
    string bestWord;

    for (auto& [word, score] : wordScores)
    {
        if (canConstructWord(word, letters) && score > bestScore)
        {
            bestScore = score;
            bestWord = word;
        }
    }

    cout << bestWord << endl;
}
