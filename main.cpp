#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;


struct Request {
    string ip_in;
    string ip_out;
    int time;
    string request_text = 
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n"
        "\r\n";
};

class WebServer{

    private: 

    int time = 5;
    int time_to_finish_current_job;

    public: 

    WebServer() { 
        time_to_finish_current_job = 0;
    }

    void work() { 
        if (time_to_finish_current_job > 0) {
            time_to_finish_current_job -= 1;
        }
    }

    void giveWork(Request r) { 
        time_to_finish_current_job = time;
    }

    bool isWorking() { 
        return time_to_finish_current_job > 0;
    }

};


class LoadBalancer {

    private:

    queue<Request> request_queue;
    vector<WebServer> web_servers;


    public:

    LoadBalancer() { 
        addServers(1);
    }

    LoadBalancer(int s, vector<Request> v) { 
        addServers(s);
        addRequestMultiple(v);
    }

    void addServers(int n) { 
        for (int i = 0; i < n; i++) { 
            web_servers.push_back(WebServer());
        }
    }

    void removeServers(int n) { 
        web_servers.erase(web_servers.end() - n, web_servers.end());
    }

    void addRequest(Request r) { 
        request_queue.push(r);
    }

    void addRequestMultiple(vector<Request> v) { 
        for (Request r : v) { 
            request_queue.push(r);
        }
    }

    void runServers() { 

        for (WebServer server : web_servers) { 
            if (server.isWorking() == false && request_queue.empty() == false) { 
                Request job = request_queue.front();
                request_queue.pop();
                server.giveWork(job);
            }
            else { 
                server.work();
            }
        }

        double request_to_server_ratio = requestSize() / serverSize();

        if (request_to_server_ratio > 100) { 
            addServers(serverSize() * 0.1 + 1);
        }
        else {
            removeServers(serverSize() * 0.1 + 1);
        }

    }

    bool isEmpty() { 
        return request_queue.empty();
    }

    int requestSize() {
        return request_queue.size();
    }

    int serverSize() { 
        return web_servers.size();
    }


};

int randomNumber() {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    int mean = 100;
    int range = 100;
    int output = (dis(gen) * range) + mean;
    return output;
}



int main() { 

    ofstream file("log.txt");
    if (!file) {
        return -1;
    }

    // User Inputs
    int initial_servers;
    int clock_time_limit; 

    cout << "Enter number of initial servers: ";
    cin >> initial_servers;

    cout << "Enter number of initial clock cycles: ";
    cin >> clock_time_limit;


    vector<Request> initial_request_buffer(initial_servers * 100);
    LoadBalancer my_load_balancer(initial_servers, initial_request_buffer);


    int clock_time = 0;

    while (clock_time < clock_time_limit) { 


        file 
            << "Requests in queue: "
            << my_load_balancer.requestSize() 
            << ", Number of Servers: "
            << my_load_balancer.serverSize()
        << endl;


        vector<Request> incoming_requests(randomNumber());
        my_load_balancer.addRequestMultiple(incoming_requests);
        my_load_balancer.runServers();


        clock_time += 1;

    }


    file.close();

    return 0;
}