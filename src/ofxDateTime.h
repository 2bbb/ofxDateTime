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
    
    inline ofxDateTime &setTimezone(float timezone_diff_hour) {
        formatted.reset();
        timezone_diff_in_hour = timezone_diff_hour;
        return *this;
    }
    
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
    std::string getFormattedString(std::string format) const;
    
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
    
    int year() const;
    uint8_t month() const;
    uint8_t day() const;
    uint8_t hour() const;
    uint8_t minute() const;
    uint8_t second() const;
    uint16_t millisecond() const;
    uint16_t microsecond() const;
    
    inline float getTimezoneDifference() const { return timezone_diff_in_hour; }
    
    inline ofxDateTime &year(int y) {
        return set(y, month(), day(), hour(), minute(), second(), millisecond(), microsecond());
    }
    inline ofxDateTime &month(int m) {
        return set(year(), m, day(), hour(), minute(), second(), millisecond(), microsecond());
    }
    inline ofxDateTime &day(int d) {
        return set(year(), month(), d, hour(), minute(), second(), millisecond(), microsecond());
    }
    inline ofxDateTime &hour(int h) {
        return set(year(), month(), day(), h, minute(), second(), millisecond(), microsecond());
    }
    inline ofxDateTime &minute(int m) {
        return set(year(), month(), day(), hour(), m, second(), millisecond(), microsecond());
    }
    inline ofxDateTime &second(int s) {
        return set(year(), month(), day(), hour(), minute(), s, millisecond(), microsecond());
    }
    inline ofxDateTime &millisecond(uint16_t msec) {
        return set(year(), month(), day(), hour(), minute(), second(), msec, microsecond());
    }
    inline ofxDateTime &microsecond(uint16_t usec) {
        return set(year(), month(), day(), hour(), minute(), second(), millisecond(), usec);
    }
    
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

int64_t operator ""_sec(unsigned long long sec) {
    return sec;
}

int64_t operator ""_min(unsigned long long min) {
    return 60 * min;
}

int64_t operator ""_hour(unsigned long long hour) {
    return 60 * 60 * hour;
}

int64_t operator ""_day(unsigned long long day) {
    return 24 * 60 * 60 * day;
}

int64_t operator ""_week(unsigned long long week) {
    return 7 * 24 * 60 * 60 * week;
}

#endif /* ofxDateTime_h */
