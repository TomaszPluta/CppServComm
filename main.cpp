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

using std::basic_ostream;
using std::cin;
using std::cout;
using std::endl;
using std::get;
using std::ostream;
using std::string;
using std::tuple;
using std::unique_ptr;
using std::vector;



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
    
MYSQL mysql;
mysql_init(&mysql);
if (mysql_real_connect(&mysql, "localhost","sqqt", "1234","sqqtDB", 0, NULL, 0)){
    mysql_select_db(&mysql, "sqqtDB");
    mysql_query(&mysql, "SELECT * FROM users");
    MYSQL_RES * mRes = mysql_store_result(&mysql);
    MYSQL_ROW  mRow;
    while ((mRow = mysql_fetch_row(mRes)) != NULL)
   {
        for (int i =0; i < mysql_num_fields(mRes); i++){
            std::string data = (mRow[i]  == NULL ? "0" : mRow[i] );
            std::cout << data + " ";
        }
            std::cout << std::endl;
    }
    
    
} else{
       std::cout<<" MySQL connecting error" << mysql_error(&mysql);
}

    Hqqt::Broker<ServerSocket> broker;
    ThreadPool pool(PoolSize);
    ServerSocket server ( ServPort );
    std::cout << "Server is running...."<<std::endl;

    
    while(1) {
      ServerSocket  * cliSocket = new(ServerSocket);
      server.accept (*cliSocket );
          
//        database->Insert("INSERT INTO users(id,status,timeLogin) VALUES (?,?,?)", { "S:"+std::string(cliSocket->get_cli_addr()),"I:100", "S:" + GetTimeNow()});
        pool.enqueue(WorkerThread, std::ref(*cliSocket), std::ref(broker));
    }



    std::cout << "Server end"<<std::endl;
    return 0;
}






