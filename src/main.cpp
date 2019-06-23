#include "serv.h"
#include "SocketException.h"
#include <string>
#include <iostream>
#include "SqqtLib.h"



int main ( int argc, char * argv[] )
{



	Hqqt::Broker broker;
  std::cout << "Server running....\n";

  try
    {
      ServerSocket server ( 1886 );

      while ( true )
	{

	  ServerSocket new_sock;
	  server.accept ( new_sock );

	  try
	    {
	      while ( true )
		{

	    	  std::cout << new_sock.get_cli_addr() <<std::endl;
		  std::string frame;
		  new_sock >> frame;
		  std::cout << "Server:" << frame <<std::endl;


		  if(frame.find("subscribe") !=  std::string::npos){
			  std::size_t pos = frame.find(":");
			  if (pos != std::string::npos){
				  std::string topic = frame.substr(pos+1);
				  //broker.AddSubscription(topic, );
				  new_sock << "subscribe ok";
			  }
		  }
		  if(frame.find("publish") !=  std::string::npos){
			  std::size_t pos = frame.find(":");
			  std::string topicId;
			  if (pos != std::string::npos){
				  topicId = frame.substr(pos+1);
			  }
			  pos = frame.find(":");
			  std::string msg;
			  if (pos != std::string::npos){
				  msg = frame.substr(pos+1);
				  broker.Publish(topicId, msg);
			  }

		  }

		}
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  std::cout << "Server end\n";
  return 0;
}
