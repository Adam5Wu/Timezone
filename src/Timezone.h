/*----------------------------------------------------------------------*
 * Arduino Timezone Library                                             *
 * Jack Christensen Mar 2012                                            *
 *                                                                      *
 * Arduino Timezone Library Copyright (C) 2018 by Jack Christensen and  *
 * licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html   *
 *----------------------------------------------------------------------*/ 

#ifndef TIMEZONE_H_INCLUDED
#define TIMEZONE_H_INCLUDED
#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif
#include <TimeLib.h>    // https://github.com/PaulStoffregen/Time

// convenient constants for TimeChangeRules
enum week_t {Last=0, First, Second, Third, Fourth}; 
enum dow_t {Sun=0, Mon, Tue, Wed, Thu, Fri, Sat};
enum month_t {Jan=0, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

// structure to describe rules for when daylight/summer time begins,
// or when standard time begins.
struct TimeChangeRule
{
    char abbrev[6];    // five chars max
    uint8_t week;      // First, Second, Third, Fourth, or Last week of the month
    uint8_t dow;       // day of week, 0=Sun, 1=Mon, ... 6=Sat
    uint8_t month;     // 0=Jan, 1=Feb, ... 11=Dec
    uint8_t hour;      // 0-23
    int offset;        // offset from UTC in minutes
};
        
class Timezone
{
    public:
        static TimeChangeRule UTC;
        Timezone(TimeChangeRule const &dstStart = UTC,
            TimeChangeRule const &stdStart = UTC);
        Timezone(int address);
        time_t toLocal(time_t utc);
        time_t toLocal(time_t utc, TimeChangeRule **tcr);
        time_t toUTC(time_t local);
        bool utcIsDST(time_t utc);
        bool locIsDST(time_t local);
        void setRules(TimeChangeRule const &dstStart, TimeChangeRule const &stdStart);
        void getRules(TimeChangeRule &dstStart, TimeChangeRule &stdStart);
        void readRules(int address);
        void writeRules(int address);

    private:
        void calcTimeChanges(int yr);
        time_t toTime_t(TimeChangeRule const &r, int yr);
        TimeChangeRule m_dst;   // rule for start of dst or summer time for any year
        TimeChangeRule m_std;   // rule for start of standard time for any year
        time_t m_dstUTC;        // dst start for given/current year, given in UTC
        time_t m_stdUTC;        // std time start for given/current year, given in UTC
        time_t m_dstLoc;        // dst start for given/current year, given in local time
        time_t m_stdLoc;        // std time start for given/current year, given in local time
};
#endif
