#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <fstream>

class Graph {
private:

    struct Node {

        std::string vertex;
        int weight;

        Node(std::string name, int val) {
            vertex = name;
            weight = val;
        }

        std::unordered_map<std::string, int> edges;

    };

    std::unordered_map<std::string, int> indexes;

    std::unordered_map<int, std::unordered_map<int, int>> adjList; // adj list of all the actor names and then the unordered map at each actor name contains each actor they share an edge with and its weight
    // weight is designated by how many movies theyve acted in togehter 



public:
    //Graph();
    //void assignCastEdges(std::string csvLine, int& index);
    //int find(std::string name, std::vector<Node>);




    // write function that based on a csv file that has end lines at end of every cast
    // assign each cast member an index in the map
    // pass in the index as well
    // push entire cast into a local vector and iterate through the cast vector
    // for every cast member have a secodn vector to iterate through the cast again and place edges in the corresponding indexes
    // write seperate function that iterates over the csv
    // first funciton just takes in a string line

    int find(std::string name, std::vector<Node> list) { //passed in the vect, see if already have edge

        for (int i = 0; i < list.size(); ++i) {
            if (list.at(i).vertex == name) {
                return i;
            }
        }

        return -1;
    }


    void assignCastEdges(std::string csvLine, int& index) {

        // get each cast members name
        // parse csvLine and delete names and first comma

        std::vector<std::string> castMembers;

        while (csvLine.size() != 0) {
            std::string castName;
            int commaIndex = csvLine.find(',');
            castName = csvLine.substr(0, commaIndex);
            castMembers.push_back(castName);
            if (indexes.count(castName) == 0) {
                indexes[castName] = index;
                ++index;
            }

            csvLine.erase(0, commaIndex + 1);
        }

        // insert edges
        // iterate through cast members and add them to the nodes list of the actor at i


       /* for (int i = 0; i < castMembers.size(); ++i) {
            int currentCast = indexes[castMembers.at(i)];
            for (int j = 0; j < castMembers.size(); ++j) {
                if (castMembers.at(i) == castMembers.at(j)) {
                    continue;
                }
                else {

                    if (find(castMembers.at(j), adjList[currentCast]) == -1) {
                        adjList[currentCast].push_back(Node(castMembers.at(j), 1));
                    }
                    else {
                        adjList[currentCast].at(find(castMembers.at(j), adjList[currentCast])).weight++;
                    }

                }
            }
        }*/


        // double for loop iterating through the cast vector

        for (int i = 0; i < castMembers.size(); ++i) {
            int vertexFrom = indexes[castMembers.at(i)];
            for (int j = 0; j < castMembers.size(); ++j) {
                int vertexTo = indexes[castMembers.at(j)];
                if (vertexFrom == vertexTo) {
                    continue;
                }
                else
                    adjList[vertexFrom][vertexTo]++;
            }
        }



    }



    Graph() {
        
        std::ifstream casts("Moviecast_Data.csv");
        std::string individualCast;
        int index = 0;
        while (getline(casts, individualCast)) {
            assignCastEdges(individualCast, index);
        }
        std::cout << adjList.size() << std::endl;
        std::cout << "Resizing adjacency matrix..." << std::endl;


        


      
    }




};
