//
//  ofxDateTime.h
//
//  Created by ISHII 2bit on 2016/06/06.
//
//

#pragma once

#ifndef ofxDateTime_h
#define ofxDateTime_h

#include <iostream>
#include <string>
#include <cstddef>
#include <memory>

namespace Poco {
    class Timestamp;
    class DateTime;
}

struct ofxDateTime {
    struct DateTimeStruct {
        DateTimeStruct(int year, int month, int day,
                       int hour = 0, int minute = 0, int second = 0,
                       int millisecond = 0, int microsecond = 0)
        : year(year), month(month), day(day)
        , hour(hour), minute(minute), second(second)
        , millisecond(millisecond), microsecond(microsecond) {}
        
        DateTimeStruct()
        : DateTimeStruct(1970, 1, 1,
                   0, 0, 0,
                   0, 0) {}
        
        DateTimeStruct(const DateTimeStruct &) = default;
        DateTimeStruct(DateTimeStruct &&) = default;
        
        DateTimeStruct &operator=(const DateTimeStruct &) = default;
        DateTimeStruct &operator=(DateTimeStruct &&) = default;
        
        int year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint16_t millisecond;
        uint16_t microsecond;
    };
    
    ofxDateTime();
    ofxDateTime(const DateTimeStruct &dt);
    ofxDateTime(const DateTimeStruct &dt, float timezone_diff_hour);
    ofxDateTime(uint64_t unix_time_sec);
    ofxDateTime(uint64_t unix_time_sec, float timezone_diff_hour);
    
    ofxDateTime(const ofxDateTime &datetime);
    ofxDateTime(const ofxDateTime &datetime, float timezone_diff_hour);
    ofxDateTime(ofxDateTime &&datetime);
    ofxDateTime(ofxDateTime &&datetime, float timezone_diff_hour);
    
    ofxDateTime &set(const DateTimeStruct &dt);
    ofxDateTime &set(const DateTimeStruct &dt, float timezone_diff_hour);
    ofxDateTime &set(int year, int month, int day,
                     int hour = 0, int minute = 0, int second = 0,
                     int millisecond = 0, int microsecond = 0);
    ofxDateTime &set(uint64_t unix_time);
    ofxDateTime &set(uint64_t unix_time, float timezone_diff_hour);
    
    ofxDateTime &operator=(const ofxDateTime &datetime);
    ofxDateTime &operator=(ofxDateTime &&datetime);
    ofxDateTime &operator=(uint64_t unix_time);
    
    bool operator==(const ofxDateTime &datetime) const;
    bool operator!=(const ofxDateTime &datetime) const;
    bool operator<(const ofxDateTime &datetime) const;
    bool operator<=(const ofxDateTime &datetime) const;
    bool operator>(const ofxDateTime &datetime) const;
    bool operator>=(const ofxDateTime &datetime) const;
    
    inline operator std::string() const { return getDateTimeString(); }
    inline std::string getDateTimeString(bool in_detail = false) const {
        return in_detail
            ? getFormattedString("%Y/%m/%d %H:%M:%s %Z")
            : getFormattedString("%Y/%m/%d %H:%M:%S");
    }
    inline std::string getISO8601FormatString(bool isExtended = false) const {
        if(isExtended) {
            return getFormattedString("%Y-%m-%d-T%H:%M:%S%z");
        } else {
            std::string str = getFormattedString("%Y%m%dT%H%M%S%z");
            return str.replace(str.length() - 3, 1, "");
        }
    }
    inline std::string getRFCFormatString() const {
        return getFormattedString("%w, %d %b %Y %H:%M:%S %Z");
    }
    std::string getFormattedString(const std::string &format) const;
    
    // returns unix timestamp
    operator uint64_t() const;
    inline uint64_t getUnixTime() const {
        return this->operator uint64_t();
    }
    
    inline ofxDateTime operator+(int64_t diff) const { return ofxDateTime(*this) += diff; }
    ofxDateTime &operator+=(int64_t diff);
    
    inline ofxDateTime operator-(int64_t diff) const { return ofxDateTime(*this) -= diff; }
    ofxDateTime &operator-=(int64_t diff);
    
    inline int64_t operator-(const ofxDateTime &from_date) const { return (operator uint64_t() - from_date.operator uint64_t()); }
    inline int64_t timeIntervalSince(const ofxDateTime &from_date) const { return *this - from_date; }
    
    uint32_t year() const;
    inline uint32_t Y() const { return year(); }
    uint8_t month() const;
    inline uint8_t M() const { return month(); }
    uint8_t day() const;
    inline uint8_t D() const { return day(); }
    uint8_t hour() const;
    inline uint8_t h() const { return hour(); }
    uint8_t minute() const;
    inline uint8_t m() const { return minute(); }
    uint8_t second() const;
    inline uint8_t s() const { return second(); }
    uint16_t millisecond() const;
    inline uint16_t ms() const { return millisecond(); }
    uint16_t microsecond() const;
    inline uint16_t us() const { return microsecond(); }
    
    inline float timezoneDifference() const { return timezone_diff_in_hour; }
    inline float tzd() const { return timezone_diff_in_hour; }
    
    inline ofxDateTime &Y(int year) { return set(year, M(), D(), h(), m(), s(), ms(), us()); }
    inline ofxDateTime &year(int year) { return Y(year); }
    inline ofxDateTime &M(int month) { return set(Y(), month, D(), h(), m(), s(), ms(), us()); }
    inline ofxDateTime &month(int month) { return M(month); }
    inline ofxDateTime &D(int day) { return set(Y(), M(), day, h(), m(), s(), ms(), us()); }
    inline ofxDateTime &day(int day) { return D(day); }
    inline ofxDateTime &h(int hour) { return set(Y(), M(), D(), hour, m(), s(), ms(), us()); }
    inline ofxDateTime &hour(int hour) { return h(hour); }
    inline ofxDateTime &m(int min) { return set(Y(), M(), D(), h(), min, s(), ms(), us()); }
    inline ofxDateTime &minute(int min) { return m(min); }
    inline ofxDateTime &s(int sec) { return set(Y(), M(), D(), h(), m(), sec, ms(), us()); }
    inline ofxDateTime &second(int sec) { return s(sec); }
    inline ofxDateTime &ms(uint16_t ms_) { return set(Y(), M(), D(), h(), m(), s(), ms_, us()); }
    inline ofxDateTime &millisecond(uint16_t ms_) { ms(ms_); }
    inline ofxDateTime &us(uint16_t us_) { return set(Y(), M(), D(), h(), m(), s(), ms(), us_); }
    inline ofxDateTime &microsecond(uint16_t us_) { return us(us_); }
    
    inline ofxDateTime &timezoneDifference(float timezone_diff_hour) {
        formatted.reset();
        timezone_diff_in_hour = timezone_diff_hour;
        return *this;
    }
    inline ofxDateTime &tzd(float timezone_diff_hour) { return timezoneDifference(timezone_diff_hour); }

    ofxDateTime &setToNow();
    
    inline ofxDateTime yesterday() const { return this->operator-(24 * 60 * 60); }
    inline ofxDateTime &goPrevDay() { return this->operator-=(24 * 60 * 60); }
    inline ofxDateTime &goPrevWeek() { return this->operator-=(7 * 24 * 60 * 60); }
    
    inline ofxDateTime tomorrow() const { return this->operator+(24 * 60 * 60); }
    inline ofxDateTime &goNextDay() { return this->operator+=(24 * 60 * 60); }
    inline ofxDateTime &goNextWeek() { return this->operator+=(7 * 24 * 60 * 60); }
    
    inline bool isPastFromNow() const { return *this - ofxDateTime() < 0L; }
    inline bool isPastFrom(const ofxDateTime &datetime) const { return *this - datetime < 0L; }
    
private:
    void update();
    std::shared_ptr<Poco::Timestamp> body;
    mutable std::shared_ptr<Poco::DateTime> formatted;
    Poco::DateTime &getDateTime() const;
    float timezone_diff_in_hour;
};

std::ostream &operator<<(std::ostream &os, const ofxDateTime &dt);

int64_t operator ""_sec(unsigned long long sec);
int64_t operator ""_min(unsigned long long min);
int64_t operator ""_hour(unsigned long long hour);
int64_t operator ""_day(unsigned long long day);
int64_t operator ""_week(unsigned long long week);

#endif /* ofxDateTime_h */
