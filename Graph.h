#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <curl/curl.h>



class Graph {
private:
    std::unordered_map<std::string, int> actorNames;

    // use vectors to avoid the const declaration issue with arrays
    std::vector<std::vector<int>> adjMatrix;//(rows, std::vector<int>(cols, 0));

public:
    Graph(int numActors);





};

