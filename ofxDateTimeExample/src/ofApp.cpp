#include "ofMain.h"
#include "ofxDateTime.h"
#include <cassert>

class ofApp : public ofBaseApp {
public:
    void setup() {
        // create from year, month, day with your timezone
        ofxDateTime def = ofxDateTime({2016, 6, 15});
        ofLogNotice("default") << def;
        
        // create from year, month, day with specified timezone
        ofxDateTime utc = ofxDateTime({2016, 6, 15}, 0);
        ofLogNotice("UTC") << utc;
        ofxDateTime jst = ofxDateTime({2016, 6, 15}, 9);
        ofLogNotice("JST") << jst;
        
        // can get unix timestamp
        ofLogNotice("UTC timestamp") << utc.getUnixTime();
        ofLogNotice("JST timestamp") << jst.getUnixTime();
        
        ofLogNotice("difference between utc and jst") << utc - jst;
        assert(utc - jst == 9 * 60 * 60);
        
        // create from timestamp
        ofLogNotice("UTC timestamp to UTC") << ofxDateTime(utc.getUnixTime(), 0);
        ofLogNotice("UTC timestamp to JST") << ofxDateTime(utc.getUnixTime(), 9);
        ofLogNotice("JST timestamp to UTC") << ofxDateTime(jst.getUnixTime(), 0);
        ofLogNotice("JST timestamp to JST") << ofxDateTime(jst.getUnixTime(), 9);
        
        // set specified component
        ofLogNotice("setter") << utc.year(2001);
        // and can method chain
        ofLogNotice("setter") << utc.month(2).day(3).hour(4).minute(5).second(6).millisecond(7).microsecond(8);
        
        // default value is now
        ofxDateTime datetime;
        ofLogNotice("default") << datetime;
        
        // `set` keeps timezone
        ofLogNotice("set") << datetime.set({2011, 12, 13, 14, 15, 16, 17});
        // or set manually
        ofLogNotice("set") << datetime.set({2011, 12, 13, 14, 15, 16, 17}, 9);
        // if timezone difference +0830 then gives 8.5
        ofLogNotice("set") << datetime.set({2011, 12, 13, 14, 15, 16, 17}, 8.5);
        
        // change timezone
        datetime.setTimezone(7);
        ofLogNotice("setTimezone") << datetime;
        
        // get next day
        auto tomorrow = datetime.tomorrow();
        ofLogNotice("tommorow") << tomorrow;
        // go next day
        ofLogNotice("goNextDay") << datetime.goNextDay();
        assert(datetime == tomorrow);
        
        // get datetime string
        ofLogNotice("simple") << datetime.getDateTimeString();
        ofLogNotice("detailed") << datetime.getDateTimeString(true);
        ofLogNotice("ISO8601") << datetime.getISO8601FormatString();
        ofLogNotice("ISO8601 ext") << datetime.getISO8601FormatString(true);
        ofLogNotice("RFC1123") << datetime.getRFCFormatString();
        ofLogNotice("custom(%e.%n.%y)") << datetime.getFormattedString("%e.%n.%y");
        ofLogNotice("custom(%Y/%m/%d)") << datetime.getFormattedString("%Y/%m/%d");
        
        // can convert to std::string. is same to getDateTimeString()
        std::string str = datetime;
        assert(str == datetime.getDateTimeString());
        
        // can convert to uint64_t. is same to getUnixTime()
        uint64_t ts = datetime;
        assert(ts == datetime.getUnixTime());
        
        datetime.set(2000, 1, 1, 0, 0, 0);
        ofLogNotice("custom literal") << (datetime + 4_day + 4_hour + 4_min + 4_sec);
    }
};

int main() {
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp);
}
