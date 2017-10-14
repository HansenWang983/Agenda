#include <vector>
#include <iostream>
#include "Date.hpp"
#include "Meeting.hpp"

using namespace std;
 
  /**
  *   @brief constructor with argument
  */
  Meeting::Meeting(const string &t_sponsor,
          const vector<string> &t_participator,
          const Date &t_startTime, const Date &t_endTime,
          const string &t_title):m_sponsor(t_sponsor),m_participators(t_participator),m_title(t_title){
    // string s1=Date::dateToString(t_startTime);
    // string s2=Date::dateToString(t_endTime);
    // cout<<s2;
    // m_startDate=Date::stringToDate(s1);
    // m_endDate=Date::stringToDate(s2);
    m_startDate.setYear(t_startTime.getYear());
    m_startDate.setMonth(t_startTime.getMonth());
    m_startDate.setDay(t_startTime.getDay());
    m_startDate.setHour(t_startTime.getHour());
    m_startDate.setMinute(t_startTime.getMinute());
    m_endDate.setYear(t_endTime.getYear());
    m_endDate.setMonth(t_endTime.getMonth());
    m_endDate.setDay(t_endTime.getDay());
    m_endDate.setHour(t_endTime.getHour());
    m_endDate.setMinute(t_endTime.getMinute());

  }

  /**
  * @brief copy constructor of left value
  */
  Meeting::Meeting(const Meeting &t_meeting)
  {
    m_sponsor=t_meeting.m_sponsor;
    m_participators=t_meeting.m_participators;
    // string s1=Date::dateToString(t_meeting.m_startDate);
    // string s2=Date::dateToString(t_meeting.m_endDate);
    // cout<<s2;
    // m_startDate=Date::stringToDate(s1);
    // m_endDate=Date::stringToDate(s2);
    m_startDate.setYear(t_meeting.m_startDate.getYear());
    m_startDate.setMonth(t_meeting.m_startDate.getMonth());
    m_startDate.setDay(t_meeting.m_startDate.getDay());
    m_startDate.setHour(t_meeting.m_startDate.getHour());
    m_startDate.setMinute(t_meeting.m_startDate.getMinute());
    m_endDate.setYear(t_meeting.m_endDate.getYear());
    m_endDate.setMonth(t_meeting.m_endDate.getMonth());
    m_endDate.setDay(t_meeting.m_endDate.getDay());
    m_endDate.setHour(t_meeting.m_endDate.getHour());
    m_endDate.setMinute(t_meeting.m_endDate.getMinute());
    m_title=t_meeting.m_title;
  }

  /**
  *   @brief get the meeting's sponsor
  *   @return a string indicate sponsor
  */
  string Meeting::getSponsor(void) const{return m_sponsor;}

  /**
  * @brief set the sponsor of a meeting
  * @param  the new sponsor string
  */
  void Meeting::setSponsor(const string &t_sponsor){m_sponsor=t_sponsor;}

  /**
  * @brief  get the participators of a meeting
  * @return return a string vector indicate participators
  */
  vector<string> Meeting::getParticipator(void) const{return m_participators;}

  /**
  *   @brief set the new participators of a meeting
  *   @param the new participators vector
  */
  void Meeting::setParticipator(const vector<string> &t_participators){m_participators=t_participators;}

  /**
  * @brief add a new participator to the meeting
  * @param the new participator
  */
  void Meeting::addParticipator(const string &t_participator){
    m_participators.push_back(t_participator);
  }

  /**
  * @brief remove a participator of the meeting
  * @param the participator to be removed
  */
  void Meeting::removeParticipator(const string &t_participator){
    // for(auto i=m_participators.begin();i!=m_participators.end();i++){
    //   if(*i==t_participator)
    //     m_participators.erase(i);
    // }
    auto i=m_participators.begin();
    while(i!=m_participators.end()){
      if(*i==t_participator){
          i=m_participators.erase(i);
      }
      else
        ++i;
    }
  }

  /**
  * @brief get the startDate of a meeting
  * @return return a string indicate startDate
  */
  Date Meeting::getStartDate(void) const{return m_startDate;}

  /**
  * @brief  set the startDate of a meeting
  * @param  the new startdate of a meeting
  */
  void Meeting::setStartDate(const Date &t_startTime){
    m_startDate.setYear(t_startTime.getYear());
    m_startDate.setMonth(t_startTime.getMonth());
    m_startDate.setDay(t_startTime.getDay());
    m_startDate.setHour(t_startTime.getHour());
    m_startDate.setMinute(t_startTime.getMinute());
  }

  /**
  * @brief get the endDate of a meeting
  * @return a date indicate the endDate
  */
  Date Meeting::getEndDate(void) const{return m_endDate;}

  /**
  * @brief  set the endDate of a meeting
  * @param  the new enddate of a meeting
  */
  void Meeting::setEndDate(const Date &t_endTime){
    m_endDate.setYear(t_endTime.getYear());
    m_endDate.setMonth(t_endTime.getMonth());
    m_endDate.setDay(t_endTime.getDay());
    m_endDate.setHour(t_endTime.getHour());
    m_endDate.setMinute(t_endTime.getMinute());
  }

  /**
  * @brief get the title of a meeting
  * @return a date title the endDate
  */
  string Meeting::getTitle(void) const{return m_title;}

  /**
  * @brief  set the title of a meeting
  * @param  the new title of a meeting
  */
  void Meeting::setTitle(const string &t_title){m_title=t_title;}

  /**
  * @brief check if the user take part in this meeting
  * @param t_username the source username
  * @return if the user take part in this meeting
  */
  bool Meeting::isParticipator(const string &t_username) const{
    for(auto i=m_participators.begin();i!=m_participators.end();i++){
      if(*i==t_username)
        return true;
    }
    return false;
  }

