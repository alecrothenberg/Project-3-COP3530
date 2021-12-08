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

class Graph {
private:



    std::unordered_map<std::string, int> indexes;


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
    int BFS(std::string from, std::string to) {

        int fromIndex = indexes[from];
        int toIndex = indexes[to];

        std::set<int> visited;
        std::queue<int> que;

        visited.insert(fromIndex);
        que.push(fromIndex);
        int dist = 0;
        while (!que.empty()) {
            int u = que.front();
            que.pop();
            ++dist;
            std::unordered_map<int,int> neighbors = adjList[u];
            for (auto itr : neighbors) {
                if (visited.count(itr.first) == 0) {
                    visited.insert(itr.first);
                    que.push(itr.first);

                    if (itr.first == toIndex) {
                        return dist;
                    }

                }
            }
        }

        return -1;
    }

    bool stringValidation(std::string input) {
        if (indexes.count(input) == 0) {
            return false;
        }
        else { return true; }
    }

    int numNodesRecursion(std::vector<int>&parents, int toIndex, int& numNodes) {
        // take in the toIndex and work backwords to where the parent = - 1

        if (parents.at(toIndex) == -1) {
            return numNodes;
        }
        else {
            ++numNodes;
            numNodesRecursion(parents, parents.at(toIndex), numNodes);
        }

    }

    int dijkstras(std::string from, std::string to) {
    

        int fromIndex = indexes[from];
        int toIndex = indexes[to];

        std::set<int> S;
        S.insert(fromIndex);
        std::set<int> vMinS;
        //int* arrV; // array of the weights of each node to get to the source node
        //int* arrP; // array of the parents of the nodes at each index

        std::vector <int> arrWeight(adjList.size());
        std::vector <int> arrParent(adjList.size());
        arrWeight.at(fromIndex) = 0;
        for (int i = 0; i < adjList.size(); ++i) {
            if (i != fromIndex ) {
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

        int numOfNodes = 0;
        while(!vMinS.empty()) {

            int u = *vMinS.begin();
            for (auto itr : vMinS) {
                if (arrWeight[itr] < arrWeight[u]) {
                    u = itr;
                }
            }
            ++numOfNodes;
            vMinS.erase(u);
            S.emplace(u);
            if (u == toIndex) {
                return numOfNodes; //numNodesRecursion(arrParent, toIndex, numOfNodes);
            }

            // itr through the edges of u 
            // if weight u v + d[u] < d[v]
            // d[v] = w(u,v) +d[u]
            // parent = u

            for (auto itr = adjList.at(u).begin(); itr != adjList.at(u).end(); ++itr) {
                if (arrWeight.at(u) + itr->second < arrWeight.at(itr->first)) {
                    arrWeight.at(itr->first) = arrWeight.at(u) + itr->second;
                    arrParent.at(itr->first) = u;
                }
            }


        }


    
    }



};

