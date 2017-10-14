#include <iostream>
#include <string>
#include <iomanip>
#include <csignal>
#include <regex>
#include <cstdlib>
#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include "AgendaUI.hpp"
#include "md5.hpp"
#include "AgendaService.hpp"

using namespace std;

int mygetch(void){
  struct termios oldt, newt;
  int ch; 
  tcgetattr( STDIN_FILENO, &oldt ); // 记录旧设置
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );//设置不回显
  tcsetattr( STDIN_FILENO, TCSANOW, &newt ); // 设置新的
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt ); // 恢复旧的设置
  return ch; 
}

bool judge_phone_number(string phone){
  for(size_t i=0;i<phone.size();i++){
    if(!isdigit(phone[i]))
      return false;
  }
  return true;
}

string password_hide(){

    char pass[100];
    int len=0;
    char c;
    while(1){
      c=mygetch();
      if(c=='\n'){
        pass[len]='\0';
        cout<<endl;
        break;
      }
      pass[len]=c;
      len++;
      cout<<"*"; 
    }
    string s=pass;
    return s;
}

  AgendaUI::AgendaUI(){

    m_userName="";
    m_userPassword="";
    startAgenda();
  }

  void AgendaUI::OperationLoop(void){
  	string choice;
    while(1){
      if(!m_userName.size()){

      cout<<"-------------------------------------Agenda-------------------------------------"<<endl;
      cout<<"Action :"<<endl;
      cout<<"l    - log in Agenda by user name and password"<<endl;
      cout<<"r    - register an Agenda account"<<endl;
      cout<<"q    - quit Agenda"<<endl;
      cout<<"--------------------------------------------------------------------------------"<<endl<<endl;
      cout<<"Agenda :~$ ";
      // m_agendaService=AgendaService();
      // string choice;
      // cin>>choice;
      // cout<<endl;
      // while(choice.size()!=1||!(choice[0]=='l'||choice[0]=='r'||choice[0]=='q')){
      // cout<<"Error input,please input again.\n";
      // cin>>choice;
    // switch(choice[0]){
    //   case 'l':
    //       userLogIn();
    //       break;
    //   case 'r':
    //       userRegister();
    //       break;
    //   case 'q':
    //       quitAgenda();
    //       break;
    // }
      // }
    }
    else{
      cout<<"-------------------------------------Agenda-------------------------------------"<<endl;
      cout<<"Action :\n";
      cout<<"o    - log out Agenda\n";
      cout<<"dc   - delete Agenda account\n";
      cout<<"lu   - list all Agenda user\n";
      cout<<"cm   - create a meeting\n";
      cout<<"amp  - add meeting participator\n";
      cout<<"rmp  - remove meeting participator\n";
      cout<<"rqm  - request to quit meeting\n";
      cout<<"la   - list all meetings\n";
      cout<<"las  - list all sponsor meetings\n";
      cout<<"lap  - list all participator meetings\n";
      cout<<"qm   - query meeting by title\n";
      cout<<"qt   - query meeting by time interval\n";
      cout<<"dm   - delete meeting by title\n";
      cout<<"da   - delete all meetings\n";
      cout<<"--------------------------------------------------------------------------------"<<endl<<endl;
      cout<<"Agenda@"<<m_userName<<" :~# ";
    }
    choice=getOperation();
    if(choice=="q"&&m_userName==""){
    	quitAgenda();
    	return;
    }
   	while(!executeOperation(choice)){
    	cout<<"Error input,please input again.\n";
    	choice=getOperation();
    	}
    }
  }
  /**
   * constructor
   */
  void AgendaUI::startAgenda(void){
      system("clear");
      m_agendaService=AgendaService();
  }

  /**
   * catch user's operation
   * @return the operation
   */
  string AgendaUI::getOperation(){

    string s;
    cin>>s;
    return s;
  }

  /**
   * execute the operation
   * @return if the operationloop continue
   */
  bool AgendaUI::executeOperation(const string &ope){
    bool judge=true;
    if(!m_userName.size())
      judge=false;
    cout<<endl;
    if(judge){
      if(ope=="o"){
        userLogOut();
        return true;
      }
      else if(ope=="dc"){
        deleteUser();
        return true;
      }
      else if(ope=="lu"){
        listAllUsers();
        return true;
      }
      else if(ope=="cm"){
        createMeeting();
        return true;
      }
      else if(ope=="amp"){
        addMeetingParticipator();
        return true;
      }
      else if(ope=="rmp"){
        removeMeetingParticipator();
        return true;
      }
      else if(ope=="rqm"){
        quitMeeting();
        return true;
      }
      else if(ope=="la"){
        listAllMeetings();
        return true;
      }
      else if(ope=="las"){
        listAllSponsorMeetings();
        return true;
      }
      else if(ope=="lap"){
        listAllParticipateMeetings();
        return true;
      }
      else if(ope=="qm"){
        queryMeetingByTitle();
        return true;
      }
      else if(ope=="qt"){
        queryMeetingByTimeInterval();
        return true;
      }
      else if(ope=="dm"){
        deleteMeetingByTitle();
        return true;
      }
      else if(ope=="da"){
        deleteAllMeetings();
        return true;
      }
      return false;
    }
    if(ope=="l"){
      userLogIn();
      return true;
    }
    else if(ope=="r"){
      userRegister();
      return true;
    }
    else if(ope=="q"){
      m_agendaService.quitAgenda();
      return true;
    }
    
    return false;
  }

  /**
   * user Login
   */
  void AgendaUI::userLogIn(void){

        string username,password;
        cout<<"[log in] [username] [password]"<<endl;
        cout<<"[username] ";
        getchar();
        getline(cin,username);
        cout<<"[password] ";
        
        password=password_hide();

        cout<<"[log in] ";
        string classify=getMD5(password);
        if(m_agendaService.userLogIn(username,classify)){
          m_userName=username;
          m_userPassword=classify;
          m_agendaService.quitAgenda();
          m_agendaService=AgendaService();
          cout<<"succeed!"<<endl<<endl;
        }
        else{
          cout<<"Password error or user doesn't exist"<<endl<<endl;
        }

  }

  /**
   * user regist
   */
  void AgendaUI::userRegister(void){
        string username,password,email,phone;
        cout<<"[register] [username] [password] [email] [phone]"<<endl;
        cout<<"Attention,please follow the international domain name rule to sign up a email address!\n";
        cout<<"Which only begins with letter,and allow letters,'-',and digits in the domain name\n";
        cout<<"And domain address must follow with @ and end up with .XXXX\n";
        cout<<"The phone number must has and only has 11 digits\n";
        cout<<"The username can has some spaces in it,but password can't\n\n\n";
        cout<<"[username] ";
         getchar();
         getline(cin,username);
         cout<<"[password] ";
         password=password_hide();
         cout<<"[Please input your password again] ";
         string s=password_hide();
         while(s!=password){
          cout<<"Error,set your password again.\n";
          cout<<"[password] ";
          password=password_hide();
          cout<<"[Please input your password again] ";
          s=password_hide();
         }
         cout<<"[email] ";
         cin>>email;
          regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");

          while(!regex_match(email,pattern))
          {
              cout<<"Wrong input format,please input again."<<endl;
              cin>>email;
          }
          cout<<"[phone] ";
          cin>>phone;
          while(phone.size()!=11||!judge_phone_number(phone))
          {
              cout<<"Wrong input format,please input again."<<endl;
              cin>>phone;
          }
          cout<<"[register] ";
          string classify=getMD5(password);
          if(m_agendaService.userRegister(username,classify,email,phone)){
          	m_agendaService.quitAgenda();
            	m_agendaService=AgendaService();
          	cout<<"succeed!\n";
          }
          else
            cout<<"This username has been registerd!\n";
          cout<<endl;
  }


  /**
   * user logout
   */
  void AgendaUI::userLogOut(void){
    m_userName="";
    m_userPassword="";
    cout<<endl<<endl;
    m_agendaService.quitAgenda();
  }

  /**
   * quit the Agenda
   */
  void AgendaUI::quitAgenda(void){
    m_agendaService.quitAgenda();
  }

  /**
   * delete a user from storage
   */
  void AgendaUI::deleteUser(void){
    cout<<"[delete agenda account] ";
    if(m_agendaService.deleteUser(m_userName,m_userPassword)){
      m_userName="";
      m_userPassword="";
      cout<<"succeed!\n";
    }
    else
      cout<<"Error!There is not have this user account\n";
  }

  /**
   * list all users from storage
   */
  void AgendaUI::listAllUsers(void){
    cout<<"[list all users]\n\n";
    cout<<setiosflags(ios::left);
    cout<<setw(20)<<"name"<<setw(30)<<"email"<<"phone"<<endl;
  
    list<User> ul=m_agendaService.listAllUsers();
    for(auto &re:ul){
      cout<<setw(20)<<re.getName()<<setw(30)<<re.getEmail()<<re.getPhone()<<endl;
    }
    cout<<endl;
    cout<<resetiosflags(ios::left);
  }

  /**
   * user create a meeting with someone else
   */
  void AgendaUI::createMeeting(void){
    int number;
    vector<string> participators;
    string start_date,end_date,participator,title;
    cout<<"[create meeting] [the number of participators]\n";
    cout<<"[create meeting] ";
    cin>>number;
    getchar();
    for(int i=0;i<number;i++){
      cout<<"[create meeting] [please enter the participator "<<i+1<<" ]\n";
      cout<<"[create meeting] ";
      getline(cin,participator);
      participators.push_back(participator); 
    }
    cout<<"[create meeting]\n";
    cout<<"[title] ";
    cin>>title;
    cout<<"[start time(yyyy-mm-dd/hh:mm)] ";
    cin>>start_date;
    while(!Date::isValid(Date::stringToDate(start_date))){
      cout<<"Please check your start_date format or logic,and input again\n";
      cin>>start_date;
    }
    cout<<"[end time(yyyy-mm-dd/hh:mm)] ";
    cin>>end_date;
    while(!Date::isValid(Date::stringToDate(end_date))){
      cout<<"Please check your start_date format or logic,and input again\n";
      cin>>end_date;
    }
    cout<<"[create meeting] ";
    if(m_agendaService.createMeeting(m_userName,title,start_date,end_date,participators))
      cout<<"succeed!\n";
    else
      cout<<"Error!Please check the input format or whether the date is valid\n";

  }

  /**
   * sponsor add a participator to the meeting
   */
  void AgendaUI::addMeetingParticipator(void){
    cout<<"[add participator] [meeting title] [participator username]\n";
    string title,participator;
    cout<<"[add participator] ";
    cin>>title;
    getchar();
    getline(cin,participator);
    cout<<"[add participator] ";
    if(m_agendaService.addMeetingParticipator(m_userName,title,participator))
      cout<<"succeed!\n";
    else
      cout<<"There is not have this meeting,or has time conflict\n";
  }

  /**
   * sponsor add a participator to the meeting
   */
  void AgendaUI::removeMeetingParticipator(void){
    cout<<"[remove participator] [meeting title] [participator username]\n";
    string title,participator;
    cout<<"[remove participator] ";
    cin>>title;
    getchar();
    getline(cin,participator);
    cout<<"[remove participator] ";
    if(m_agendaService.removeMeetingParticipator(m_userName,title,participator))
      cout<<"succeed!\n";
    else
      cout<<"This meeting is not have this participator!\n";
  }

  /**
   * user quit from meeting
   */
  void AgendaUI::quitMeeting(void){
    cout<<"[quit meeting] [meeting title]\n";
    string title;
    cout<<"[quit meeting] ";
    cin>>title;
    cout<<"[quit meeting] ";
    if(m_agendaService.quitMeeting(m_userName,title))
      cout<<"succeed!\n";
    else
      cout<<"This meeting is not have this participator\n";
  }

  /**
   * list all meetings from storage
   */
  void AgendaUI::listAllMeetings(void){
    cout<<"[list all meetings]\n\n";
    list<Meeting> ml=m_agendaService.listAllMeetings(m_userName);
    if(!ml.size())
      cout<<"There is not have this meeting\n\n";
    else
      printMeetings(ml);

  }

  /**
   * list all meetings that this user sponsored
   */
  void AgendaUI::listAllSponsorMeetings(void){
    cout<<"[list all sponsor meetings]\n\n";
    list<Meeting> ml=m_agendaService.listAllSponsorMeetings(m_userName);
    if(!ml.size())
      cout<<"There is not have this meeting\n\n";
    else
      printMeetings(ml);

  }

  /**
   * list all meetings that this user take part in
   */
  void AgendaUI::listAllParticipateMeetings(void){
    cout<<"[list all participator meetings]\n\n";
    list<Meeting> ml=m_agendaService.listAllParticipateMeetings(m_userName);
    if(!ml.size())
      cout<<"There is not have this meeting\n\n";
    else
      printMeetings(ml);
  }

  /**
   * search meetings by title from storage
   */
  void AgendaUI::queryMeetingByTitle(void){
    cout<<"[query meeting] [title]:\n";
    cout<<"[query meeting] ";
    string title;
    cin>>title;
    list<Meeting> ml=m_agendaService.meetingQuery(m_userName,title);
    if(!ml.size())
      cout<<"There is not have this meeting\n\n";
    else
      printMeetings(ml);
  }

  /**
   * search meetings by timeinterval from storage
   */
  void AgendaUI::queryMeetingByTimeInterval(void){
    cout<<"[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n";
    cout<<"[query meeting] ";
    string start_date,end_date;
    cin>>start_date>>end_date;
    list<Meeting> ml=m_agendaService.meetingQuery(m_userName,start_date,end_date);
    if(!ml.size())
      cout<<"None meeting during this time\n\n";
    else
      printMeetings(ml);
  }

  /**
   * delete meetings by title from storage
   */
  void AgendaUI::deleteMeetingByTitle(void){
    cout<<"[delete meeting] [title]\n";
    cout<<"[delete meeting] ";
    string title;
    cin>>title;
    cout<<"[delete meeting] ";
    if(m_agendaService.deleteMeeting(m_userName,title))
      cout<<"succeed!\n";
    else
      cout<<"There is not have this meeting\n";
  }

  /**
   * delete all meetings that this user sponsored
   */
  void AgendaUI::deleteAllMeetings(void){
    cout<<"[delete all meetings] ";
    if(m_agendaService.deleteAllMeetings(m_userName))
      cout<<"succeed!\n";
    else
      cout<<"There is not have this sponsor\n";
  }

  /**
   * show the meetings in the screen
   */
  void AgendaUI::printMeetings(const list<Meeting> &t_meetings){
    cout<<setiosflags(ios::left);
    cout<<setw(10)<<"title"<<setw(10)<<"sponsor"<<setw(20)<<"start time"<<setw(20)<<"end time"<<"participator"<<endl;
    vector<string> v;
    for(auto &re:t_meetings){
      cout<<setw(10)<<re.getTitle()<<setw(10)<<re.getSponsor()<<setw(20)<<Date::dateToString(re.getStartDate())<<setw(20)<<Date::dateToString(re.getEndDate());
      v=re.getParticipator();
      for(size_t i=0;i<v.size();i++){
        if(i!=v.size()-1)
          cout<<v[i]<<",";
        else
          cout<<v[i];
      }      
      cout<<endl;
    }
    cout<<resetiosflags(ios::left);
    cout<<endl;
  }
