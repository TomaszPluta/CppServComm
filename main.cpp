#include "serv.h"
#include "SocketException.h"
#include "SqqtLib.h"
#include "ThreadPool.h"

#include <string>
#include <iostream>
#include <functional>
#include <thread>


//std::string GetFrame(){
//;
//}


void SocketSend(std::string addr, std::string msg)
{

}


auto t1Lambda = [&] ( ServerSocket new_sock )
{

    Hqqt::Broker<ServerSocket> broker;
    try {
        while (1) {
            std::cout << "child process here:" << std::endl;
            std::string frame;
            new_sock >> frame;

            std::cout << "Server got:" << frame <<std::endl;
            std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;
            std::string addr;
            //	  frame = GetFrame();
            new_sock << broker.OnReceivedFrame(frame, new_sock);
        }
    } catch (...) {
        std::cout<<"client closed connection"<<std::endl;
        exit(0);
    }
};


void t1 ( ServerSocket new_sock,    Hqqt::Broker<ServerSocket> & broker)
{
    //Hqqt::Broker<ServerSocket> broker;
    try {
        while (1) {
              std::cout<<"- - - - - - - - "<<std::endl;
            std::string frame;
            new_sock >> frame;

            std::cout << "Server got:" << frame <<std::endl;
            std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;
            std::string addr;
            //	  frame = GetFrame();
            new_sock << broker.OnReceivedFrame(frame, new_sock);
        }
    } catch (...) {
        std::cout<<"client closed connection"<<std::endl;
        exit(0);
    }
}

//get to know:
//https://www.modernescpp.com/index.php/c-core-guidelines-be-aware-of-the-traps-of-condition-variables




constexpr int ServPort = 1886;
constexpr int PoolSize = 4;

int main ( int argc, char * argv[] )
{
    Hqqt::Broker<ServerSocket> broker;
    ThreadPool pool(PoolSize);

    std::cout << "Server is running...."<<std::endl;
    ServerSocket server ( ServPort );
    
  
    while(1) {
        ServerSocket  * cliSocket = new(ServerSocket);

        server.accept (*cliSocket );
        pool.enqueue(t1, std::ref(*cliSocket), std::ref(broker));
    }



    std::cout << "Server end"<<std::endl;
    return 0;
}






