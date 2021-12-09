#include <unordered_map>
#include <string>
#include <vector>



class Graph {
private:



    std::unordered_map<std::string, int> indexes;
    std::unordered_map<int, std::string> reverseIndexes;

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




    void assignCastEdges(std::string csvLine, int& index);

    Graph();

    //Gonna need to change BFS to fit weighted map


    bool stringValidation(std::string input);
       

    std::vector<std::string> numNodes(std::vector<int> parents, int toIndex);
        

    std::pair<std::pair<long long, int>, std::vector<std::string>> dijkstras(std::string from, std::string to);


    std::pair<std::pair<long long, int>, std::vector<std::string>> BFS(std::string from, std::string to);

    std::pair<std::pair<long long, int>, std::vector<std::string>> DFS(std::string from, std::string to);

    std::unordered_map<std::string, int> getNames();

    

};

