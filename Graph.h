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

class Graph {
private:



    std::unordered_map<std::string, int> indexes;


    std::unordered_map<int, std::unordered_map<int, int>> adjList; // adj list of all the actor names and then the unordered map at each actor name contains each actor they share an edge with and its weight
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
                    adjList[vertexFrom][vertexTo]++;
                }
                else if (vertexFrom == vertexTo) {
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
        std::cout << "Creating Graph of Actors..." << std::endl;

        while (getline(casts, individualCast)) {
            assignCastEdges(individualCast, index);
        }


    }


};

