#include "serv.h"
#include "SocketException.h"
#include "SqqtLib.h"
#include "ThreadPool.h"
#include "ExternalLibraries/MySQL-CRUD-CPP/Database.h"

#include <string>
#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <ctime> 




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
    try {
        while (1) {
              std::cout<<"- - - - - - - - "<<std::endl;
            std::string frame;
            new_sock >> frame;

            std::cout << "Server got:" << frame <<std::endl;
            std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;
            std::string addr;
            std::mutex m;
            std::unique_lock<std::mutex> mLock(m);
            new_sock << broker.OnReceivedFrame(frame, new_sock);
        }
    } catch (...) {
        std::cout<<"client closed connection"<<std::endl;
        exit(0);
    }
}

constexpr int ServPort = 1886;
constexpr int PoolSize = 4;
constexpr int ColumnNo = 3;

int main ( int argc, char * argv[] )
{
    
DB::Database *database = new DB::Database("localhost", "3306", "sqqt", "1234", "sqqtDB");
std::vector<std::vector<std::string>> users = database->Get("SELECT * FROM users", ColumnNo);
   
for (int i =0; i < users.size(); i++){
    for (int j =0; j < users[i].size(); j++){
        std::cout<<users[i][j] << " | ";
    } std::cout<<std::endl;
}
    
    Hqqt::Broker<ServerSocket> broker;
    ThreadPool pool(PoolSize);

    std::cout << "Server is running...."<<std::endl;
    ServerSocket server ( ServPort );
    
    while(1) {
      ServerSocket  * cliSocket = new(ServerSocket);
      server.accept (*cliSocket );
      
  
     std::chrono::system_clock::time_point timeNow  = std::chrono::system_clock::now();
     std::time_t timeConv =  std::chrono::system_clock::to_time_t(timeNow);
     std::cout << ctime(&timeConv) << std::endl; 
    
    
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    
        database->Insert("INSERT INTO users(id,status,timeLogin) VALUES (?,?,?)", { "S:"+std::string(cliSocket->get_cli_addr()),"I:100", "S:" + std::string(ctime(&timeConv))});
        pool.enqueue(t1, std::ref(*cliSocket), std::ref(broker));
    }



    std::cout << "Server end"<<std::endl;
    return 0;
}






