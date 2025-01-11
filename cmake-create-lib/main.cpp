/*
$ sudo apt install libcurl4-gnutls-dev
$ g++ -std=c++11 -lcurl main.cpp -o main
*/

#include <iostream>
#include <curl/curl.h>

int main()
{
    CURL *curl = curl_easy_init();
    if(curl) 
    {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        res = curl_easy_perform(curl);

        std::cout << res << std::endl;

        curl_easy_cleanup(curl);
    }
    return 0; 
}