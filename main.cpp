#include <iostream>
#include <cstring>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "client.cpp"

using namespace std;

int main(int argc, char *argv[]){

    const int t_num = 1;
    echo_session *sessions[t_num];

    memset(sessions, 0 ,sizeof(sessions));
    try{
        if(argc != 3){
            std::cerr << "Usage:<host> <port>\n";
            return 1;
        }
        boost::asio::io_service io_service;
        for(int i=0; i<t_num; i++){
            sessions[i] = new echo_session(io_service);
            sessions[i]->start(argv[1], argv[2]);
        }

        io_service.run();

        for(int i=0; i<t_num; i++)
            if(sessions[i] != NULL)
                delete sessions[i];
    }catch(std::exception &e){
        for(int i=0; i<t_num; i++)
            if(sessions[i] != NULL)
                delete sessions[i];
        std::cerr << "Exception:" << e.what() << "\n";
    }
    return 0;
}
