
#pragma once


#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>
#include <chrono>


namespace Hqqt{
    
using namespace std::chrono_literals;

using Addr = std::string;
using Topicid = std::string;
using Msg = std::string;






template <class T>
class Client{
	T & _obj;

public:
	Client(T *obj): _obj(obj){};
	std::string addr; ///prv

	void Send (Msg msg){
		_obj << msg;
	}

	void Update(Msg msg){
		_obj << msg;
	}

};



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
            sleep(1);
		}
	}

};


template <class T>
class Broker{

	//set instead to avoid duplications?


	void AddTopic(std::string topicId){
		Topic<T>  * topic= new Topic<T>(topicId);
		topics.push_back(topic);
	}

	Topic<T> * GetTopicById(Topicid id){
		auto it = std::find_if(topics.begin(), topics.end(), [&](Topic<T> * topic){return (topic->_id  == id);});
		if (it != topics.end()){
			return *it;
		} else {
			return nullptr;
		}
	}



public:
	std::vector <Topic<T>*> topics;

	std::string OnReceivedFrame(const std::string frame, T* senderObj){

		std::stringstream frameStream(frame);
		std::string token;
		std::vector<std::string> tokenList;

		while(std::getline(frameStream, token, ':'))
		{
			tokenList.push_back(token);
		}


		constexpr   int Head = 0; //convert vector to map
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


	void AddSubscription(std::string topicId, T *obj){
//		Topic<T> * topic =  GetTopicById(topicId); //opaque with lines below
//		if (topic == nullptr){
//			topic = new Topic<T>(topicId);
//			topics.push_back(topic);
//		}
//		Client<T> * cli = new Client<T>(obj);
//		topic->Attach(cli);

	}




	void Publish (std::string topicId, std::string msg){
		Topic<T> * topic = GetTopicById(topicId);
		if (topic == nullptr){
					topic = new Topic<T>(topicId);
				}
			topic->Notify(msg);
	}


};

}
