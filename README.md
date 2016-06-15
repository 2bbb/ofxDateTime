# ofxDateTime

utility for datetime and unix time

## How to use

```cpp
	ofxDateTime jst = ofxDateTime({2016, 6, 6}, 9); // 2016/06/06 00:00:00 +09:00
	ofxDateTime utc = ofxDateTime({2016, 6, 6}, 0); // 2016/06/06 00:00:00 +00:00
	ofxDateTime ytz = ofxDateTime({2016, 6, 6}); // 2016/06/06 00:00:00 + your timezone diff
	
	ofxDateTime jst_from_timestamp = ofxDateTime(1465138800, 9);
	jst_from_timestamp.getDateTimeString(); // 2016/06/06 00:00:00
	ofxDateTime utc_from_timestamp = ofxDateTime(1465138800, 0);
	utc_from_timestamp.getDateTimeString(); // 2016/06/05 15:00:00
	
	jst.getFormattedString("%e.%n.%y"); // 6.6.16
	jst.getFormattedString("%Y/%m/%d"); // 2016.6.6
	
	jst.year(2015); // change to 2015/06/06 00:00:00 +09:00
```

## API

TODO

## Update History

### 2016/06/15 ver 0.01 release

* initial

## License

MIT License.

## Author

* ISHII 2bit [bufferRenaiss co., ltd.]
* ishii[at]buffer-renaiss.com
