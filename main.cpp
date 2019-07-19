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


void PrintDatabase (DB::Database *database)
{
    std::vector<std::vector<std::string>> users = database->Get("SELECT * FROM users", ColumnNo);
    for (int i =0; i < users.size(); i++) {
        for (int j =0; j < users[i].size(); j++) {
            std::cout<<users[i][j] << " | ";
        }
        std::cout<<std::endl;
    }
}

std::string GetTimeNow (){
     std::chrono::system_clock::time_point timeNow  = std::chrono::system_clock::now();
     std::time_t timeConv =  std::chrono::system_clock::to_time_t(timeNow);
     std::string tNow=   std::string(ctime(&timeConv));
     return tNow;
}




int main ( int argc, char * argv[] )
{
    
DB::Database *database = new DB::Database("localhost", "3306", "sqqt", "1234", "sqqtDB");
PrintDatabase(database);
    
    Hqqt::Broker<ServerSocket> broker;
    ThreadPool pool(PoolSize);
    ServerSocket server ( ServPort );
    std::cout << "Server is running...."<<std::endl;

    
    while(1) {
      ServerSocket  * cliSocket = new(ServerSocket);
      server.accept (*cliSocket );
        

    
        database->Insert("INSERT INTO users(id,status,timeLogin) VALUES (?,?,?)", { "S:"+std::string(cliSocket->get_cli_addr()),"I:100", "S:" + GetTimeNow()});
        pool.enqueue(WorkerThread, std::ref(*cliSocket), std::ref(broker));
    }



    std::cout << "Server end"<<std::endl;
    return 0;
}






