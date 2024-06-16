#include <iostream>
#include <string>
#include <cmath>
#include <random>

using namespace std;


struct request {
    string ip_in;
    string ip_out;
    int time;
    string request_text = 
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n"
        "\r\n";
};

class webServer{
    private: 
        bool is_working = false;


    public: 

    void work() { 
        is_working = false;
    }

    void giveWork(string ip_in, string ip_out) { 
        is_working = true;
    }


};




int main() { 
    printf("Hello world\n");






    
    return 0;
}