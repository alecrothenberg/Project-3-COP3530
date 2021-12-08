#include <iostream>
#include <vector>
#include "Graph.h"








int main() {
    /*
    std::string apiAddress = "https://api.themoviedb.org/3/movie//credits?api_key=c2cb38dac2b28f5c96780f1f9a396944";
    std::cout << apiAddress << std::endl;
    int startofMovie = apiAddress.find("movie/");
    apiAddress.insert(startofMovie + 6, std::to_string(62));
    std::cout << apiAddress << std::endl;
    */
    Graph graff = Graph();

    std::cout << "What would you like to do?" << std::endl;
    std::string action;
    std::cin >> action;
    std::string actor1;
    std::string actor2;

    if (action == "BFS") {
        std::cout << "Actor 1: ";
        std::cin >> actor1;

        std::cout << "Actor 2: ";
        std::cin >> actor2;

        if (!graff.stringValidation(actor1) || !graff.stringValidation(actor2)) {
            std::cout << "Invalid Actor!" << std::endl;
        }
        else
            graff.BFS(actor1, actor2);

    }
    else if (action == "dijkstras") {
        // std::cout << "Actor 1: ";
        //std::cin >> actor1;

        //std::cout << "Actor 2: ";
        //std::cin >> actor2;

        std::cout << graff.dijkstras("Bruce Willis", "Danny DeVito") << "" << graff.BFS("Bruce Willis", "Danny Devito");
    }






    return 0;
}
