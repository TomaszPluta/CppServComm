#include "serv.h"
#include "SocketException.h"
#include "SqqtLib.h"
#include "ThreadPool.h"
#include <string>
#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <ctime> 
#include <cassert>

#include <mysql.h>

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "SqlWrapper.h"


constexpr int ServPort = 1886;
constexpr int PoolSize = 4;
constexpr int ColumnNo = 3;


auto WorkerThread = [&] ( ServerSocket new_sock ,  Hqqt::Broker<ServerSocket> &broker)
{
    try {
        while (1) {
            std::cout << "child process here:" << std::endl;
            std::string frame;
            new_sock >> frame;
            std::cout << "Server got:" << frame <<std::endl;
            std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;
            new_sock << broker.OnReceivedFrame(frame, new_sock);
        }
    } catch (...) {
        std::cout<<"client closed connection"<<std::endl;
        exit(0);
    }
};

std::string GetTimeNow (){
     std::chrono::system_clock::time_point timeNow  = std::chrono::system_clock::now();
     std::time_t timeConv =  std::chrono::system_clock::to_time_t(timeNow);
     std::string tNow=   std::string(ctime(&timeConv));
     return tNow;
}


int main ( int argc, char * argv[] )
{

SqlWrapper MySqlConnector;

MySqlConnector.Connect("localhost","sqqt", "1234","sqqtDB");
std:: string querryRes = MySqlConnector.SendQuerry("SELECT * FROM users");
std::cout << querryRes<<std::endl;


std:: string querryResS =  MySqlConnector.SendQuerry("SELECT CustomerName, City FROM Customers");


    Hqqt::Broker<ServerSocket> broker;
    ThreadPool pool(PoolSize);
    ServerSocket server ( ServPort );
    std::cout << "Server is running...."<<std::endl;

    int CliCntr;
    while(1) {
      ServerSocket  * cliSocket = new(ServerSocket);
      server.accept (*cliSocket );
     
        MySqlConnector.SendQuerry("INSERT INTO users (id, status, timeLogin) VALUES ('"+std::string(cliSocket->get_cli_addr())+"', '"+std::to_string(CliCntr)+"', '"+GetTimeNow()+"')");
        std:: string querryRes3 = MySqlConnector.SendQuerry("SELECT * FROM users");
        std::cout << querryRes3<<std::endl;
        CliCntr++;
      pool.enqueue(WorkerThread, std::ref(*cliSocket), std::ref(broker));
    }



    std::cout << "Server end"<<std::endl;
    return 0;
}






