#pragma once

#include "curl/curl.h"
#include "iostream"
#include <vector>
#include <string>

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

 /*
***
  */


size_t write_data(char* buf, size_t size, size_t nmemb, void* up) {
    std::string fetchdata;
    fetchdata.append((char*)buf, size*nmemb);
    std::cout<<fetchdata<<std::endl;
    return size * nmemb;
}


class EmailReceiver{
private:
        CURL *_curl;
        std::string _user;
        std::string _pwd;
        std::string _url;
public:
EmailReceiver( std::string user,  std::string pwd, std::string inboxUrl) : _user(user), _pwd(pwd), _url(inboxUrl) {
     _curl = curl_easy_init();
}

int FetchMail() {
        curl_easy_setopt(_curl, CURLOPT_USERNAME, _user);
        curl_easy_setopt(_curl, CURLOPT_PASSWORD, _pwd);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);

        curl_easy_setopt(_curl, CURLOPT_URL, _url);
        curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "UID FETCH 1 BODY[HEADER.FIELDS (To)]");

        CURLcode res = curl_easy_perform(_curl);

        if(res != CURLE_OK){
            std::cout<< "curl_easy_perform() failed: %s\n"<<std::endl;
        }
        curl_easy_cleanup(_curl);

    return (int)res;
}
};

