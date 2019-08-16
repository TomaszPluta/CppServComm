
#pragma once


#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>

#include "SqttClientItf.h"
#include "serv.h"
#include "SocketException.h"

namespace Hqqt{
using Msg = std::string;


class ClientSocket : public Client{
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


class ClientDebug : public Client{
public:
    void Send (Msg msg) {
        std::cout<<msg;
    }
	void Update(Msg msg){
        Send(msg);
    }
};

}