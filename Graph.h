#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <curl/curl.h>

//https://stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string, by The Quantum Physicist
size_t curlToString(void* data, size_t size, size_t nmemb, std::string* s) {

    /*
      Pretty sure this just prepares a C-style string and
      adds data to it with append, nothing too crazy
    */
    size_t newSize = size * nmemb;

    /*
      Not entirely sure if the try/catch is necessary,
      but it works so I won't change it for now
     */
    try {
        s->append((char*)data, newSize);
    }
    catch (std::bad_alloc& excpt) {
        return 0;
    }

    return newSize;
}


// prints all the names
// use this to return a vector using getCast and adjust the url every time


class Graph {
private:
    std::unordered_map<std::string, int> actorNames;

    // use vectors to avoid the const declaration issue with arrays
    std::vector<std::vector<int>> adjMatrix;//(rows, std::vector<int>(cols, 0));



public:
    void parseAndFill(std::string jsString, int& index) {
        std::size_t found = jsString.find("name");

        if (found != std::string::npos) {

            /*
             * Find first instance of "name", erase the
             * rest so I can find the right " char after
             * and start at the beginning of the name
            */
            jsString.erase(0, found + 7);

            //Find next " char that lies at the end of the name
            found = jsString.find('\"');

            actorNames[jsString.substr(0, found)] = index;



            /*
             * Get rid of content before name and up to
             * "original_name" section so that find doesn't find it
             */

            jsString.erase(0, found + 15);
            index++;
            parseAndFill(jsString, index);
        }
    }


    void apiCall(const char* apiAddress) {
        //This would probably be in an API call function, but for now its just the main
        CURL* curl;
        CURLcode result;

        curl_global_init(CURL_GLOBAL_DEFAULT);

        curl = curl_easy_init();

        if (curl) {
            std::string dataString;
            curl_easy_setopt(curl, CURLOPT_URL, apiAddress);

            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https

            //Prepping the curl data for the string and assigning output to dataString
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToString);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);

            result = curl_easy_perform(curl);

            //Fuck with us and then we tweakin hoe (tweakin hoe)
            int index = 0;
            parseAndFill(dataString, index);

            //Testing getCast
            /*for (const auto& person : movieCast) {
                std::cout << person << std::endl;
            }
            */
            // Error check
            if (result != CURLE_OK) {
                std::cerr << "Error during curl request: "
                          << curl_easy_strerror(result) << std::endl;
            }

            curl_easy_cleanup(curl);
        }
        else {
            std::cerr << "Error initializing curl." << std::endl;
        }

        curl_global_cleanup();
    }

    void actorMap() {

        //insert a number starting at 62 right after movie/

        std::cout << "Making API calls..." << std::endl;
        for (int i = 62; i < 5000; ++i) {


            // string find returns start of movie/
            // add 5 to index of find to insert
            std::string apiAddress = "https://api.themoviedb.org/3/movie//credits?api_key=c2cb38dac2b28f5c96780f1f9a396944";
            int startofMovie = apiAddress.find("movie/");
            const char* API;
            std::string iString = std::to_string(i);
            //apiAddress.erase(startofMovie + 6, iString.size());
            apiAddress.insert(startofMovie + 6, iString);
            API = apiAddress.c_str();

            apiCall(API);
        }
    }

    Graph(){
        // need to intialize the 2D array to contain all 0's to start

        actorMap();

        std::cout << actorNames.size() << std::endl;
        std::cout << "Resizing adjacency matrix..." << std::endl;
        adjMatrix.resize(actorNames.size());
        for (int i = 0; i < actorNames.size(); ++i) { // assigns every value to 0
            adjMatrix.at(i).resize(actorNames.size(), 0);
        }

        //apiCall("https://api.themoviedb.org/3/movie/63/credits?api_key=c2cb38dac2b28f5c96780f1f9a396944");
        // array is intialied
        // need to have a seperate function that places everything into the unordered map and assigns an index
        // probably need to change the constructor to no int taken in
    }

};

