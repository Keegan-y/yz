#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <iostream>
//#include <dirent>
#include "Helper.h"
#include "ComputeBO.h"
#include "ComputeBV.h"
using namespace std;

namespace SignalCaculator {
	string getContractHead(const string& str) {
		auto pos = str.find('_');
		if (pos == string::npos) {
			cerr << "invalid signal format. signal : " << str << endl;
			exit(-1);
		}

		return str.substr(0, pos);
	}

	//this is platform dependent
	vector<string> listDir(const string& dir) {
		//dirp = opendir(dir.c_str());
		//while ((dp = readdir(dirp)) != NULL)
		//	if (dp->d_namlen == len && !strcmp(dp->d_name, name)) {
		//		(void)closedir(dirp);
		//		return FOUND;
		//	}
		//(void)closedir(dirp);
		//return NOT_FOUND;
		return vector<string>{"ru1705_20161205.csv", "rb1705_20161205.csv", "cu1703_20161205.csv", "al1702_20161205.csv", "al1703_20161205.csv"};
	}

	void parseAndAddFunction(const string& signal, ComputeSignal& cs) {
		vector<string> fields;

		size_t pos = 0;
		
		while (pos < signal.size()) {
			size_t newPos = signal.find('_', pos);
			string tmp;
			if (newPos != string::npos) {
				tmp = signal.substr(pos, newPos - pos);
				pos = newPos + 1;
			}
			else {
				tmp = signal.substr(pos);
				pos = signal.size();
			}

			fields.push_back(tmp);
		}
		//typical signal string: ru_bo_10_SHIFT al_bv_DIFF
		//ignore first part
		if (fields.size() < 3) { cerr << "invalid signal :" << signal << endl; exit(-1); }
		if (fields[1] == "bv") {
			if (fields[2] == "DIFF") {
				addDiffTask(cs, signal);
			}
			else if (fields[2] == "RATIO") {
				addRatioTask(cs, signal);
			}
			else {
				cerr << "not valid signal catogery : " << signal << endl; exit(-1);
			}
			return;
		}
		if (fields[1] == "bo") {
			if (fields.size() < 4) { cerr << "invalid signal fields count : signal:" << signal << endl; exit(-1); }
			int param = stoi(fields[2]);
			param *= 1000;//convert to millionseonds
			if (fields[3] == "AVG") {
				addAVGTask(cs, signal, param);
			}
			else if (fields[3] == "SHIFT") {
				addSHIFTTask(cs, signal, param);
			}
			else {
				cerr << "not valid signal catogery : " << signal << endl; exit(-1);
			}
			return;
		}
		cerr << "not valid signal catogery : " << signal << endl; exit(-1);
	}

	void addDiffTask(ComputeSignal & cs, const std::string & signalName) {
		using namespace std;
		using namespace placeholders;
		std::function<int(const string&, const string&)> fbvdiff(&diff);
		ComputeBV<int, const string&, const string&> bvdiff(/*"rb_bv_DIFF"*/signalName, fbvdiff);
		//std::shared_ptr<ComputeBV<int, const string&, const string&>> bvdiff(new ComputeBV<int, const string&, const string&>(signalName, fbvdiff));
		//copy bvdiff
		cs.addTask(std::bind(&ComputeBV<int, const string&, const string&>::compute, bvdiff, _1, _2, _3, _4));
	}

	void addRatioTask(ComputeSignal & cs, const std::string & signalName) {
		using namespace std;
		using namespace placeholders;
		std::function<double(const string&, const string&)> fbvratio(&ratio);
		ComputeBV<double, const string&, const string&> bvratio(/*"rb_bv_DIFF"*/signalName, fbvratio);
		//std::shared_ptr<ComputeBV<double, const string&, const string&>> bvratio(new ComputeBV<double, const string&, const string&>(signalName, fbvratio));;
		cs.addTask(std::bind(&ComputeBV<double, const string&, const string&>::compute, bvratio, _1, _2, _3, _4));
	}

	void addAVGTask(ComputeSignal & cs, const std::string & signalName, int timeDiffMils) {
		//AVGCaculator avg;
		using namespace std;
		using namespace placeholders;
		std::shared_ptr<AVGCaculator> avg(new AVGCaculator);
		std::function<double(int, int, double)> fboavg = std::bind(&AVGCaculator::avg, avg, _1, _2, _3);
		//ComputeBO<double, int, int, double> boavg(/*"rb_bo_5_AVG"*/signalName, timeDiffMils, fboavg);
		auto boavg = std::make_shared<ComputeBO<double, int, int, double>>(signalName, timeDiffMils, fboavg);
		//copy smart pointer
		cs.addTask(std::bind(&ComputeBO<double, int, int, double>::compute, boavg, _1, _2, _3, _4));
	}

	void addSHIFTTask(ComputeSignal & cs, const std::string & signalName, int timeDiffMils) {
		using namespace std;
		using namespace placeholders;
		std::shared_ptr<SHIFTCaculator> shift(new SHIFTCaculator);
		std::function<double(int, int, double)> fboshift = std::bind(&SHIFTCaculator::shift, shift, _1, _2, _3);
		//ComputeBO<double, int, int, double> boshift(signalName, timeDiffMils, fboshift);
		auto boshift = make_shared<ComputeBO<double, int, int, double>>(signalName, timeDiffMils, fboshift);
		cs.addTask(std::bind(&ComputeBO<double, int, int, double>::compute, boshift, _1, _2, _3, _4));
	}

	void batch() {
		ifstream ifs("signals.txt");
		vector<string> signals;
		istream_iterator<string> iter(ifs);
		istream_iterator<string> eof;

		copy(iter, eof, back_inserter(signals));
		if (signals.empty()) { return; }

		//sort(signals.begin(), signals.end());
		//this is mentioned in specification
		unordered_map<string, string> hash{ {"rb", "rb1705"}, {"ru", "ru1705"}, {"cu", "cu1703"}, {"al", "al1702"} };

		//let's build caculators
		unordered_map<string, ComputeSignal> cal;
		for (auto item : signals) {
			string curHead = getContractHead(item);
			auto& cs = cal[curHead];
			parseAndAddFunction(item, cs);
		}

		//iterate through files in this dir, and do caculation
		auto fileNames = listDir(".");
		for (auto file : fileNames) {
			if (file.size() < 2) { continue; }
			string head = file.substr(0, 2);
			auto iter = hash.find(head);
			if (iter == hash.end()) { continue; }
			string filePrefix = file.substr(0, iter->second.size());
			if (filePrefix != iter->second) { continue; }
			//then this is our target file
			//better copy than use ref. cause ref will alter internal state of avg and shift.
			//
			auto cs = cal[head];
			cs.setInputFileName(file);
			cs.setOutputFileName(file + "output.txt");
			cs.process();
			cout << "processed file:" << file << "->" << file + "output.txt" << endl;
		}
	}
}
