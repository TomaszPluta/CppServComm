#include "curl/curl.h"
#include <sstream>
#include <vector>
#include <iostream>

class Header {

   static  constexpr int fromPos = 0;
   static  constexpr int datePos = 1;
   static  constexpr int subjectPos = 2;
       public:
    std::string _date;
    std::string _to;
    std::string _from;
    std::string _subject;
    


        Header(){};
    
    Header(std::string headTxt){
        std::stringstream ss (headTxt); 
        std::string tmp;
        std::vector<std::string>  tokens;
         while(getline(ss, tmp)){
             tokens.push_back(tmp);
         }
        _to = tokens[fromPos];
        _date = tokens[datePos];
        _subject = tokens[subjectPos];
    }
    
    
        Header(std::string toAddr, std::string fromAddr, std::string date, std::string subject): _to(toAddr), _from(fromAddr), _date(date), _subject(subject)  {
;
    }
    
    friend std::ostream & operator <<(std::ostream& os, const Header &header);
};


inline std::ostream & operator <<(std::ostream& os, const Header &header){ //move to cpp, not inline
    os<<header._date<<std::endl; 
    os<<header._to<<std::endl;
    os<<header._subject<<std::endl;
    return os;
}

class EmailMsg {

public:


    Header _header;//
    std::string _body;//
    
    EmailMsg(){};
    
    EmailMsg(std::string addrTo, std::string subject, std::string body){
        _header._to = addrTo;
        _header._subject = subject;
        _body = body;
    }
    
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





class MailBox
{
private:
    CURL *_curl;
    std::string _user;
    std::string _pwd;
    std::string _inUrl;
    std::string _outUrl;
    std::string readBuffer;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        ((std::string*)userp)->clear();
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }


    
    
    std::vector<int> GetUnreadIds();
    
    static size_t SendCallback(void *ptr, size_t size, size_t nmemb, void *userp);

public:

    std::string FetchMailHeader(int id)  {
        curl_easy_setopt(_curl, CURLOPT_URL, std::string(_inUrl+"/;UID="+std::to_string(id)+"/;SECTION=HEADER.FIELDS%20(DATE%20FROM%20SUBJECT)").c_str());
        CURLcode res = curl_easy_perform(_curl);
        if(res != CURLE_OK) {
            throw std::runtime_error("unable to fetch e-mail (given id: "+std::to_string(id)+")");
        }
        std::cout<<"email header: "<<readBuffer<<std::endl;
        return readBuffer;
    }
    
    std::string FetchMailText(int id)  {
        curl_easy_setopt(_curl, CURLOPT_URL, std::string(_inUrl+"/;UID="+std::to_string(id)+"/;SECTION=1").c_str());
        CURLcode res = curl_easy_perform(_curl);
        if(res != CURLE_OK) {
            throw std::runtime_error("unable to fetch e-mail (given id: "+std::to_string(id)+")");
        }
         std::cout<<"email body: "<<readBuffer<<std::endl;
        return readBuffer;
    }
    



    EmailMsg FetchMail(int id)  {
      
        
      EmailMsg email( FetchMailHeader(id), FetchMailText(id));
      
        return email;
    }
    
    
    MailBox( std::string user,  std::string pwd, std::string inboxUrl, std::string outUrl) : _user(user), _pwd(pwd), _inUrl(inboxUrl), _outUrl(outUrl) {
        _curl = curl_easy_init();
        curl_easy_setopt(_curl, CURLOPT_USERNAME, _user.c_str());
        curl_easy_setopt(_curl, CURLOPT_PASSWORD, _pwd.c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
    }

    ~MailBox() {
       // curl_easy_cleanup(_curl);
    }



   
    
    std::vector<EmailMsg> GetUnreadEmails(){
           _curl = curl_easy_init();
        curl_easy_setopt(_curl, CURLOPT_USERNAME, _user.c_str());
        curl_easy_setopt(_curl, CURLOPT_PASSWORD, _pwd.c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
        
        
        std::vector<EmailMsg> unreads;
        for (auto i : GetUnreadIds()){
            unreads.push_back(FetchMail(i));
        }
        
           curl_easy_cleanup(_curl);
        return unreads;
    }
    
    
    
    
    
    
    void Send(EmailMsg email);
    void Send(std::string to, std::string subject, std::string content);
};
    
    
    
    
    
    