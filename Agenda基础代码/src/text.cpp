#include <iostream>
#include <string>
#include <vector>
#include <csignal>
#include "Storage.hpp"
#include "AgendaService.hpp"
#include "AgendaUI.hpp"

using namespace std;


AgendaUI a;


void signalhandller(int signal){
	cout<<endl<<"Saveing..."<<endl;
	// saveAgenda();
	exit(signal);
}


int main(int argc, char const *argv[])
{

	signal(SIGINT,signalhandller);

	a.OperationLoop();

	return 0;
}




	// shared_ptr<Storage> p=Storage::getInstance();	
	// Date d1("2026-07-31/12:62");
	// Date d2("2016-07-29/12:00");

	// AgendaService a;

	// list<User> ul=a.listAllUsers();

	// for(auto &re:ul)
	//  	 cout<<re.getName()<<endl;

	// // cout<<a.deleteUser("haha","haha")<<endl;

	//  vector<string> v={"b"};

	//  string b="b";

	 // cout<<a.createMeeting("a","niu2",)
	 // cout<<a.removeMeetingParticipator("a","niu2","b")<<endl;

	// cout<<a.quitMeeting("b","niu3")<<endl;

	 // cout<<a.deleteUser("maomao","123")<<endl;
	// for(auto &re:temp)
	//  	 cout<<re.getTitle()<<endl;


	 	// cout<<a.removeMeetingParticipator("a","niu","ta")<<endl;

	 // cout<<a.quitMeeting("wo","nui")<<endl;
	  // cout<<a.quitMeeting("wo","nui")<<endl;

	 // list<Meeting> ml=a.listAllMeetings("a");

	 // for(auto &re:ml)
	 // 	 cout<<re.getSponsor()<<endl;

	 // 	cout<<a.deleteAllMeetings("haha")<<endl;





	// vector<string> ss={"aaa","bbb"};
	// Meeting u("wang",ss,d1,d2,"www");
	// //p->createMeeting(u);
	// // int m=p->deleteMeeting([](const Meeting &uu){
	// // 	if(uu.getSponsor()=="wang")
	// // 		return true;
	// // 	return false;});
	
	// // cout<<m<<endl;

	// User user("www","www","www","www");
	// // p->createUser(user);

	// int m=p->deleteUser([](const User &uu){
	// 	if(uu.getName()=="www")
	// 		return true;
	// 	return false;});
	
	// cout<<m<<endl;
	// list<User> li=p->queryUser([](const User &user){return true;});

	// for(auto &re:li)
	// 	 cout<<re.getName()<<endl;
	// cout<<Date::dateToString(d1)<<endl<<Date::dateToString(d2)<<endl;
		

	// list<Meeting> li=p->queryMeeting([](const Meeting &u){return true;});
	// auto i=li.begin();
	// i++;
	// vector<string> v=i->getParticipator();
	// for(auto j=v.begin();j!=v.end();j++){
	// 	cout<<*j<<endl;
	// }
	// // for(auto &re:li)
	// // 	 cout<<Date::dateToString(re.getEndDate())<<endl;
	// // 	//cout<<re.getTitle()<<endl;