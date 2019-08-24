#pragma once

#include "curl/curl.h"
#include "iostream"
#include <vector>
#include <string>

#include "/home/tomek/credraspbmail.crd"

#define LINUX_H

class EmailSender
{
public:
	EmailSender();

	void setTo(const std::string to);
	void setFrom(const std::string from);
	void setCc(const std::string to);
	void setSubject(const std::string subject);
	void setBody(const std::string body);
	void setSMTP_username(const std::string username);
	void setSMTP_password(const std::string password);
	void setSMTP_host(const std::string host);
	void addAttachment(const std::string filepath);
	void removeAllAttachments();
	void constructEmail();
	void clearEmailContents();
	int send() const;
	void dump() const;

private:
	std::string smtp_user;
	std::string smtp_password;
	std::string smtp_host;

	std::string to;
	std::string from;
	std::string cc;
	std::string subject;
	std::string body;

	std::vector<std::string> email_contents;
	std::vector<std::string> attachments;

	int numberOfLines;
};

class Inbox{
private:
        CURL *_curl;
        std::string _user;
        std::string _pwd;
        std::string _url;
public:
    
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

  std::string readBuffer;

    
    
    Inbox( std::string user,  std::string pwd, std::string inboxUrl) : _user(user), _pwd(pwd), _url(inboxUrl){
        _curl = curl_easy_init();
        curl_easy_setopt(_curl, CURLOPT_USERNAME, _user.c_str());
        curl_easy_setopt(_curl, CURLOPT_PASSWORD, _pwd.c_str());
//        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
        

    }
    
    ~Inbox()    {
     curl_easy_cleanup(_curl);
}



int GetMailNumber() {
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(_curl, CURLOPT_URL, "imaps://imap.gmail.com:993/INBOX?NEW"); 
        CURLcode res = curl_easy_perform(_curl);
       std::cout << readBuffer << std::endl;
        if(res != CURLE_OK){
            std::cout<< "curl_easy_perform() failed: %s\n"<< curl_easy_strerror(res)<<std::endl;
        }
    return (int)res;
};



 


int FetchMail(int id) {
       curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
       
       curl_easy_setopt(_curl, CURLOPT_URL, std::string("imaps://imap.gmail.com:993/INBOX/;UID="+std::to_string(id)+"/;SECTION=TEXT").c_str()); //_url + opta /optb + mail nb as arg

        CURLcode res = curl_easy_perform(_curl);
        std::cout << readBuffer << std::endl;
        if(res != CURLE_OK){
            std::cout<< "curl_easy_perform() failed: %s\n"<< curl_easy_strerror(res)<<std::endl;
        }

    return (int)res;
}
};

