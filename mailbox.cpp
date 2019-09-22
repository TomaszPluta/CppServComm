#include "mailbox.h"
#include <iostream>
#include <string.h>

#include "/home/tomek/credraspbmail.crd"

//




  std::vector<int> MailBox::GetUnreadIds()
    {
        curl_easy_setopt(_curl, CURLOPT_URL, "imaps://imap.gmail.com:993/INBOX?UNSEEN");
        CURLcode res = curl_easy_perform(_curl);
        if(res != CURLE_OK) {
            throw std::runtime_error("unable to retrieve the number of unread emails");
        }
    
        if (size_t pos = readBuffer.find("* SEARCH") != std::string::npos) {
            readBuffer.erase(0, std::string("* SEARCH").length());
        }
        
        std::cout<< "mail unread: "<<readBuffer<<std::endl;
        std::stringstream ss(readBuffer);
        std::string id;
        std::vector<int> ids;
        while (ss >> id) {
            ids.push_back(std::stoi(id));
        }
    
        return ids;
    }

size_t  MailBox::SendCallback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    std::string * msg = static_cast<std::string*> (userp);
    if (msg->size()> 0) {
        size_t len = msg->size();
        memcpy(ptr, msg->c_str(), len);
        msg->clear();
        return len;
    } else {
        delete(msg);
        return 0;
    }
}

void MailBox::Send(std::string to, std::string subject, std::string content)
{
    
     _curl = curl_easy_init();
        curl_easy_setopt(_curl, CURLOPT_USERNAME, _user.c_str());
        curl_easy_setopt(_curl, CURLOPT_PASSWORD, _pwd.c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
        
        
        
    
    
    
    
    
    
    
std::string mail =
"To: " + to + "\r\n"
"From: " + _user + "\r\n"
"Subject: "+ subject +"\r\n"
"\r\n" + content;




      std::string * msg = new std::string(mail);
      std::cout<<"sent: "<<msg->c_str()<<std::endl;
      struct curl_slist *recipients = NULL;

    curl_easy_setopt(_curl, CURLOPT_URL, _outUrl.c_str());
    curl_easy_setopt(_curl, CURLOPT_MAIL_FROM, _user.c_str());

     recipients = curl_slist_append(recipients, to.c_str());
    curl_easy_setopt(_curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(_curl, CURLOPT_READFUNCTION, SendCallback);
    curl_easy_setopt(_curl, CURLOPT_READDATA, msg);
    curl_easy_setopt(_curl, CURLOPT_UPLOAD, 1L);

    CURLcode res = curl_easy_perform(_curl);
    curl_slist_free_all(recipients);
    curl_easy_cleanup(_curl);
    if (res != CURLE_OK) {
        throw std::runtime_error("unable to send email");
    }
}
