#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <queue>
#include <vector>

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
        for (int i = 0; i < 20; i++) {
            web_servers.push_back(WebServer());
        }
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
    }

    bool isEmpty() { 
        return request_queue.empty();
    }

    int size() {
        return request_queue.size();
    }


};



int main() { 
    printf("Hello world\n");




    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> normalDist(5.0, 1);

    for (int i = 0; i < 20; i++) { 
        cout << int(normalDist(gen)) << endl;
    }






    int clock_time = 0;


 

    // both of these should be user input
    int servers = 100;
    int time; // i interperet this as the time it takes to process a file

    int initial_buffer_size = servers * 100;

    vector<Request> request_buffer(initial_buffer_size);

    LoadBalancer my_load_balancer;
    my_load_balancer.addRequestMultiple(request_buffer);


    while (my_load_balancer.isEmpty() == false) { 

        vector<Request> incoming_requests(3);
        my_load_balancer.addRequestMultiple(incoming_requests);

        my_load_balancer.runServers();

        cout << my_load_balancer.size() << endl;




        clock_time += 1;


    }










    return 0;
}