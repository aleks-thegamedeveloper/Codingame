// https://www.codingame.com/ide/puzzle/death-first-search-episode-1

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct GraphNode
{
    vector<GraphNode*> linkedNodes;
    int index{ -1 };
    bool isExit{ false };
};

vector<int> Dijkstra_search(const std::vector<GraphNode> graph, GraphNode* startingNode, int targetIdx)
{
    std::unordered_map<int, int> nodeToDistMap;
    unordered_map<int, int> nodeToPrevNodeMap;
    std::vector<const GraphNode*> nodesToVisit;

    for (const GraphNode& node : graph)
    {
        nodeToDistMap[node.index] = INT_MAX - 1;
        nodeToPrevNodeMap[node.index] = -1;
        nodesToVisit.emplace_back(&node);
    }

    nodeToDistMap[startingNode->index] = 0;

    while (!nodesToVisit.empty())
    {
        // find closest node
        auto closestNodeIt = nodesToVisit.end();
        int lowestDist = INT_MAX - 1;
        for (auto it{ nodesToVisit.begin() }; it != nodesToVisit.end(); ++it)
        {
            if (lowestDist > nodeToDistMap[(*it)->index])
            {
                closestNodeIt = it;
                lowestDist = nodeToDistMap[(*it)->index];
            }
        }

        if (closestNodeIt == nodesToVisit.end())
        {
            // no path
            return {};
        }
        const GraphNode* currentNode = *closestNodeIt;
        if (currentNode->index == targetIdx)
        {
            break;
        }
        nodesToVisit.erase(closestNodeIt);

        for (const GraphNode* linkedNode : currentNode->linkedNodes)
        {
            const int newDist{ nodeToDistMap[currentNode->index] + 1 };
            if (newDist < nodeToDistMap[linkedNode->index] + 1)
            {
                nodeToDistMap[linkedNode->index] = newDist;
                nodeToPrevNodeMap[linkedNode->index] = currentNode->index;
            }
        }
    }

    vector<int> path;
    path.emplace_back(targetIdx);
    int currentNode = targetIdx;
    while (currentNode != startingNode->index)
    {
        currentNode = nodeToPrevNodeMap[currentNode];
        path.emplace_back(currentNode);
    }
    return path;
}

int main()
{
    int n; // the total number of nodes in the level, including the gateways
    int l; // the number of links
    int e; // the number of exit gateways
    cin >> n >> l >> e; cin.ignore();

    vector<GraphNode> graphNodes{ vector<GraphNode>(n) };
    for (int i{ 0 }; i < n; ++i)
    {
        graphNodes[i].index = i;
    }

    for (int i = 0; i < l; i++) {
        int n1; // N1 and N2 defines a link between these nodes
        int n2;
        cin >> n1 >> n2; cin.ignore();

        vector<GraphNode>::iterator n1It{ find_if(graphNodes.begin(), graphNodes.end(),
            [n1](const GraphNode& n) { return n.index == n1; }) };
        vector<GraphNode>::iterator n2It{ find_if(graphNodes.begin(), graphNodes.end(),
            [n2](const GraphNode& n) { return n.index == n2; }) };


        n1It->linkedNodes.emplace_back(n2It.base());
        n2It->linkedNodes.emplace_back(n1It.base());
    }

    vector<GraphNode*> exitNodes;
    for (int i = 0; i < e; i++) {
        int ei; // the index of a gateway node
        cin >> ei; cin.ignore();

        vector<GraphNode>::iterator exitNode{ find_if(graphNodes.begin(), graphNodes.end(),
            [ei](const GraphNode& n) { return n.index == ei; }) };

        exitNode->isExit = true;

        exitNodes.emplace_back(exitNode.base());
    }

    // game loop
    while (1) {
        int si; // The index of the node on which the Bobnet agent is positioned this turn
        cin >> si; cin.ignore();

        vector<int> shortestPath;
        int shortestPathCost{ INT_MAX };
        for (GraphNode* exitNode : exitNodes)
        {
            const vector<int>& path{ Dijkstra_search(graphNodes, exitNode, si) };
            if (!path.empty() && path.size() < shortestPathCost)
            {
                shortestPathCost = path.size();
                shortestPath = path;
            }
        }

        const int severedLinkLhs{ shortestPath[0] };
        const int severedLinkRhs{ shortestPath[1] };
        cout << severedLinkLhs << " " << severedLinkRhs << endl;

        auto nodeAToUpdateIt{
            find_if(graphNodes.begin(), graphNodes.end(), [severedLinkLhs](const GraphNode& n) {return n.index == severedLinkLhs; }) };
        nodeAToUpdateIt->linkedNodes.erase(remove_if(
            nodeAToUpdateIt->linkedNodes.begin(),
            nodeAToUpdateIt->linkedNodes.end(),
            [severedLinkRhs](const GraphNode* a) { return a->index == severedLinkRhs; }));

        auto nodeBToUpdateIt{
            find_if(graphNodes.begin(), graphNodes.end(), [severedLinkRhs](const GraphNode& n) {return n.index == severedLinkRhs; }) };
        nodeBToUpdateIt->linkedNodes.erase(remove_if(
            nodeBToUpdateIt->linkedNodes.begin(),
            nodeBToUpdateIt->linkedNodes.end(),
            [severedLinkLhs](const GraphNode* a) { return a->index == severedLinkLhs; }));
    }
}
