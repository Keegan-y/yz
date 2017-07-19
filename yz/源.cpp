#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <deque>
#include <algorithm>
#include <iterator>
#include "Record.h"
using namespace std;
using CodeTest::RecordFields;


vector<int> getEndPos(const string& line) {
	//size of line is checked when processing begin
	//so is line[0] != ','. of course line[0] is not valid input which means input file is broken.
	assert(line[0] != ',');
	vector<int> endPos;
	int len = line.size();
	for (int i = 1; i < len; i++) {
		if (line[i] == ',') {
			endPos.push_back(i);
		}
	}
	endPos.push_back(line.size());
	return endPos;
}

//since not every field is needed, I won't parse the whole string to one object
string getField(const vector<int>& endPos, int index, const string& line) {
	int len = endPos.size();
	assert((index >= 0 && index < len));
	if (index == 0) {
		return line.substr(0, endPos[0]);
	}

	return line.substr(endPos[index - 1] + 1, endPos[index] - endPos[index - 1] - 1);
}

string getTimeStampStr(const string& s) {
	//field 0, 20, 21
	//20161205  20:59:00 500
	//format to "20161205  20:59:00.500"

	auto pos = s.find(',');
	string day = s.substr(0, pos);
	string time;
	string msec;

	for (int cnt = 1; cnt < 22; cnt++) {
		auto newpos = s.find(',', pos + 1);
		assert(newpos != string::npos);
		if (cnt == RecordFields::LASTMODIFYTIME) {
			time = s.substr(pos + 1, newpos - pos - 1);
		}
		else if (cnt == RecordFields::LASTMODIFYMILS) {
			msec = s.substr(pos + 1, newpos - pos - 1);
		}
		pos = newpos;
	}

	return day + " " + time + "." + msec;
}

int matchAndExtract(const string& s, int index, int lenMax, int factor) {
	int number = 0;
	int upper = s.size();
	for (int i = index; i < index + lenMax && i < upper; i++) {
		number *= 10;
		number += s[i] - '0';
	}
	return  number * factor;
}

int strToMili(const string& s) {
	//the format is :
	//20161205 xx:xx:xx.xxx 21
	//20161205 xx:xx:xx.x   19
	int len = s.size();
	if (len > 21 && len < 19) { cerr << "strToMilis input string length error. input is " << s << endl; exit(-1); }
	int result = 0;
	int index = 9;
	//match hour:
	result += matchAndExtract(s, index, 2, 60 * 60 * 1000);
	index += 2;
	if (s[index++] != ':') { cerr << "strToMilis missing first ':' " << endl; exit(-1); }
	//match minute;
	result += matchAndExtract(s, index, 2, 60 * 1000);
	index += 2;
	if (s[index++] != ':') { cerr << "strToMilis missing second ':' " << endl; exit(-1); }
	//match second:
	result += matchAndExtract(s, index, 2, 1000);
	index += 2;
	if (s[index++] != '.') { cerr << "strToMilis missing '.' before millionseconds" << endl; exit(-1); }
	//match milis
	result += matchAndExtract(s, index, 3, 1);;
	return  result;
}

int timeDiff(const string& prev, const string& cur) {
	int p = strToMili(prev);
	int c = strToMili(cur);
	return c - p;
}

vector<string> readLinesFromFile(ifstream& ifs) {
	vector<string> result;
	//ignore the first line
	string line;
	getline(ifs, line);
	while (getline(ifs, line)) {
		result.push_back(line);
	}
	return result;
}

void sortToTimeStampAscending(vector<string>& records) {
	sort(records.begin(), records.end()
		, [&](const string& s1, const string& s2) {
		string timestamp1 = getTimeStampStr(s1);
		string timestamp2 = getTimeStampStr(s2);
		return timestamp1 < timestamp2;
	}
	);
}

vector<string> preprocess(ifstream& ifs) {
	auto records = readLinesFromFile(ifs);
	cout << "read file into memory" << endl;
	sortToTimeStampAscending(records);
	cout << "sort done" << endl;
	return records;
}

void checkRecord(const string& line) {
	//some check on string formats
	if (line.empty()) {
		cerr << "read empty line" << endl;
		exit(-1);
	}
	if (line[0] == ',' || *line.rbegin() == ',') {
		cerr << "invalid data : (begin or end with comma)" << line << endl;
		exit(-1);
	}
	//checking ends here
}

void removeOldRecords(const string& cur, int diffMilis, deque<pair<string, double>>& d) {
	while (!d.empty()) {
		string& ts = d.front().first;
		if (timeDiff(ts, cur) > diffMilis) {
			d.pop_front();
		}
		else {
			break;
		}
	}
}
int processing(const string& inputFile, const string& outputFile) {
	ifstream ifs(inputFile);
	if (ifs.fail()) {
		cerr << strerror(errno) << endl;
		return -1;
	}
	ofstream ofs(outputFile);
	if (ofs.fail()) {
		cerr << strerror(errno) << endl;
		return -1;
	}

	vector<string> timeIncreasingRecords = preprocess(ifs);

	//<timestamp, askprice - bidprice> of recent 5s
	deque<pair<string, double>> acc;
	//probably one optimization to save diff of previous 5s, do it later
	//askprice - bidprice accumualation 
	//double accSum = 0;

	for (auto line : timeIncreasingRecords) {
		checkRecord(line);

		//this is for retrieving fields easily
		vector<int> endPos = getEndPos(line);

		//time stamp for now:
		string timeStamp = getTimeStampStr(line);

		//compute rb_bv_diff:
		{
			string bidVol1 = getField(endPos, RecordFields::BIDVOL1, line);
			string askVol1 = getField(endPos, RecordFields::ASKVOL1, line);
			int bv1 = stoi(bidVol1);
			int av1 = stoi(askVol1);
			int result = bv1 - av1;

			ofs << timeStamp << " rb_bv_DIFF " << result << endl;
		}
		//compute rb_bo_5_AVG:
		{
			string askPrice1 = getField(endPos, RecordFields::ASKPRICE1, line);
			string bidPrice1 = getField(endPos, RecordFields::BIDPRICE1, line);
			double ap1 = stod(askPrice1);
			double bp1 = stod(bidPrice1);
			double diff = ap1 - bp1;

			//if any record is not within the recent 5s, remove it
			removeOldRecords(timeStamp, 5000, acc);

			//caculate the aver
			double aver = 0;
			for (auto& item : acc) {
				aver += item.second;
			}

			double result = diff;
			if (aver != 0) {
				result += aver / acc.size();
			}
			
			acc.push_back(make_pair(timeStamp, diff));
			ofs << timeStamp << " rb_bo_5_AVG " << result << endl;
		}
	}

	ofs.close();

	return 0;
}

int main() {
	auto ret = processing("ru1705_20161205.csv", "output.txt");
	cout << "processing finished" << endl;
	return ret;
}