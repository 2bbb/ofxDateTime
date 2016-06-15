//
//  ofxDateTime.cpp
//
//  Created by ISHII 2bit on 2016/06/06.
//
//

#include "ofxDateTime.h"

#include <Poco/Timestamp.h>
#include <Poco/DateTime.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timezone.h>

#include <Poco/Format.h>
#include <Poco/DateTimeFormatter.h>

#include "ofUtils.h"

#pragma mark constructor

ofxDateTime::ofxDateTime()
: body(new Poco::Timestamp())
, formatted()
, timezone_diff_in_hour(0.0f) {}

ofxDateTime::ofxDateTime(const DateTimeStruct &dt)
: ofxDateTime(dt, Poco::Timezone::tzd() / 3600.0) {}

ofxDateTime::ofxDateTime(const DateTimeStruct &dt, float timezone_diff_hour)
: body(new Poco::Timestamp())
, formatted()
, timezone_diff_in_hour(timezone_diff_hour) {
    Poco::DateTime date(dt.year, dt.month, dt.day,
                        dt.hour, dt.minute, dt.second,
                        dt.millisecond, dt.microsecond);

    date -= Poco::Timespan(timezone_diff_in_hour * 3600, 0);
    *body = date.timestamp();
}

ofxDateTime::ofxDateTime(uint64_t unix_time)
: ofxDateTime(unix_time, Poco::Timezone::tzd() / 3600.0) {}

ofxDateTime::ofxDateTime(uint64_t unix_time, float tzd)
: body(new Poco::Timestamp(Poco::Timestamp::fromEpochTime(unix_time)))
, formatted()
, timezone_diff_in_hour(tzd) {}

ofxDateTime::ofxDateTime(const ofxDateTime &datetime)
: ofxDateTime(datetime, Poco::Timezone::tzd() / 3600.0) {}

ofxDateTime::ofxDateTime(const ofxDateTime &datetime, float timezone_diff_hour)
: body(new Poco::Timestamp(*(datetime.body)))
, formatted()
, timezone_diff_in_hour(datetime.timezone_diff_in_hour) {}

ofxDateTime::ofxDateTime(ofxDateTime &&datetime)
: ofxDateTime(std::move(datetime), Poco::Timezone::tzd() / 3600.0) {}

ofxDateTime::ofxDateTime(ofxDateTime &&datetime, float timezone_diff_hour)
: body(std::move(datetime.body))
, formatted()
, timezone_diff_in_hour(timezone_diff_in_hour) {}

#pragma mark set

ofxDateTime &ofxDateTime::set(const DateTimeStruct &dt) {
    return *this = std::move(ofxDateTime(dt, timezone_diff_in_hour));
}

ofxDateTime &ofxDateTime::set(const DateTimeStruct &dt, float timezone_diff_hour) {
    return *this = std::move(ofxDateTime(dt, timezone_diff_hour));
}

ofxDateTime &ofxDateTime::set(int year, int month, int day,
                              int hour, int minute, int second,
                              int millisecond, int microsecond)
{
    return *this = std::move(ofxDateTime({
        year, month, day,
        hour, minute, second,
        millisecond, microsecond
    }, timezone_diff_in_hour));
}

ofxDateTime &ofxDateTime::set(uint64_t unix_time) {
    return *this = std::move(ofxDateTime(unix_time, timezone_diff_in_hour));
}

ofxDateTime &ofxDateTime::set(uint64_t unix_time, float tzd) {
    return *this = std::move(ofxDateTime(unix_time, tzd));
}

ofxDateTime &ofxDateTime::operator=(const ofxDateTime &datetime) {
    return *this = std::move(ofxDateTime(datetime));
}

ofxDateTime &ofxDateTime::operator=(ofxDateTime &&datetime) {
    body = std::move(datetime.body);
    formatted.reset();
    timezone_diff_in_hour = datetime.timezone_diff_in_hour;
    
    return *this;
}

ofxDateTime &ofxDateTime::operator=(uint64_t unix_time) {
    return set(unix_time);
}

#pragma mark comparator

bool ofxDateTime::operator==(const ofxDateTime &dt) const { return *body == *(dt.body); }
bool ofxDateTime::operator!=(const ofxDateTime &dt) const { return *body != *(dt.body); }
bool ofxDateTime::operator< (const ofxDateTime &dt) const { return *body <  *(dt.body); }
bool ofxDateTime::operator<=(const ofxDateTime &dt) const { return *body <= *(dt.body); }
bool ofxDateTime::operator> (const ofxDateTime &dt) const { return *body >  *(dt.body); }
bool ofxDateTime::operator>=(const ofxDateTime &dt) const { return *body >= *(dt.body); }

#pragma mark cast

std::string ofxDateTime::getFormattedString(const std::string &format) const {
    return Poco::DateTimeFormatter::format(getDateTime(), format, static_cast<int>(3600 * timezone_diff_in_hour));
}

Poco::DateTime &ofxDateTime::getDateTime() const {
    if(!formatted) {
        formatted = std::make_shared<Poco::DateTime>(*body);;
        formatted->makeLocal(timezone_diff_in_hour * 3600.0);
    }
    return *formatted;
}

ofxDateTime::operator uint64_t() const {
    return body->epochTime();
}

#pragma mark add/sub

ofxDateTime &ofxDateTime::operator+=(int64_t diff) {
    *body += Poco::Timespan(diff, 0);
    formatted.reset();
    return *this;
}

ofxDateTime &ofxDateTime::operator-=(int64_t diff) {
    *body -= Poco::Timespan(diff, 0);
    formatted.reset();
    return *this;
}

#pragma mark getter / setter

uint32_t ofxDateTime::year() const { return getDateTime().year(); }
uint8_t ofxDateTime::month() const { return getDateTime().month(); }
uint8_t ofxDateTime::day() const { return getDateTime().day(); }
uint8_t ofxDateTime::hour() const { return getDateTime().hour(); }
uint8_t ofxDateTime::minute() const { return getDateTime().minute(); }
uint8_t ofxDateTime::second() const { return getDateTime().second(); }
uint16_t ofxDateTime::millisecond() const { return getDateTime().millisecond(); }
uint16_t ofxDateTime::microsecond() const { return getDateTime().microsecond(); }

ofxDateTime &ofxDateTime::setToNow() {
    body->update();
    return *this;
}

std::ostream &operator<<(std::ostream &os, const ofxDateTime &dt) {
    return os << dt.getDateTimeString(true) << " [" << dt.getUnixTime() << "]";
}
