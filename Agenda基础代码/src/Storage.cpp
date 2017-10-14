#include <functional>
#include <list>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "Meeting.hpp"
#include "User.hpp"
#include "Path.hpp"
#include "Storage.hpp"
#include "md5.hpp"

using namespace std;

shared_ptr<Storage> Storage::m_instance=nullptr;

Meeting stringToMeeting(string s)
{
  for(size_t i=0;i<s.size();i++)
  {
    if(s[i]==' ')
      s[i]='_';
  }
  for(size_t i=0;i<s.size();i++)
  {
    if(s[i]=='"'||s[i]==',')
      s[i]=' ';
  }
  string sponsor,title,sdate,edate,part;
  stringstream ss;
  ss<<s;
  ss>>sponsor>>part>>sdate>>edate>>title;
  ss.clear();
  for(size_t i=0;i<sponsor.size();i++){
    if(sponsor[i]=='_')
      sponsor[i]=' ';
  }
  for(size_t i=0;i<title.size();i++){
    if(title[i]=='_')
      title[i]=' ';
  }
  Date start_date(sdate);
  Date end_date(edate);
  vector<string> v;
  int count=1;
  for(size_t i=0;i<part.size();i++)
  {
    if(part[i]=='&'){
      part[i]=' ';
      count++;
    }
  }
  ss<<part;
  for(int i=0;i<count;i++){
    string temp;
    ss>>temp;
    for(size_t j=0;j!=temp.size();j++){
      if(temp[j]=='_')
        temp[j]=' ';
    }
    v.push_back(temp);
  }
  Meeting m(sponsor,v,start_date,end_date,title);
  return m;
}

User stringToUser(string s)
{
  for(size_t i=0;i<s.size();i++)
  {
    if(s[i]==' ')
      s[i]='_';
  }
  for(size_t i=0;i<s.size();i++)
  {
    if(s[i]=='"'||s[i]==',')
      s[i]=' ';
  }
  string name,password,email,phone;
  stringstream ss;
  ss<<s;
  ss>>name>>password>>email>>phone;
  for(size_t i=0;i<name.size();i++){
    if(name[i]=='_')
      name[i]=' ';
  }
  for(size_t i=0;i<password.size();i++){
    if(password[i]=='_')
      password[i]=' ';
  }
  for(size_t i=0;i<email.size();i++){
    if(email[i]=='_')
      email[i]=' ';
  }
  for(size_t i=0;i<phone.size();i++){
    if(phone[i]=='_')
      phone[i]=' ';
  }
  // cout<<password<<endl;
  User u(name,password,email,phone);
  return u;
}
  /**
  *   default constructor
  */
  Storage::Storage(){
    m_dirty=false;
    readFromFile();
  }


  /**
  *   read file content into memory
  *   @return if success, true will be returned
  */
  bool Storage::readFromFile(void){
    ifstream Meeting_in(Path::meetingPath);
    ifstream User_in(Path::userPath);
    if(!Meeting_in&&!User_in)
       return false;
    string s;
    while(getline(Meeting_in,s))
    {
      Meeting m=stringToMeeting(s);
      m_meetingList.push_back(m);
    }
    string ss;
    while(getline(User_in,ss))
    {
      User u=stringToUser(ss);
      m_userList.push_back(u);
    }
    Meeting_in.close();
    User_in.close();
    return true;
  }

  /**
  *   write file content from memory
  *   @return if success, true will be returned
  */
  bool Storage::writeToFile(void){
    ofstream Meeting_out(Path::meetingPath);
    ofstream User_out(Path::userPath);
    if(!Meeting_out.is_open()&&!User_out.is_open())
      return false;
    size_t count=0;
    for(auto i=m_userList.begin();i!=m_userList.end();i++)
    {
      // string s=getMD5(i->getPassword());
      User_out<<"\""<<i->getName()<<"\",\""<<i->getPassword()<<"\",\""<<i->getEmail()<<"\",\""<<i->getPhone()<<"\"";
      // if(i!=m_userList.end()-1)
      if(count!=m_userList.size()-1)
        User_out<<endl;
      count++;
    }
    User_out.close();
    size_t count1=0;
    // “A”,“B&C”,“2016-08-01/00:00”,“2016-08-01/12:00”,”meeting“ 
    for(auto i=m_meetingList.begin();i!=m_meetingList.end();i++)
    {
      Meeting_out<<'\"'<<i->getSponsor()<<"\",\"";
      vector<string> v=i->getParticipator();
      size_t count=0;
      for(auto j=v.begin();j!=v.end();j++)
      {
          if(count!=v.size()-1)
            Meeting_out<<*j<<'&';
          else 
            Meeting_out<<*j;
          count++;
      }
      Meeting_out<<"\",\""<<Date::dateToString(i->getStartDate())<<"\",\""<<Date::dateToString(i->getEndDate())<<"\",\""<<i->getTitle()<<'\"';
      // if(i!=m_meetingList.end()-1)
     if(count1!=m_meetingList.size()-1)
        Meeting_out<<endl;
      count1++;
    }
    Meeting_out.close();
    return true;

  }

  /**
  * get Instance of storage
  * @return the pointer of the instance
  */
  shared_ptr<Storage> Storage::getInstance(void){
    if(m_instance)
      return m_instance;
    m_instance.reset(new Storage);
    return m_instance;
  }

  /**
  *   destructor
  */
  Storage::~Storage(){
    writeToFile();
  }

  // CRUD for User & Meeting
  // using C++11 Function Template and Lambda Expressions

  /**
  * create a user
  * @param a user object
  */
  void Storage::createUser(const User &t_user){
    m_userList.push_back(t_user);
    m_dirty=true;
  }

  /**
  * query users
  * @param a lambda function as the filter
  * @return a list of fitted users
  */
  list<User> Storage::queryUser(function<bool(const User &)> filter) const{
    list<User> query_list;
    for(auto i=m_userList.begin();i!=m_userList.end();i++)
    {
      if(filter(*i))
        query_list.push_back(*i);
    }
    return query_list;
  }

  /**
  * update users
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the user
  * @return the number of updated users
  */
  int Storage::updateUser(function<bool(const User &)> filter,
                 function<void(User &)> switcher){
    int number=0;
    for(auto i=m_userList.begin();i!=m_userList.end();i++){
      if(filter(*i))
      {
        switcher(*i);
        number++;
      }
    }
    m_dirty=true;
    return number;
  }

  /**
  * delete users
  * @param a lambda function as the filter
  * @return the number of deleted users
  */
  int Storage::deleteUser(function<bool(const User &)> filter){
    int number=0;
    auto it=m_userList.begin();
    while(it!=m_userList.end()){
      if(filter(*it)){
        it=m_userList.erase(it);
        number++;
      }
      else
        ++it;
    }
    m_dirty=true;
    return number;
  }

  /**
  * create a meeting
  * @param a meeting object
  */
  void Storage::createMeeting(const Meeting &t_meeting){
    m_meetingList.push_back(t_meeting);
    m_dirty=true;
  }

  /**
  * query meetings
  * @param a lambda function as the filter
  * @return a list of fitted meetings
  */
  list<Meeting> Storage::queryMeeting(
      function<bool(const Meeting &)> filter) const{
    list<Meeting> query_meeting;
    for(auto i=m_meetingList.begin();i!=m_meetingList.end();i++){
      if(filter(*i))
        query_meeting.push_back(*i);
    }
    return query_meeting;
  }

  /**
  * update meetings
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the meeting
  * @return the number of updated meetings
  */
  int Storage::updateMeeting(function<bool(const Meeting &)> filter,
                    function<void(Meeting &)> switcher){
    int number=0;
    for(auto i=m_meetingList.begin();i!=m_meetingList.end();i++){
      if(filter(*i))
      {
        switcher(*i);
        number++;
      }
    }
    m_dirty=true;
    return number;
  }

  /**
  * delete meetings
  * @param a lambda function as the filter
  * @return the number of deleted meetings
  */
  int Storage::deleteMeeting(function<bool(const Meeting &)> filter){
    int number=0;
    auto it=m_meetingList.begin();
    while(it!=m_meetingList.end()){
      if(filter(*it)){
        it=m_meetingList.erase(it);
        number++;
      }
      else
        ++it;
    }
    m_dirty=true;
    return number;
  }

  /**
  * sync with the file
  */
  bool Storage::sync(void){
    if(m_dirty){
      m_dirty=false;
      return writeToFile();
    }
    return false;
  }

