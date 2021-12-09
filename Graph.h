#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <fstream>
#include <iterator>
#include <unordered_set>
#include <set>
#include <queue>
#include <stack>

class Graph {
private:



    std::unordered_map<std::string, int> indexes;
    std::unordered_map<int, std::string> reverseIndexes;

    std::unordered_map<int, std::unordered_map<int,int>> adjList; // adj list of all the actor names and then the unordered map at each actor name contains each actor they share an edge with and its weight
    // weight is designated by how many movies theyve acted in togehter 



public:

    // write function that based on a csv file that has end lines at end of every cast
    // assign each cast member an index in the map
    // pass in the index as well
    // push entire cast into a local vector and iterate through the cast vector
    // for every cast member have a secodn vector to iterate through the cast again and place edges in the corresponding indexes
    // write seperate function that iterates over the csv
    // first funciton just takes in a string line




    void assignCastEdges(std::string csvLine, int& index) {

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
                    if (adjList[vertexFrom][vertexTo] == 0) {
                        adjList[vertexFrom][vertexTo]++;
                    }
                    else {
                        adjList[vertexFrom][vertexTo] *= (1.0 / 1.1);
                    }
                }
                else if (vertexFrom == vertexTo) {
                    continue;//You're bad at C++
                }
                else {
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

    Graph() {

        std::ifstream casts("Moviecast_Data.csv");
        std::string individualCast;
        int index = 0;
        std::cout << "Creating Graph of Actors..." << std::endl;

        while (getline(casts, individualCast)) {
            assignCastEdges(individualCast, index);
        }
       


    }

    //Gonna need to change BFS to fit weighted map
    

    bool stringValidation(std::string input) {
        if (indexes.count(input) == 0) {
            return false;
        }
        else { return true; }
    }

    std::vector<std::string> numNodes(std::vector<int> parents, int toIndex) {
        // take in the toIndex and work backwords to where the parent = - 1
        int numNodes = 0;
        std::vector<std::string> indexesPassed;
        while (parents.at(toIndex) != -1) {
            indexesPassed.push_back(reverseIndexes[toIndex]);
            toIndex = parents.at(toIndex);
        }
        return indexesPassed;
    }

    std::pair<int, std::vector<std::string>> dijkstras(std::string from, std::string to) {
    

        int fromIndex = indexes[from];
        int toIndex = indexes[to];

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
        
        while(!vMinS.empty()) {

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
                    std::vector<std::string> nodePars = numNodes(arrParent, toIndex);
                    int numTraversed;
                    if (nodePars.size() == 1) {
                        numTraversed = 1;
                    }
                    else if (nodePars.size() == 0) {
                        numTraversed = 0;
                    }
                    else
                        numTraversed = nodePars.size() - 1;

                    nodePars.push_back(from);
                    
                    std::pair<int, std::vector<std::string>> toReturn = std::make_pair(numTraversed, nodePars);

                    return toReturn;
                }
                
            }


        }


        std::vector<std::string> bad = { "no connection" };
        std::pair<int, std::vector<std::string>> badPair = std::make_pair(-1, bad);
        return badPair;
    }

    std::pair<int, std::vector<std::string>> BFS(std::string from, std::string to) {

        int fromIndex = indexes[from];
        int toIndex = indexes[to];

        if (toIndex == fromIndex) {
            std::vector<std::string> actor = { "Same Actor!" };
            std::pair<int, std::vector<std::string>> same = std::make_pair(0, actor);
            return same;
        }


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
                        std::vector<std::string> nodePars = numNodes(parents, toIndex);
                        int numTraversed;

                        if (nodePars.size() == 1) {
                            numTraversed = 1;
                        }
                        else if (nodePars.size() == 0) {
                            numTraversed = 0;
                        }
                        else
                            numTraversed = nodePars.size() - 1;

                        nodePars.push_back(from);
                        std::pair<int, std::vector<std::string>> toReturn = std::make_pair(numTraversed, nodePars);

                        return toReturn;
                    }
                }
            }
        }
        std::vector<std::string> bad = { "no connection" };
        std::pair<int, std::vector<std::string>> badPair = std::make_pair(-1, bad);
        return badPair;
    }

    std::pair<int, std::vector<std::string>> DFS(std::string from, std::string to) {

        int fromIndex = indexes[from];
        int toIndex = indexes[to];

        if (toIndex == fromIndex) {
            std::vector<std::string> actor = { "Same Actor!" };
            std::pair<int, std::vector<std::string>> same = std::make_pair(0, actor);
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
                        std::vector<std::string> nodePars = numNodes(parents, toIndex);
                        int numTraversed;
                        if (nodePars.size() == 1) {
                            numTraversed = 1;
                        }
                        else if (nodePars.size() == 0) {
                            numTraversed = 0;
                        }
                        else
                            numTraversed = nodePars.size() - 1;

                        nodePars.push_back(from);

                        std::pair<int, std::vector<std::string>> toReturn = std::make_pair(numTraversed, nodePars);

                        return toReturn;
                    }
                }
            }
        }
        std::vector<std::string> bad = { "no connection" };
        std::pair<int, std::vector<std::string>> badPair = std::make_pair(-1, bad);
        return badPair;
    }


};

