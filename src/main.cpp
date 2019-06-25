#include "serv.h"
#include "SocketException.h"
#include "SqqtLib.h"
#include <string>
#include <iostream>
#include <string>



std::string GetFrame(){
;
}



int main ( int argc, char * argv[] )
{


	Hqqt::Broker broker;
  std::cout << "Server running....\n";
      ServerSocket server ( 1886 );

while(1){
	  ServerSocket new_sock;
	  server.accept ( new_sock );
	  if (fork() == 0){
		  while (1){
			  std::cout << "child here:" << std::endl;
			  std::string frame;
			  new_sock >> frame;

			  std::cout << "Server got:" << frame <<std::endl;
			  std::cout << "Peer addr" << new_sock.get_cli_addr() <<std::endl;


			  std::string addr;
			  frame = GetFrame();
			  new_sock << broker.OnReceivedFrame(frame, addr);
		  }
	  }else {

	  }

}
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
