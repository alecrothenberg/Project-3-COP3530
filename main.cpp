#include <iostream>
#include <curl/curl.h>

//https://stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string, by The Quantum Physicist
size_t curlToString(void* data, size_t size, size_t nmemb, std::string* s){

    /*
      Pretty sure this just prepares a C-style string and
      adds data to it with append, nothing too crazy
    */
    size_t newSize = size*nmemb;

    /*
      Not entirely sure if the try/catch is necessary,
      but it works so I won't change it for now
     */
    try{
        s->append((char*)data, newSize);
    }
    catch(std::bad_alloc &excpt){
        return 0;
    }

    return newSize;
}

int main() {

    //This would probably be in an API call function, but for now its just the main
    CURL* curl;
    CURLcode result;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if(curl){
        std::string dataString;
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.themoviedb.org/3/movie/362611/credits?api_key=c2cb38dac2b28f5c96780f1f9a396944");

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https

        //Prepping the curl data for the string and assigning output to dataString
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataString);

        //I just printed it, but from here we would call some functions to parse the data and assign it to the graph

        result = curl_easy_perform(curl);


        std::cout << "String: " << dataString;

        // Error check
        if (result != CURLE_OK) {
            std::cerr << "Error during curl request: "
                      << curl_easy_strerror(result) << std::endl;
        }

        curl_easy_cleanup(curl);

    }
    else{
        std::cerr << "Error initializing curl." << std::endl;
    }

    curl_global_cleanup();

    return 0;
}
