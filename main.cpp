#include "serv.h"
#include "SocketException.h"
#include "SqqtLib.h"
#include <string>
#include <iostream>

#include <functional>
#include <thread>

//std::string GetFrame(){
//;
//}


void SocketSend(std::string addr, std::string msg){

}

//
// auto ThreadLambda = [&](){
//                        try{
//                                while (1){
//                                        std::cout << "child process here:" << std::endl;
//                                        std::string frame;
//                                        new_sock >> frame;
//
//                                        std::cout << "Server got:" << frame <<std::endl;
//                                        std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;
//                                        std::string addr;
//                              //	  frame = GetFrame();
//                                        new_sock << broker.OnReceivedFrame(frame, new_sock);
//                                }
//                                }
//                        catch (...){
//                                std::cout<<"client closed connection"<<std::endl;
//                                exit(0);
//                        }
//              };
              
              
              

void t1 ( ServerSocket new_sock ){

    Hqqt::Broker<ServerSocket> broker;
     try{
                    while (1){
                            std::cout << "child process here:" << std::endl;
                            std::string frame;
                            new_sock >> frame;

                            std::cout << "Server got:" << frame <<std::endl;
                            std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;
                            std::string addr;
                  //	  frame = GetFrame();
                            new_sock << broker.OnReceivedFrame(frame, new_sock);
                    }
                    }
            catch (...){
                    std::cout<<"client closed connection"<<std::endl;
                    exit(0);
            }
}

//thread pool z boosta: https://stackoverflow.com/questions/19500404/how-to-create-a-thread-pool-using-boost-in-c

int main ( int argc, char * argv[] )
{
Hqqt::Broker<ServerSocket> broker;
std::thread thObj;
std::thread thObj2;
std::thread thObj3;

std::cout << "Server running....\n";
ServerSocket server ( 1886 );

    while(1){
ServerSocket new_sock;
server.accept ( new_sock );
 thObj = std::thread(t1,  std::ref(new_sock));

 ServerSocket new_sock2;
 server.accept ( new_sock2 );
 thObj2 = std::thread(t1, std::ref(new_sock2));
 
 ServerSocket new_sock3;
 server.accept ( new_sock3 );
 thObj3 = std::thread(t1, std::ref(new_sock3));
    }
            

          

  thObj.join();
  thObj2.join();
  std::cout << "Server end\n";
  return 0;
}
















//
//
//int main ( int argc, char * argv[] )
//{
//
//
//	Hqqt::Broker broker;
//  std::cout << "Server running....\n";
//
//  try
//    {
//      ServerSocket server ( 1886 );
//
//      while ( true )
//	{
//
//	  ServerSocket new_sock;
//	  server.accept ( new_sock );
//
//		auto lambdaSend = [&](std::string msg){
//			new_sock << msg;
//		};
//
//	  try
//	    {
//	      while ( true )
//		{
//
//	      std::string frame;
//		  new_sock >> frame;
//
//
//
//		  std::cout << "Server got:" << frame <<std::endl;
//
//	    	  std::cout << new_sock.get_cli_addr() <<std::endl;
//
//	    	  if (cli not connected){
//	    		  Client cli = new Client;
//	    		  cli.SetSendCallback(SocketSend);
//	    		  ADD cLIENT
//
//	    	  }else getClint
//
//
//
//
//
//		  if(frame.find("subscribe") !=  std::string::npos){
//			  std::size_t pos = frame.find(":");
//			  if (pos != std::string::npos){
//				  std::string topic = frame.substr(pos+1);
//				  broker.AddSubscription(topic, new_sock.get_cli_addr());
//				  new_sock << "subscribe ok";
//			  }
//		  }
//		  if(frame.find("publish") !=  std::string::npos){
//			  std::size_t pos = frame.find(":");
//			  std::string topicId;
//			  if (pos != std::string::npos){
//				  topicId = frame.substr(pos+1);
//			  }
//			  pos = frame.find(":");
//			  std::string msg;
//			  if (pos != std::string::npos){
//				  msg = frame.substr(pos+1);
//				  broker.Publish(topicId, msg);
//				  new_sock << "publish ok";
//			  }
//
//		  }
//
//		}
//	    }
//	  catch ( SocketException& ) {}
//
//	}
//    }
//  catch ( SocketException& e )
//    {
//      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
//    }
//
//  std::cout << "Server end\n";
//  return 0;
//}
