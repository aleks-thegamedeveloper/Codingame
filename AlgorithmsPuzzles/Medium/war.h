/*
# PUZZLE
https://www.codingame.com/ide/puzzle/winamax-battle

# The Goal
Let's go back to basics with this simple card game: war!

Your goal is to write a program which finds out which player is the winner for a given card distribution of the "war" game.

# Rules
War is a card game played between two players. Each player gets a variable number of cards of the beginning of the game: that's the player's deck. Cards are placed face down on top of each deck.

Step 1 : the fight
At each game round, in unison, each player reveals the top card of their deck – this is a "battle" – and the player with the higher card takes both the cards played and moves them to the bottom of their stack. The cards are ordered by value as follows, from weakest to strongest:
2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A.

Step 2 : war
If the two cards played are of equal value, then there is a "war". First, both players place the three next cards of their pile face down. Then they go back to step 1 to decide who is going to win the war (several "wars" can be chained). As soon as a player wins a "war", the winner adds all the cards from the "war" to their deck.

Special cases:
If a player runs out of cards during a "war" (when giving up the three cards or when doing the battle), then the game ends and both players are placed equally first.
The test cases provided in this puzzle are built in such a way that a game always ends (you do not have to deal with infinite games)
Each card is represented by its value followed by its suit: D, H, C, S. For example: 4H, 8C, AS.

When a player wins a battle, they put back the cards at the bottom of their deck in a precise order. First the cards from the first player, then the one from the second player (for a "war", all the cards from the first player then all the cards from the second player).

For example, if the card distribution is the following:
Player 1 : 10D 9S 8D KH 7D 5H 6S
Player 2 : 10H 7H 5C QC 2C 4H 6D
Then after one game turn, it will be:
Player 1 : 5H 6S 10D 9S 8D KH 7D 10H 7H 5C QC 2C
Player 2 : 4H 6D

Victory Conditions
A player wins when the other player no longer has cards in their deck.

# Input
Line 1: the number N of cards for player one.
N next lines: the cards of player one.
Next line: the number M of cards for player two.
M next lines: the cards of player two.

# Output
If players are equally first: PAT
Otherwise, the player number (1 or 2) followed by the number of game rounds separated by a space character. A war or a succession of wars count as one game round.

# Constraints
0 < N, M < 1000
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum Outcome
{
    Player1,
    Player2,
    War
};

int cardToValue(string card)
{
    card = card.substr(0, card.size() - 1);

    if (card == "A")
        return 13;
    else if (card == "K")
        return 12;
    else if (card == "Q")
        return 11;
    else if (card == "J")
        return 10;
    else if (card == "10")
        return 9;
    else if (card == "9")
        return 8;
    else if (card == "8")
        return 7;
    else if (card == "7")
        return 6;
    else if (card == "6")
        return 5;
    else if (card == "5")
        return 4;
    else if (card == "4")
        return 3;
    else if (card == "3")
        return 2;
    else if (card == "2")
        return 1;
}

Outcome CompareCards(const string& card1, const string& card2)
{
    int val1 = cardToValue(card1);
    int val2 = cardToValue(card2);

    if (val1 == val2)
        return War;
    if (val1 > val2)
        return Player1;

    return Player2;
}

int war_main()
{
    vector<string> deck1;
    vector<string> deck2;
    int n; // the number of cards for player 1
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        string cardp_1; // the n cards of player 1
        cin >> cardp_1; cin.ignore();

        deck1.emplace_back(cardp_1);
    }
    int m; // the number of cards for player 2
    cin >> m; cin.ignore();
    for (int i = 0; i < m; i++) {
        string cardp_2; // the m cards of player 2
        cin >> cardp_2; cin.ignore();

        deck2.emplace_back(cardp_2);
    }

    vector<string> player1CardsToAppend;
    vector<string> player2CardsToAppend;

    bool bGameOver = false;
    bool bIsPAT = false;
    int rounds{ 0 };
    int winnerId{ -1 };
    while (!bGameOver && !bIsPAT)
    {
        const string top1 = deck1[0];
        const string top2 = deck2[0];

        player1CardsToAppend.emplace_back(top1);
        player2CardsToAppend.emplace_back(top2);

        cerr << "Deck1 size: " << deck1.size() << endl;
        deck1.erase(deck1.begin());
        cerr << "Deck2 size: " << deck2.size() << endl;
        deck2.erase(deck2.begin());

        switch (CompareCards(top1, top2))
        {
        case Player1:
        {
            deck1.insert(deck1.end(), player1CardsToAppend.begin(), player1CardsToAppend.end());
            deck1.insert(deck1.end(), player2CardsToAppend.begin(), player2CardsToAppend.end());
            player1CardsToAppend.clear();
            player2CardsToAppend.clear();

            ++rounds;
            break;
        }
        case Player2:
        {
            deck2.insert(deck2.end(), player1CardsToAppend.begin(), player1CardsToAppend.end());
            deck2.insert(deck2.end(), player2CardsToAppend.begin(), player2CardsToAppend.end());
            player1CardsToAppend.clear();
            player2CardsToAppend.clear();

            ++rounds;
            break;
        }
        case War:
        {
            if (deck1.size() < 4 || deck2.size() < 4)
            {
                bIsPAT = true;
                break;
            }

            player1CardsToAppend.insert(player1CardsToAppend.end(), deck1.begin(), deck1.begin() + 3);
            deck1.erase(deck1.begin(), deck1.begin() + 3);
            player2CardsToAppend.insert(player2CardsToAppend.end(), deck2.begin(), deck2.begin() + 3);
            deck2.erase(deck2.begin(), deck2.begin() + 3);
        }
        }

        if (deck1.empty())
        {
            winnerId = 2;
            bGameOver = true;
        }
        else if (deck2.empty())
        {
            winnerId = 1;
            bGameOver = true;
        }
    }

    if (bIsPAT)
        cout << "PAT" << endl;
    else
        cout << winnerId << " " << rounds;
}
