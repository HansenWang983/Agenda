#include <list>
#include <string>
#include <set>
#include <iostream>
#include "Storage.hpp"
#include "AgendaService.hpp"
#include "md5.hpp"

using namespace std;

bool create_meeting_judge(const string &userName,const string &startDate,const string &endDate,const list<Meeting> &temp);


  /**
   * constructor
   */
  AgendaService::AgendaService(){
   startAgenda();
  }

  /**
   * destructor
   */
  AgendaService::~AgendaService(){
   quitAgenda();
  }

  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
  bool AgendaService::userLogIn(const string &userName, const string &password){

    // string s=getMD5(password);
   list<User> ul=m_storage->queryUser([&](const User &u){
    return (u.getName()==userName)&&(u.getPassword()==password);});
   return !ul.empty();

  }

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
  bool AgendaService::userRegister(const string &userName, const string &password,
                    const string &email, const string &phone){

   
    // if(userName.size()==0||password.size()==0||email.size()==0||phone.size()==0)
    // 	return false;
   list<User> ul=m_storage->queryUser([&userName](const User &uu){
    return (uu.getName()==userName);});

   if(ul.empty()){
    m_storage->createUser(User(userName,password,email,phone));
    return true;
   }
   return false;
  }

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
  bool AgendaService::deleteUser(const string &userName, const string &password){

	list<Meeting> ml;
  // string s=getMD5(password);
   int m=m_storage->deleteUser([&](const User &uu){
  if(uu.getName()==userName&&uu.getPassword()==password)
   return true;
  return false;});
   if(m){
    deleteAllMeetings(userName);
    ml=listAllParticipateMeetings(userName);
    for(auto i=ml.begin();i!=ml.end();i++){
		removeMeetingParticipator(i->getSponsor(),i->getTitle(),userName);
    }
    return true;
   }
   return false;
 //   list<User> l;
 //   l=m_storage->queryUser([&userName](const User &u){
 //    if(u.getName()==userName)
 //     return true;
 //    else
 //    	return false;
 //   });
 //   if(l.empty())
 //    return false;
 //   if(password!=l.begin()->getPassword())
 //    return false;
 //   m_storage->deleteUser([&userName](const User &u){
 //    return (u.getName()==userName);});

 //   m_storage->deleteMeeting([&userName](const Meeting &mmm){
 // if(mmm.getSponsor()==userName||mmm.isParticipator(userName))
 //  return true;
 // else
 // 	return false;
 // });
 //   return true;

  }

  /**
   * list all users from storage
   * @return a user list result
   */
  list<User> AgendaService::listAllUsers(void) const{
   list<User> u=m_storage->queryUser([](const User &uu){return true;});
   return u;
  }

  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startDate the meeting's start date
   * @param endDate the meeting's end date
   * @return if success, true will be returned
   */
  bool AgendaService::createMeeting(const string &userName, const string &title,
                     const string &startDate, const string &endDate,
                     const vector<string> &participator){

	if(participator.size()==0)
  		return false;
   // for(auto i=participator.begin();i!=participator.end();i++){
   // 	for(auto j=participator.begin();j!=i;j++){
   // 		if(*j==*i)
   // 			return false;
   // 	}
   
   Date sd(startDate);
   Date ed(endDate);

   if(!Date::isValid(sd))
    return false;
	if(!Date::isValid(ed))
		return false;

   if(sd>=ed)
    return false;
	
	set<string> part;

	if(m_storage->queryMeeting([=](const Meeting &m){
		return m.getTitle()==title;
	}).size()>0) return false;

	for(auto &i:participator){
		if(i==userName)
			return false;
		if(m_storage->queryUser([=](const User &u){
			return u.getName()==i;
		}).size()!=1) return false;
		part.insert(i);
	}
	if(part.size()!=participator.size())
		return false;
	part.insert(userName);

	Meeting mmm(userName,participator,startDate,endDate,title);

	for(auto &re:part){
		list<Meeting> ml=m_storage->queryMeeting([=](const Meeting &m){
			if(m.getSponsor()!=re&&!m.isParticipator(re))
				return false;
			if(m.getStartDate()>=ed||m.getEndDate()<=sd)
				return false;
			else
				return true;
		});
		if(ml.size()>0)
			return false;
	}

 	m_storage->createMeeting(mmm);
    return true;

//    list<User> ul=m_storage->queryUser([userName](const User &uu){
//     if(uu.getName()==userName)
//      return true;
//     return false;});
//    if(ul.size()!=0)
//     return false;



//     list<Meeting> temp=listAllMeetings(userName);

   
//    // if(create_meeting_judge(userName,startDate,endDate,temp))
//    // 	  return false;
//     Date ssd,eed;
// 	for(auto i=temp.begin();i!=temp.end();i++){
// 		if(i->getSponsor()==userName){
// 			ssd=i->getStartDate();
// 			eed=i->getEndDate();
// 			if(!(Date::stringToDate(startDate)>=eed||Date::stringToDate(endDate)<=ssd))
//        			return false;
// 		}
// 	}

//  list<Meeting> ml;
//  for(auto i=participator.begin();i!=participator.end();i++){
//   ml=listAllMeetings(*i);
//   // if(create_meeting_judge(*i,startDate,endDate,ml))
//   // 	return false;
  
//   Date sd,ed;
//   auto j=ml.begin();
//      for(;j!=ml.end();j++){
//       sd=j->getStartDate();
//       ed=j->getEndDate();
//       if(!(Date::stringToDate(startDate)>=ed||Date::stringToDate(endDate)<=sd))
//        return false;
//      }
 
// }
  
  }

  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::addMeetingParticipator(const string &userName,
                              const string &title,
                              const string &participator){

  
    bool judge=false;
   Date sdate,edate;
   list<User> ul=m_storage->queryUser([participator](const User &u){
   	if(u.getName()==participator)
   		return true;
   	return false;});
   if(!ul.size())
   	return false;
   list<Meeting> ml=listAllMeetings(participator);
    m_storage->updateMeeting([userName,title](const Meeting &mm){
    if(mm.getSponsor()==userName&&mm.getTitle()==title)
     return true;
    return false;},[participator,&sdate,&edate,&ml,&judge](Meeting &mmm){
     sdate=mmm.getStartDate();
     edate=mmm.getEndDate();
     Date sd,ed;
      if(mmm.isParticipator(participator))
          return;
     auto i=ml.begin();
     for(;i!=ml.end();i++){
      sd=i->getStartDate();
      ed=i->getEndDate();
      if(!(sdate>=ed||edate<=sd))
       return;
     }
      mmm.addParticipator(participator);
      judge=true;
      return;
    });
   return judge;

  }

  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::removeMeetingParticipator(const string &userName,
                                 const string &title,
                                 const string &participator){
  
    bool judge=false;
    int count=0;
    if(userName==participator)
    	return false;
    m_storage->updateMeeting([userName,title](const Meeting &mm){
    if(mm.getSponsor()==userName&&mm.getTitle()==title)
     return true;
    return false;},[participator,&count,&judge](Meeting &mmm){
     if(mmm.isParticipator(participator)){
      judge=true;
      mmm.removeParticipator(participator);
  if(mmm.getParticipator().size()==0)
        count=1;
     }
    });
   if(count)
    deleteMeeting(userName,title);
   return judge;
  }

  /**
   * quit from a meeting
   * @param userName the current userName. no need to be the sponsor
   * @param title the meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::quitMeeting(const string &userName, const string &title){
  
   bool judge=false;
   string sponsor;
   m_storage->updateMeeting([title,userName](const Meeting &mm){
    if(mm.getTitle()==title&&userName!=mm.getSponsor())
     return true;
    return false;},[&sponsor,&judge,&userName](Meeting &mmm){
      if(mmm.isParticipator(userName)){
        judge=true;
        sponsor=mmm.getSponsor();
      }
    });
    if(judge){
      removeMeetingParticipator(sponsor,title,userName);
    }
   return judge;
  }

  /**
   * search a meeting by username and title
   * @param uesrName the sponsor's userName
   * @param title the meeting's title
   * @return a meeting list result
   */
  list<Meeting> AgendaService::meetingQuery(const string &userName,
                                  const string &title) const{
   // vector<string> v;
   list<Meeting> ml=m_storage->queryMeeting([&userName,&title](const Meeting &mm){
    if(mm.getTitle()==title)
    {
     	if(mm.getSponsor()==userName)
      		return true;
   		if(mm.isParticipator(userName))
    		return true;
   		return false;
   }
   return false;});
    return ml;
  }
  /**
   * search a meeting by username, time interval
   * @param uesrName the sponsor's userName
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
  list<Meeting> AgendaService::meetingQuery(const string &userName,
                                  const string &startDate,
                                  const string &endDate) const{
   // vector<string> v;
    list<Meeting> ml;
    Date sd=Date::stringToDate(startDate);
    Date ed=Date::stringToDate(endDate);
    if(!Date::isValid(sd))
     	return ml;
 	if(!Date::isValid(ed))
 		return ml;
    if(Date::stringToDate(startDate)>=Date::stringToDate(endDate))
     return ml;
    ml=m_storage->queryMeeting([&userName,&sd,&ed](const Meeting &mm){
    if(mm.getSponsor()==userName||mm.isParticipator(userName)){
      if(mm.getStartDate()>=sd&&mm.getEndDate()<=sd)
        return true;
      if(mm.getStartDate()<=ed&&mm.getStartDate()>=sd)
        return true;
      if(mm.getEndDate()>=sd&&mm.getEndDate()<=ed)
        return true;
       return false;
    }
    return false;});

    return ml;
  }

  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result
   */
  list<Meeting> AgendaService::listAllMeetings(const string &userName) const{
  
    list<Meeting> ml=m_storage->queryMeeting([userName](const Meeting &mm){
    if(mm.getSponsor()==userName)
     return true;
  if(mm.isParticipator(userName))
   return true;
  return false;});

   return ml;

  }

  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
  list<Meeting> AgendaService::listAllSponsorMeetings(const string &userName) const{
   list<Meeting> ml=m_storage->queryMeeting([userName](const Meeting &mm){
    if(mm.getSponsor()==userName)
     return true;
    return false;});
   return ml;
  }

  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
 list<Meeting> AgendaService::listAllParticipateMeetings(const string &userName) const{
      

   list<Meeting> ml=m_storage->queryMeeting([userName](const Meeting &mm){
    if(mm.isParticipator(userName))
     return true;
    return false;});

   return ml;

  }


  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::deleteMeeting(const string &userName, const string &title){
   int m=m_storage->deleteMeeting([userName,title](const Meeting &mm){
  if(mm.getSponsor()==userName&&mm.getTitle()==title)
   return true;
  return false;});
   return m;
  }

  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
  bool AgendaService::deleteAllMeetings(const string &userName){
 int m=m_storage->deleteMeeting([userName](const Meeting &mm){
  if(mm.getSponsor()==userName)
   return true;
  return false;});
   return m;
  }

  /**
   * start Agenda service and connect to storage
   */
  void AgendaService::startAgenda(void){
 m_storage=Storage::getInstance();
  }

  /**
   * quit Agenda service
   */
  void AgendaService::quitAgenda(void){
   m_storage->sync();
  }




// bool create_meeting_judge(const string &userName,
//                                   const string &startDate,
//                                   const string &endDate,const list<Meeting> &temp){

//  bool judge=false;
//     for(auto i=temp.begin();i!=temp.end();i++){
//       if(i->getSponsor()==userName||i->isParticipator(userName)){
//         if(i->getStartDate()>=Date::stringToDate(startDate)&&i->getEndDate()<=Date::stringToDate(endDate))
//           judge=true;
//         if(i->getStartDate()>=Date::stringToDate(startDate)&&i->getStartDate()<Date::stringToDate(endDate))
//           judge=true;
//         if(i->getEndDate()<=Date::stringToDate(endDate)&&i->getEndDate()>Date::stringToDate(startDate))
//           judge=true;
//        else
//          judge=false;
//       }
//     }
//     return judge;
// }

