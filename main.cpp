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
#include <utility>
#include <mysql.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "queue"
#include "SqttClient.h"

#include "SqlWrapper.h"
#include <fstream>

constexpr int ServPort = 1886;
constexpr int PoolSize = 4;
constexpr int ColumnNo = 3;

std::queue <std::pair <std::string, SqttClient *>> msgQueue;
std::mutex msgMutex;
std::unique_lock<std::mutex>msgLock(msgMutex);


auto WorkerThread = [&] ( ServerSocket new_sock ,  Hqqt::Broker<SqttClient> &broker)
{
    try {
        SqttClient * socketCli = new ClientSocket(new_sock);
        while (1) {
            std::cout << "child process here:" << std::endl;
            std::string frame;
            new_sock >> frame;
            std::cout << "Server got:" << frame <<std::endl;
            std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;
            msgLock.lock();
            
            msgQueue.push(std::make_pair(frame, socketCli));
            
        }
    } catch (...) {
        std::cout<<"client closed connection"<<std::endl;
        exit(0);
    }
};


auto brokerThread = [&] (Hqqt::Broker<SqttClient> &broker)
{
    msgLock.lock();
    while(msgQueue.empty());
    auto msgCli = msgQueue.front();
    msgQueue.pop();
    broker.OnReceivedFrame(msgCli.first, msgCli.second);
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

    Hqqt::Broker<SqttClient> broker;
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






