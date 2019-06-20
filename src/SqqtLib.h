
#pragma once


#include <vector>
#include <string>
#include <algorithm>

namespace Hqqt{

using Addr = std::string;
using Topicid = std::string;
using Msg = std::string;





class Client{
	std::vector<std::string> subscriptions;


public:
	std::string addr; ///prv

	void Update(Msg msg){
		;
	}

};


class Authoriser {
public:
	std::vector<Client*> clients;
	void AddClient(Client* cli){
		clients.push_back(cli);
	}
	bool IsAuthorised(Client * cli){
		return true;
	}

};




class Topic{
	std::vector <Client *> clients;
public:
	Topicid _id;
	Msg _msg;

	Topic(std::string id){
		_id = id;
	}

	void Attach (Client* cli){
		clients.push_back(cli);
	}

	void Detach (Client* cli){
		clients.push_back(cli);
	}

	void Notify(){
		for (auto i : clients){
			i->Update(_msg);
		}
	}

};








class Broker{
	std::vector <Topic*> topics;

public:
	void AddTopic(std::string topicid){
		Topic * topic = new Topic(topicid);
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
};




	//	if (PUB){
	//		Topic * topic = GetTopicByid(topicid){
	//			topic->Notify(msg);
//		}
//	}



}
