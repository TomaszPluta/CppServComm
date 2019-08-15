
#pragma once


#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>
#include <chrono>

#include "SqttClientItf.h"

namespace Hqqt{
    
using namespace std::chrono_literals;

using Addr = std::string;
using Topicid = std::string;
using Msg = std::string;







class Topic{

	std::vector <Client*> clients;

public:
	Topicid _id;

	Topic(std::string id){
		_id = id;
	}

	void Attach (Client* cli){
		clients.push_back(cli);
	}

	void Detach (Client* cli){
		clients.erase(std::remove_if(clients.begin(), clients.end(), [&](Client *c){return(c->GetAddr()==cli->GetAddr());}), clients.end());
	}

	void Notify(std::string msg){
		for (auto i : clients){
			i->Update(msg);
            sleep(1);
		}
	}

};


template <class T>
class Broker{

	//set instead to avoid duplications?


	void AddTopic(std::string topicId){
		Topic  * topic= new Topic(topicId);
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
	std::vector <Topic*> topics;

	std::string OnReceivedFrame(const std::string frame, T* senderObj){

		std::stringstream frameStream(frame);
		std::string token;
		std::vector<std::string> tokenList;

		while(std::getline(frameStream, token, ':'))
		{
			tokenList.push_back(token);
		}


		constexpr   int Head = 0;
		constexpr   int Topic = 1;
		constexpr   int Msg = 2;

		if(tokenList[Head] == "subscribe"){ //map
			AddSubscription(tokenList[Topic], senderObj);
			return "subscribe ok";
		}

		if(tokenList[Head] == "publish"){ //map
			Publish(tokenList[Topic], tokenList[Msg]);
			return "publish ok";
		}

		return "no matching frame found";
	}


	void AddSubscription(std::string topicId, Client *cli){
		Topic * topic =  GetTopicById(topicId); //make more verbose: "get topic, if not, create one"
		if (topic == nullptr){
			topic = new Topic(topicId);
			topics.push_back(topic);
		}
		topic->Attach(cli);
	}




	void Publish (std::string topicId, std::string msg){
		Topic * topic = GetTopicById(topicId);
		if (topic == nullptr){
					topic = new Topic(topicId);
				}
			topic->Notify(msg);
	}


};

}
