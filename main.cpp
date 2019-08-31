#include "serv.h"
#include "SocketException.h"
#include "SqqtLib.h"
#include "ThreadPool.h"
#include <string>
#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <ctime> 
#include <cassert>
#include <utility>
#include <mysql.h>
#include <stdexcept>

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "queue"

#include "SqttClientItf.h"
#include "SqttClientsImpl.h"
#include "SqlWrapper.h"
#include <fstream>

#include "SharedQueue.h"

#include "email.h"
#include "/home/tomek/credraspbmail.crd"

constexpr int ServPort = 1886;
constexpr int PoolSize = 4;
constexpr int ColumnNo = 3;

SharedQueue <std::pair <std::string, Hqqt::Client *>> msgQueue;



auto WorkerThread = [&] ( ServerSocket new_sock )
{
    try {
            Hqqt::Client * socketCli = new Hqqt::ClientSocket(new_sock);
        while (1) {
            std::cout << "child process here:" << std::endl;
            std::string frame;
            new_sock >> frame;
            std::cout << "Server got:" << frame <<std::endl;
            std::cout << "from peer addr: " << new_sock.get_cli_addr() <<std::endl;
            msgQueue.Push(std::make_pair(frame, socketCli));
        }
    } catch (...) {
        std::cout<<"client closed connection"<<std::endl;
        exit(0);
    }
};




auto BrokerThread = [&] (Hqqt::Broker &broker)
{
    while (1){
    std::cout << "broker thread active"<<std::endl;
         auto msgCli  = msgQueue.Front();
        msgQueue.Pop();
        std::cout << "broker got msg"<<std::endl;
        broker.OnReceivedFrame(msgCli.first, msgCli.second);
    }
};




std::string GetTimeNow (){
     std::chrono::system_clock::time_point timeNow  = std::chrono::system_clock::now();
     std::time_t timeConv =  std::chrono::system_clock::to_time_t(timeNow);
     std::string tNow=   std::string(ctime(&timeConv));
     return tNow;
}

std::string fetchdata;




int fetchmail() {
    CURL *curl;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, mail_login);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, mail_password);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
       // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Inbox::write_data);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

   //     curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/INBOX/;UID=5/;SECTION=TEXT");
        curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/INBOX?NEW"); //_url + opta /optb
        //curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/INBOX");


        res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n",  curl_easy_strerror(res)); curl_easy_cleanup(curl);
        }
     std::ofstream outfile("fetched.txt", std::ios_base::app);
    outfile << fetchdata;
    outfile.close();

    }


    return (int)res;
}
 





//
//
//int fetchmail() {
//    CURL *curl;
//    CURLcode res = CURLE_OK;
//
//    curl = curl_easy_init();
//    if(curl) {
//        curl_easy_setopt(curl, CURLOPT_USERNAME, mail_login);
//        curl_easy_setopt(curl, CURLOPT_PASSWORD, mail_password);
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, EmailReceiver::write_data);
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//
//        curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/INBOX");
//        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "UID FETCH 1 BODY[HEADER.FIELDS (To)]");
//
//        res = curl_easy_perform(curl);
//
//        if(res != CURLE_OK)
//            fprintf(stderr, "curl_easy_perform() failed: %s\n",
//
//        curl_easy_strerror(res));        
//        curl_easy_cleanup(curl);
//    }
//
//    std::ofstream outfile("fetched.txt", std::ios_base::app);
//    outfile << fetchdata;
//    outfile.close();
//
//    return (int)res;
//}

void Mailtest(void){
//    
//Email e;
//	int curlError = 0;
//	// e.dump();
//
//	e.setTo(mail_destination);
//	e.setFrom("mail.raspberry.smtp@gmail.com");
//	e.setSubject("hello world");
//	e.setCc("");
//	e.setBody("My test email from my app");
//
//	e.setSMTP_host("smtps://smtp.gmail.com:465");
//	e.setSMTP_username(mail_login);
//	e.setSMTP_password(mail_password);
//
//	e.constructEmail();
//	e.dump();
//
//	curlError = e.send();
//
//	if (curlError){
//		std::cout << "Error sending email!" << std::endl;
//	}
//
//	else{
//		std::cout << "Email sent successfully!" << std::endl;
//	}
}



int main ( int argc, char * argv[] )
{
 try{
        Inbox inbox(mail_login, mail_password, "imaps://imap.gmail.com:993/INBOX");
        std::cout<<inbox.GetUnreadEmails();
       EmailMsg email = inbox.FetchMail(5);
        std::cout <<email;
  } catch (std::runtime_error &e){
     std::cout<<e.what()<<std::endl;
  }
    
SqlWrapper MySqlConnector;

MySqlConnector.Connect("localhost","sqqt", "1234","sqqtDB");
std:: string querryRes = MySqlConnector.SendQuerry("SELECT * FROM users");
//std::cout << querryRes<<std::endl;

    ThreadPool pool(PoolSize);
    Hqqt::Broker broker;
     pool.enqueue(BrokerThread, std::ref(broker));

    ServerSocket server ( ServPort );
    std::cout << "Server is running...."<<std::endl;




    int CliCntr;
    while(1) {
      ServerSocket  * cliSocket = new(ServerSocket);
      server.accept (*cliSocket );
     
        MySqlConnector.SendQuerry("INSERT INTO users (id, status, timeLogin) VALUES ('"+std::string(cliSocket->get_cli_addr())+"', '"+std::to_string(CliCntr)+"', '"+GetTimeNow()+"')");
  //      std:: string querryRes3 = MySqlConnector.SendQuerry("SELECT * FROM users");
   //     std::cout << querryRes3<<std::endl;
        CliCntr++;
      pool.enqueue(WorkerThread, std::ref(*cliSocket));
    }



    std::cout << "Server end"<<std::endl;
    
    
	
    return 0;
}






