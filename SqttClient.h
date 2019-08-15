
#pragma once


#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>


#include "serv.h"
#include "SocketException.h"

using Msg = std::string;

class SqttClient{
	std::string addr;
public:
	virtual void Send (Msg msg) = 0;
	virtual void Update(Msg msg) =0;
    
   SqttClient & operator<<(Msg msg){
        Send(msg);
    }
};




class ClientSocket : public SqttClient{
        ServerSocket & _socket;
public:

 ClientSocket(ServerSocket &socket) : _socket {socket}{};
 
    void Send (Msg msg) {
        _socket<<msg;
    }
	void Update(Msg msg){
        Send(msg);
    }
    
    
};


class ClientDebug : public SqttClient{
public:
    void Send (Msg msg) {
        std::cout<<msg;
    }
	void Update(Msg msg){
        Send(msg);
    }
};

