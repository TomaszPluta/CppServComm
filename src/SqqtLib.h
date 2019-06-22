
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

	Topic(std::string id){
		_id = id;
	}

	void Attach (Client* cli){
		clients.push_back(cli);
	}

	void Detach (Client* cli){
		clients.push_back(cli);
	}

	void Notify(std::string msg){
		for (auto i : clients){
			i->Update(msg);
		}
	}

};








class Broker{
	std::vector <Topic*> topics;
	//set instead to avoid duplications?

public:
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

	void Publish (std::string topicID, std::string msg){
		Topic * topic = GetTopicById(topicID);
			topic->Notify(msg);
	}


};



