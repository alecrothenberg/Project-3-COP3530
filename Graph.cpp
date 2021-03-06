#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <unordered_set>
#include <set>
#include <queue>
#include <stack>
#include <chrono>
#include "Graph.h"


void Graph::assignCastEdges(std::string csvLine, int& index) {
    
    // get each cast members name
    // parse csvLine and delete names and first comma

    std::unordered_set<std::string> castMembers;
    std::string castName;
    int random = std::rand() % 102775;
    while (csvLine.size() != 0) {

        int commaIndex = csvLine.find(',');
        castName = csvLine.substr(0, commaIndex);
        castMembers.insert(castName);
        if (indexes.find(castName) == indexes.end()) {
            indexes[castName] = index;
            reverseIndexes[index] = castName;
            ++index;
        }


        csvLine.erase(0, commaIndex + 1);
    }


    for (auto itr1 : castMembers) {
        int vertexFrom = indexes[itr1];
        for (auto itr2 : castMembers) {
            int vertexTo = indexes[itr2];
            if (castMembers.size() == 1) {
                //Create entry for person, but do not connect
                adjList[vertexFrom];
            }
            else if (vertexFrom != vertexTo) {
                //Creates and connects entry, increments edge if extant
                if (adjList[vertexFrom][vertexTo] == 0) {
                    adjList[vertexFrom][vertexTo]++;
                }
                else {
                    adjList[vertexFrom][vertexTo] *= (1.0 / 1.1);
                }
            }
        }
    }
}

Graph::Graph() {

    std::ifstream casts("Moviecasts_50k.csv");
    std::string individualCast;
    int index = 0;
   // std::cout << "Creating Graph of Actors..." << std::endl;

    while (getline(casts, individualCast)) {
       assignCastEdges(individualCast, index);

    }
    



}


std::vector<std::string> Graph::numNodes(std::vector<int> parents, int toIndex) {
    // take in the toIndex and work backwords to where the parent = - 1
    int numNodes = 0;
    std::vector<std::string> indexesPassed;
    while (parents.at(toIndex) != -1) {
        indexesPassed.push_back(reverseIndexes[toIndex]);
        toIndex = parents.at(toIndex);
    }
    return indexesPassed;
}

std::pair<std::pair<long long, int>, std::vector<std::string>> Graph::dijkstras(std::string from, std::string to) {
    auto startSearchDijk = std::chrono::high_resolution_clock::now();
    int fromIndex = indexes[from];
    int toIndex = indexes[to];

    if (toIndex == fromIndex) {
        //Returns a the data package if the actor is found
        std::vector<std::string> actor = { "Same Actor!" };
        auto elapsedSearchDijk = std::chrono::high_resolution_clock::now() - startSearchDijk;
        long long microsecondsDijk = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchDijk).count();
        std::pair<long long, int> timeAndNum = std::make_pair(microsecondsDijk, 0);
        std::pair<std::pair<long long, int>, std::vector<std::string>> same = std::make_pair(timeAndNum, actor);

        return same;
    }

    std::set<int> S;
    S.insert(fromIndex);
    std::set<int> vMinS;
    // array of the weights of each node to get to the source node
    // array of the parents of the nodes at each index

    std::vector <int> arrWeight(adjList.size());
    std::vector <int> arrParent(adjList.size());
    arrWeight.at(fromIndex) = 0;
    arrParent.at(fromIndex) = -1;


    for (int i = 0; i < adjList.size(); ++i) {
        if (i != fromIndex) {
            arrWeight.at(i) = INT_MAX;
        }
    }

    for (int i = 0; i < adjList.size(); ++i) {
        if (i == fromIndex) {
            for (auto itr = adjList.at(fromIndex).begin(); itr != adjList.at(fromIndex).end(); ++itr) {
                arrWeight.at(itr->first) = itr->second;
                arrParent.at(itr->first) = fromIndex;
            }
        }
        else {
            vMinS.insert(i);
            if (arrParent.at(i) != fromIndex) { arrParent.at(i) = -1; }
            // iterate through vector of pairs at src
            // if it has and edge set d[v] to weight
        }
    }

    while (!vMinS.empty()) {

        int u = *vMinS.begin();
        for (auto itr : vMinS) {
            if (arrWeight[itr] < arrWeight[u]) {

                u = itr;
            }
        }

        vMinS.erase(u);
        S.emplace(u);


        // itr through the edges of u 
        // if weight u v + d[u] < d[v]
        // d[v] = w(u,v) +d[u]
        // parent = u

        for (auto itr = adjList.at(u).begin(); itr != adjList.at(u).end(); ++itr) {
            if (arrWeight.at(u) + itr->second < arrWeight.at(itr->first)) {
                arrWeight.at(itr->first) = arrWeight.at(u) + itr->second;
                arrParent.at(itr->first) = u;
            }
            if (itr->first == toIndex) {
                auto elapsedSearchDijk = std::chrono::high_resolution_clock::now() - startSearchDijk;
                long long microsecondsDijk = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchDijk).count();

                std::vector<std::string> nodePars = numNodes(arrParent, toIndex);
                int numTraversed;
                if (nodePars.size() == 1) {
                    numTraversed = 1;
                }
                else
                    numTraversed = nodePars.size() - 1;

                nodePars.push_back(from);

                std::pair<long long, int> timeAndNum = std::make_pair(microsecondsDijk, numTraversed);

                std::pair<std::pair<long long, int>, std::vector<std::string>> toReturn = std::make_pair(timeAndNum, nodePars);

                return toReturn;
            }

        }


    }

    //Returns a data package with flags for a disconnection
    auto elapsedSearchDijk = std::chrono::high_resolution_clock::now() - startSearchDijk;
    long long microsecondsDijk = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchDijk).count();
    std::vector<std::string> bad = { "no connection" };
    std::pair<long long, int> longTime = std::make_pair(microsecondsDijk, -1);
    std::pair<std::pair<long long, int>, std::vector<std::string>> badPair = std::make_pair(longTime, bad);
    return badPair;
}


std::pair<std::pair<long long, int>, std::vector<std::string>> Graph::BFS(std::string from, std::string to) {

    auto startSearchBFS = std::chrono::high_resolution_clock::now();
    int fromIndex = indexes[from];
    int toIndex = indexes[to];

    if (toIndex == fromIndex) {
        std::vector<std::string> actor = { "Same Actor!" };
        auto elapsedSearchBFS = std::chrono::high_resolution_clock::now() - startSearchBFS;
        long long microsecondsBFS = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchBFS).count();
        std::pair<long long, int> timeAndNum = std::make_pair(microsecondsBFS, 0);
        std::pair<std::pair<long long, int>, std::vector<std::string>> same = std::make_pair(timeAndNum, actor);

        return same;
    }

    //Standard BFS search, uses queue to traverse a graph with a set to avoid previously visited vertices 
    std::set<int> visited;
    std::queue<int> que;
    std::vector<int> parents(adjList.size());
    parents.at(fromIndex) = -1;

    visited.insert(fromIndex);
    que.push(fromIndex);
    int dist = 0;
    while (!que.empty()) {
        int u = que.front();
        que.pop();
        //++dist;
        std::unordered_map<int, int> neighbors = adjList[u];
        for (auto itr : neighbors) {
            if (visited.count(itr.first) == 0) {
                visited.insert(itr.first);
                que.push(itr.first);
                parents.at(itr.first) = u;
                if (itr.first == toIndex) {
                    auto elapsedSearchBFS = std::chrono::high_resolution_clock::now() - startSearchBFS;
                    long long microsecondsBFS = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchBFS).count();

                    std::vector<std::string> nodePars = numNodes(parents, toIndex);
                    int numTraversed;
                    if (nodePars.size() == 1) {
                        numTraversed = 1;
                    }
                    else
                        numTraversed = nodePars.size() - 1;

                    nodePars.push_back(from);

                    std::pair<long long, int> timeAndNum = std::make_pair(microsecondsBFS, numTraversed);

                    std::pair<std::pair<long long, int>, std::vector<std::string>> toReturn = std::make_pair(timeAndNum, nodePars);

                    return toReturn;
                }
            }
        }
    }
    auto elapsedSearchBFS = std::chrono::high_resolution_clock::now() - startSearchBFS;
    long long microsecondsBFS = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchBFS).count();
    std::vector<std::string> bad = { "no connection" };
    std::pair<long long, int> longTime = std::make_pair(microsecondsBFS, -1);
    std::pair<std::pair<long long, int>, std::vector<std::string>> badPair = std::make_pair(longTime, bad);
    return badPair;
}

//Standard DFS with stack and visited set, similar to BFS
std::pair<std::pair<long long, int>, std::vector<std::string>> Graph::DFS(std::string from, std::string to) { // time and then node number
    auto startSearchDFS = std::chrono::high_resolution_clock::now();

    int fromIndex = indexes[from];
    int toIndex = indexes[to];

    if (toIndex == fromIndex) {
        std::vector<std::string> actor = { "Same Actor!" };
        auto elapsedSearchDFS = std::chrono::high_resolution_clock::now() - startSearchDFS;
        long long microsecondsDFS = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchDFS).count();
        std::pair<long long, int> timeAndNum = std::make_pair(microsecondsDFS, 0);
        std::pair<std::pair<long long, int>, std::vector<std::string>> same = std::make_pair(timeAndNum, actor);

        return same;
    }

    std::set<int> visited;
    std::stack<int> sta;
    std::vector<int> parents(adjList.size());
    parents.at(fromIndex) = -1;

    visited.insert(fromIndex);
    sta.push(fromIndex);
    int dist = 0;
    while (!sta.empty()) {
        int u = sta.top();
        sta.pop();
        //++dist;
        std::unordered_map<int, int> neighbors = adjList[u];
        for (auto itr : neighbors) {
            if (visited.count(itr.first) == 0) {
                visited.insert(itr.first);
                sta.push(itr.first);
                parents.at(itr.first) = u;
                if (itr.first == toIndex) {
                    auto elapsedSearchDFS = std::chrono::high_resolution_clock::now() - startSearchDFS;
                    long long microsecondsDFS = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchDFS).count();

                    std::vector<std::string> nodePars = numNodes(parents, toIndex);
                    int numTraversed;
                    if (nodePars.size() == 1) {
                        numTraversed = 1;
                    }
                    else
                        numTraversed = nodePars.size() - 1;

                    nodePars.push_back(from);

                    std::pair<long long, int> timeAndNum = std::make_pair(microsecondsDFS, numTraversed);

                    std::pair<std::pair<long long, int>, std::vector<std::string>> toReturn = std::make_pair(timeAndNum, nodePars);

                    return toReturn;
                }
            }
        }
    }
    auto elapsedSearchDFS = std::chrono::high_resolution_clock::now() - startSearchDFS;
    long long microsecondsDFS = std::chrono::duration_cast<std::chrono::microseconds>(elapsedSearchDFS).count();
    std::vector<std::string> bad = { "no connection" };
    std::pair<long long, int> longTime = std::make_pair(microsecondsDFS, -1);
    std::pair<std::pair<long long, int>, std::vector<std::string>> badPair = std::make_pair(longTime, bad);
    return badPair;
}

//Returns the indexes map to get name data for wxComboboxes
std::unordered_map<std::string, int> Graph::getNames() {
    return indexes;
}
