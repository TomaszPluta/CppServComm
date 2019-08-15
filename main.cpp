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

#include "SqttClientItf.h"
#include "SqlWrapper.h"
#include <fstream>

#include "SharedQueue.h"
constexpr int ServPort = 1886;
constexpr int PoolSize = 4;
constexpr int ColumnNo = 3;

SharedQueue <std::pair <std::string, Hqqt::Client *>> msgQueue;



auto WorkerThread = [&] ( ServerSocket new_sock ,  Hqqt::Broker &broker)
{
    try {
        SqttClient * socketCli = new ClientSocket(new_sock);
        while (1) {
            std::cout << "child process here:" << std::endl;
            std::string frame;
            new_sock >> frame;
            std::cout << "Server got:" << frame <<std::endl;
            std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;

            msgQueue.Push(std::make_pair(frame, socketCli));
            
            std::cout << "msg pushed"<<std::endl;
        }
    } catch (...) {
        std::cout<<"client closed connection"<<std::endl;
        exit(0);
    }
};


auto BrokerThread = [&] (Hqqt::Broker<SqttClient> &broker)
{
    std::cout << "broker thread active"<<std::endl;
         auto msgCli  = msgQueue.Front();
        msgQueue.Pop();
        std::cout << "msg poped"<<std::endl;
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

    ThreadPool pool(PoolSize);
    Hqqt::Broker<SqttClient> broker;
     pool.enqueue(BrokerThread, std::ref(broker));

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






