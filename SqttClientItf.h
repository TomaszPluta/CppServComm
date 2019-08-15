
#pragma once


#include <string>

#include "serv.h"
#include "SocketException.h"

namespace Hqqt{

using Msg = std::string;

class Client{
	std::string _addr;
public:
	virtual void Send (Msg msg) = 0;
	virtual void Update(Msg msg) =0;
    
    const std::string GetAddr(void){
        return _addr;
    }
   Client & operator<<(Msg msg){
        Send(msg);
    }
};


}
