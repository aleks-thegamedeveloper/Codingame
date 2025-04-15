/*
# PUZZLE: https://www.codingame.com/ide/puzzle/telephone-numbers
# The Goal
    By joining the iDroid smartphone development team, you have been given the responsibility of developing the contact manager.
    Obviously, what you were not told is that there are strong technical constraints for iDroid: the system doesn't have much memory and the processor is as fast as a Cyrix from the 90s...
    In the specifications, there are two points in particular that catch your attention:
    1. Intelligent Assistance for entering numbers
    The numbers corresponding to the first digits entered will be displayed to the user almost instantly.
    2. Number storage optimization
    First digits which are common to the numbers should not be duplicated in the memory.
    Fortunately, the specifications also have this little chart to guide you in the implementation (picture).
    Your task is to write a program that displays the number of items (which are numbers) required to store a list of telephone numbers with the structure presented above.

# Game Input
    Line 1: The number N of telephone numbers.
    N following lines: Each line contains a phone number, with a maximum length L. Telephone numbers consist of only the digits 0 to 9 included, without any spaces.

# Output
    The number of elements (referencing a number) stored in the structure.

# Constraints
    0 <= N <= 10000
    2 <= L <= 20
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <memory>

using namespace std;

/**
* For every input phone number
    locate the appropriate root (or create one)
    traverse the tree for that root and populate it with missing nodes
    increment the number of nodes as the new nodes are created

* Tree data structure
    Node is represented by the Entry and an array of child nodes
 **/

struct PhoneNumberNode
{
    char numberElement;
    std::vector<PhoneNumberNode> childNodes;
};

PhoneNumberNode& FindOrCreateNode(std::vector<PhoneNumberNode>* nodes, char desiredElement, uint& outTotalNodeCount)
{
    std::vector<PhoneNumberNode>::iterator desiredNode{ find_if(
        nodes->begin(),
        nodes->end(),
        [desiredElement](const PhoneNumberNode& root) -> bool { return root.numberElement == desiredElement;}) };

    if (desiredNode != nodes->end())
    {
        return *desiredNode;
    }

    PhoneNumberNode& newNode{ nodes->emplace_back(PhoneNumberNode{}) };
    newNode.numberElement = desiredElement;

    ++outTotalNodeCount;

    return newNode;
}

int telephone_numbers_main()
{
    std::vector<PhoneNumberNode> phoneNumberRoots;
    uint totalNodeCount{ 0 };

    int n;
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        string telephone;

        cin >> telephone; cin.ignore();

        std::vector<PhoneNumberNode>* possibleNodesForNextElement{ &phoneNumberRoots };
        for (const char numberElement : telephone)
        {
            PhoneNumberNode& elementNode{ FindOrCreateNode(possibleNodesForNextElement, numberElement, totalNodeCount) };
            possibleNodesForNextElement = &elementNode.childNodes;
        }
    }


    // The number of elements (referencing a number) stored in the structure.
    cout << totalNodeCount << endl;
}
