#include <initializer_list>
#include <string>
#include <sstream>
#include <stdio.h>
#include <cctype>
#include "Date.hpp"
using namespace std;

  /**
  * @brief default constructor
  */
  Date::Date():m_year(0),m_month(0),m_day(0),m_hour(0),m_minute(0){}
  /**
  * @brief constructor with arguments
  */
  Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute):m_year(t_year),m_month(t_month),m_day(t_day),m_hour(t_hour),m_minute(t_minute){}

  /**
  * @brief constructor with a string
    2016-08-01/12:00
  */
  Date::Date(const string &dateString)
  {
    // string s=dateString;
    // s[4]=' ';
    // s[7]=' ';
    // s[10]=' ';
    // s[13]=' ';
    // stringstream ss;
    // ss<<s;
    // ss>>m_year>>m_month>>m_day>>m_hour>>m_month;
    // const char* str=dateString.c_str();
    // sscanf(str,"%d-%d-%d/%d:%d",&m_year,&m_month,&m_day,&m_hour,&m_minute);
    *this=Date::stringToDate(dateString);
  }
  /**
  * @brief return the year of a Date
  * @return   a integer indicate the year of a date
  */
  int Date::getYear(void) const{return m_year;}

  /**
  * @brief set the year of a date
  * @param a integer indicate the new year of a date
  */
  void Date::setYear(const int t_year){m_year=t_year;}

  /**
  * @brief return the month of a Date
  * @return   a integer indicate the month of a date
  */
  int Date::getMonth(void) const{return m_month;}

  /**
  * @brief set the month of a date
  * @param a integer indicate the new month of a date
  */
  void Date::setMonth(const int t_month){m_month=t_month;}

  /**
  * @brief return the day of a Date
  * @return   a integer indicate the day of a date
  */
  int Date::getDay(void) const{return m_day;}

  /**
  * @brief set the day of a date
  * @param a integer indicate the new day of a date
  */
  void Date::setDay(const int t_day){m_day=t_day;}

  /**
  * @brief return the hour of a Date
  * @return   a integer indicate the hour of a date
  */
  int Date::getHour(void) const{return m_hour;}

  /**
  * @brief set the hour of a date
  * @param a integer indicate the new hour of a date
  */
  void Date::setHour(const int t_hour){m_hour=t_hour;}

  /**
  * @brief return the minute of a Date
  * @return   a integer indicate the minute of a date
  */
  int Date::getMinute(void) const{return m_minute;}

  /**
  * @brief set the minute of a date
  * @param a integer indicate the new minute of a date
  */
  void Date::setMinute(const int t_minute){m_minute=t_minute;}

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  */
  bool Date::isValid(const Date &t_date)
  {
    if(t_date.m_year<1000||t_date.m_year>9999||t_date.m_month<=0||t_date.m_month>12||t_date.m_day<=0||t_date.m_day>31||t_date.m_hour<0||t_date.m_hour>=24||t_date.m_minute<0||t_date.m_minute>=60)
      return false;
    if(t_date.m_month==2)
    {
      if((t_date.m_year%4==0&&t_date.m_year%100!=0)||t_date.m_year%400==0)
      {
        if(t_date.m_day>=30)
            return false;
      }
      else 
      {
        if(t_date.m_day>=29)
          return false;
      }
    }
    else if(t_date.m_month==4||t_date.m_month==6||t_date.m_month==9||t_date.m_month==11)
    {
      if(t_date.m_day>30)
        return false;
    }
    return true;
  }

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
  Date Date::stringToDate(const string &str)
  {
    Date date;
    // const char* str=t_dateString.c_str();
    int year=0,month=0,day=0,hour=0,minute=0;
    if(str.size()!=16)
      return date;
    if(str[4]!='-'||str[7]!='-'||str[10]!='/'||str[13]!=':')
      return date;
    for(int j=0;j<16;j++)
    {
      if(j!=4&&j!=7&&j!=10&&j!=13)
      {
        if(!isdigit(str[j]))
          return date;
      }
    }
    string s=str;
    s[4]=' ';
    s[7]=' ';
    s[10]=' ';
    s[13]=' ';
    stringstream ss;
    ss<<s;
    ss>>year>>month>>day>>hour>>minute;
    date.setYear(year);
    date.setMonth(month);
    date.setDay(day);
    date.setHour(hour);
    date.setMinute(minute);
    return date;
  }
  /**
  * @brief convert a date to string, if the format is not correct return
  * 0000-00-00/00:00
  */
  string Date::dateToString(const Date &t_date)
  {
    char str[30];
    string s,s1,s2,s3,s4;
    if(!isValid(t_date))
      return "0000-00-00/00:00";
    
    if(t_date.m_month<10)
      sprintf(str,"%d-0%d",t_date.m_year,t_date.m_month);
    else
      sprintf(str,"%d-%d",t_date.m_year,t_date.m_month);
    s1=str;
    if(t_date.m_day<10)
      sprintf(str,"-0%d",t_date.m_day);
    else
      sprintf(str,"-%d",t_date.m_day);
    s2=str;
    if(t_date.m_hour<10)
      sprintf(str,"/0%d:",t_date.m_hour);
    else
      sprintf(str,"/%d:",t_date.m_hour);
    s3=str;
    if(t_date.m_minute<10)
      sprintf(str,"0%d",t_date.m_minute);
    else
      sprintf(str,"%d",t_date.m_minute);
    s4=str;
    s=s1+s2+s3+s4;
    return s;
  }

  /**
  *  @brief overload the assign operator
  */
  Date& Date::operator=(const Date &t_date)
  {
    m_year=t_date.m_year;
    m_month=t_date.m_month;
    m_day=t_date.m_day;
    m_hour=t_date.m_hour;
    m_minute=t_date.m_minute;
    return *this;
  }

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
  bool Date::operator==(const Date &t_date) const
  {
    if(m_year==t_date.m_year)
    {
      if(m_month==t_date.m_month)
      {
        if(m_day==t_date.m_day)
        {
          if(m_hour==t_date.m_hour)
          {
            if(m_minute==t_date.m_minute)
              return true;
          }
        }
      }
    }
    return false;
  }

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
  bool Date::operator>(const Date &t_date) const
  {
    if(m_year>t_date.m_year)
      return true;
    if(m_year==t_date.m_year)
    {
      if(m_month>t_date.m_month)
        return true;
      if(m_month==t_date.m_month)
      {
        if(m_day>t_date.m_day)
          return true;
        if(m_day==t_date.m_day)
        {
          if(m_hour>t_date.m_hour)
            return true;
          if(m_hour==t_date.m_hour)
          {
            if(m_minute>t_date.m_minute)
              return true;
          }
        }
      }
    }
    return false;
  }

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
  bool Date::operator<(const Date &t_date) const
  {
    if(m_year<t_date.m_year)
      return true;
    if(m_year==t_date.m_year)
    {
      if(m_month<t_date.m_month)
        return true;
      if(m_month==t_date.m_month)
      {
        if(m_day<t_date.m_day)
          return true;
        if(m_day==t_date.m_day)
        {
          if(m_hour<t_date.m_hour)
            return true;
          if(m_hour==t_date.m_hour)
          {
            if(m_minute<t_date.m_minute)
              return true;
          }
        }
      }
    }
    return false;
  }

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
  bool Date::operator>=(const Date &t_date) const
  {
    if(*this==t_date||*this>t_date)
      return true;
    return false;
  }

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool Date::operator<=(const Date &t_date) const
  {
    if(*this==t_date||*this<t_date)
      return true;
    return false;
  }
