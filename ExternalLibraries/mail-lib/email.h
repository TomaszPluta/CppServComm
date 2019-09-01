#pragma once

#include "curl/curl.h"
#include "iostream"
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <clocale>

#include "/home/tomek/credraspbmail.crd"

#define LINUX_H

//
//std::locale::global(std::locale(""));   

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


class Header {

   static  constexpr int fromPos = 0;
   static  constexpr int datePos = 1;
   static  constexpr int subjectPos = 2;
    public:
    std::string recDate;
    std::string from;
    std::string subject;
    
    Header(std::string head){
        std::stringstream ss (head); 
        std::string tmp;
        std::vector<std::string>  tokens;
         while(getline(ss, tmp)){
             tokens.push_back(tmp);
         }
        from = tokens[fromPos];
        recDate = tokens[datePos];
        subject = tokens[subjectPos];
    }
    friend std::ostream & operator <<(std::ostream& os, const Header &header);
};


inline std::ostream & operator <<(std::ostream& os, const Header &header){ //move to cpp, not inline
    os<<header.recDate<<std::endl; 
    os<<header.from<<std::endl;
    os<<header.subject<<std::endl;
    return os;
}


class EmailMsg {
    Header _header;
    std::string _body;
public:
    EmailMsg(std::string header, std::string body) : _header(header), _body(body){};
    friend std::ostream & operator<<(std::ostream &os, const EmailMsg &email) ;
};



inline std::ostream& operator<<(std::ostream& os, const EmailMsg& email) {
     os<< email._header;
    os<< email._body;
    return os;
}


inline std::ostream& operator<<(std::ostream& os, std::vector<EmailMsg> ev) {
    for (const auto & i : ev){
        os<< i;
    }
    return os;
}


 
class Inbox
{
private:
    CURL *_curl;
    std::string _user;
    std::string _pwd;
    std::string _url;
    std::string readBuffer;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        ((std::string*)userp)->clear();
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }



    
     std::vector<int> GetUnreadIds() {
        curl_easy_setopt(_curl, CURLOPT_URL, "imaps://imap.gmail.com:993/INBOX?UNSEEN");
        CURLcode res = curl_easy_perform(_curl);
        if(res != CURLE_OK) {
            throw std::runtime_error("unable to retrieve the number of unread emails");
        }
        
       if (size_t pos = readBuffer.find("* SEARCH") != std::string::npos){
           readBuffer.erase(0, std::string("* SEARCH").length());
       }
      
       std::stringstream ss(readBuffer);
       std::string id;
       std::vector<int> ids;
       while (ss >> id){
           ids.push_back(std::stoi(id));
       }

       return ids;
    }

public:

    std::string FetchMailHeader(int id)  {
        curl_easy_setopt(_curl, CURLOPT_URL, std::string(_url+"/;UID="+std::to_string(id)+"/;SECTION=HEADER.FIELDS%20(DATE%20FROM%20SUBJECT)").c_str());
        CURLcode res = curl_easy_perform(_curl);
        if(res != CURLE_OK) {
            throw std::runtime_error("unable to fetch e-mail (given id: "+std::to_string(id)+")");
        }
        return readBuffer;
    }
    
    std::string FetchMailText(int id)  {
        curl_easy_setopt(_curl, CURLOPT_URL, std::string(_url+"/;UID="+std::to_string(id)+"/;SECTION=1").c_str());
        CURLcode res = curl_easy_perform(_curl);
        if(res != CURLE_OK) {
            throw std::runtime_error("unable to fetch e-mail (given id: "+std::to_string(id)+")");
        }
        return readBuffer;
    }
    



    EmailMsg FetchMail(int id)  {
      EmailMsg email( FetchMailHeader(id), FetchMailText(id));
        return email;
    }
    
    
    Inbox( std::string user,  std::string pwd, std::string inboxUrl) : _user(user), _pwd(pwd), _url(inboxUrl) {
        _curl = curl_easy_init();
        curl_easy_setopt(_curl, CURLOPT_USERNAME, _user.c_str());
        curl_easy_setopt(_curl, CURLOPT_PASSWORD, _pwd.c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
    }

    ~Inbox() {
        curl_easy_cleanup(_curl);
    }



   
    
    std::vector<EmailMsg> GetUnreadEmails(){
        std::vector<EmailMsg> unreads;
        for (auto i : GetUnreadIds()){
            unreads.push_back(FetchMail(i));
        }
        return unreads;
    }
    
    
};
