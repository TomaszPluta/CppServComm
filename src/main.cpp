#include "serv.h"
#include "SocketException.h"
#include <string>
#include <iostream>
#include "SqqtLib.h"



int main ( int argc, char * argv[] )
{



	Hqqt::Broker broker;
  std::cout << "running....\n";

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
		  std::string frame;
		  new_sock >> frame;
		  if(frame.find("subscribe") !=  std::string::npos){
			  std::size_t pos = frame.find(":");
			  if (pos != std::string::npos){
				  std::string topic = frame.substr(pos+1);
				  broker.AddTopic(topic);
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

  return 0;
}
