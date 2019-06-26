
#pragma once


#include <vector>
#include <string>
#include <algorithm>
#include <functional>

namespace Hqqt{

using Addr = std::string;
using Topicid = std::string;
using Msg = std::string;






template <class T>
class Client{
	T & _obj;

public:
	Client(T &obj): _obj(obj){};
	std::string addr; ///prv

	void Send (Msg msg){
		_obj << msg;
	}

	void Update(Msg msg){
		_obj << msg;
	}


};

//
//class Authoriser {
//public:
//	std::vector<Client*> clients;
//	void AddClient(Client* cli){
//		clients.push_back(cli);
//	}
//	bool IsAuthorised(Client * cli){
//		return true;
//	}
//
//};



template <class T>
class Topic{

	std::vector <Client<T> *> clients;

public:
	Topicid _id;

	Topic(std::string id){
		_id = id;
	}

	void Attach (Client<T>* cli){
		clients.push_back(cli);
	}

	void Detach (Client<T>* cli){
		clients.push_back(cli);
	}

	void Notify(std::string msg){
		for (auto i : clients){
			i->Update(msg);
		}
	}

};



//template<class T>
//class Sender{
//	T & obj;
//public:
//	void Send(std::string msg){
//		obj<<msg;
//	}
//};
//

class Broker{
	std::vector <Topic*> topics;
	//set instead to avoid duplications?


	void AddTopic(std::string topicId){
		Topic * topic = new Topic(topicId);
		topics.push_back(topic);
	}

	Topic * GetTopicById(Topicid id){
		auto it = std::find_if(topics.begin(), topics.end(), [&](Topic * topic){return (topic->_id  == id);});
		if (it != topics.end()){
			return *it;
		} else {
			return nullptr;
		}
	}



public:

	std::string OnReceivedFrame(const std::string& frame, std::string addr){
		if(frame.find("subscribe") !=  std::string::npos){
			std::size_t pos = frame.find(":");
			if (pos != std::string::npos){
				std::string topic = frame.substr(pos+1);
				AddSubscription(topic, addr);
				return "subscribe ok";
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
				Publish(topicId, msg);
				return "publish ok";
			}
		}
		return "no matching frame found";
	}


	void AddSubscription(std::string topicId, std::string cliAddr){
		Topic * topic =  GetTopicById(topicId);
		Client * cli = new Client; // =  authorizer.GetClient(address);
		cli->addr = cliAddr;
		topic->Attach(cli);
	}




	void Publish (std::string topicID, std::string msg){
		Topic * topic = GetTopicById(topicID);
			topic->Notify(msg);
	}


};

}
