#include "Graph.h"
#include <vector>

Graph::Graph(int actors) {
    // need to intialize the 2D array to contain all 0's to start
    
    for (int i = 0; i < actors; ++i) { // assigns every value to 0
        adjMatrix.at(i).resize(actors, 0);
    }
    
    


}
