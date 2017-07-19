#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <deque>
#include <functional>
namespace SignalCaculator {
	struct Record {
		/*交易日, 0
		合约代码,
		交易所代码,
		合约在交易所的代码,
		最新价,
		上次结算价, 5
		昨收盘,
		昨持仓量,
		今开盘,
		最高价,
		最低价, 10
		数量,
		成交金额,
		持仓量,
		今收盘,
		本次结算价, 15
		涨停板价,
		跌停板价,
		昨虚实度,
		今虚实度,
		最后修改时间, 20
		最后修改毫秒,
		申买价一,
		申买量一,
		申卖价一,
		申卖量一, 25
		申买价二,
		申买量二,
		申卖价二,
		申卖量二,
		申买价三, 30
		申买量三,
		申卖价三,
		申卖量三,
		申买价四,
		申买量四, 35
		申卖价四,
		申卖量四,
		申买价五,
		申买量五,
		申卖价五, 40
		申卖量五,
		当日均价,
		业务日期
		*/
		//std::string tradingDay;
		//std::string contractNumber;
		//std::string exchange;
		//std::string contractNumberInExchange;
		//double latestPrice;


	};

	enum RecordFields
	{
		LASTMODIFYTIME = 20,
		LASTMODIFYMILS = 21,
		BIDPRICE1 = 22,
		BIDVOL1 = 23,
		ASKPRICE1 = 24,
		ASKVOL1 = 25
	};

	//this is a value object. it's ok to copy object.
	class ComputeSignal {
	public:
		using Callable = std::function<void(std::vector<int> &endPos, std::string &line, std::ofstream &ofs, std::string &curTimeStamp)>;
		ComputeSignal(const std::string& inputFileName, const std::string& outputFileName);
		ComputeSignal() = default;
		void setInputFileName(const std::string& name) { inFile_ = name; }
		void setOutputFileName(const std::string& name) { outFile_ = name; }
		//void computeBVDIFF(std::vector<int> &endPos, std::string &line, std::ofstream &ofs, std::string &curTimeStamp);
		int process();
		void computeBOAVG(std::vector<int>& endPos, std::string & line, int & curTSMilis, std::deque<std::pair<int, double>>& acc, double & accSum, std::ofstream & ofs, std::string & curTimeStamp, int diff);
		//void computeBOAVG(std::vector<int> &endPos, std::string &line, int &curTSMilis, std::deque<std::pair<int, double>> &acc, double &accSum, std::ofstream &ofs, std::string &curTimeStamp);
		void addTask(const Callable& c) { tasks_.push_back(c); }
	private:
		std::string inFile_;
		std::string outFile_;
		std::vector <Callable> tasks_;
		std::vector<std::string> preprocess(std::ifstream & ifs);
		void checkRecord(const std::string & line);
		//void removeOldRecords(const std::string & cur, int diffMilis, std::deque<std::pair<std::string, double>>& d);

	};
	std::vector<int> getEndPos(const std::string& line);
	std::string getField(const std::vector<int>& endPos, int index, const std::string & line);
	std::string getTimeStampStr(const std::string & s);
	int matchAndExtractNumber(const std::string & s, int index, int lenMax, int factor);
	int strToMillisecond(const std::string & s);
	int timeDiff(const std::string & prev, const std::string & cur);
	int timeDiff(int prev, int cur);
	std::vector<std::string> readLinesFromFile(std::ifstream & ifs);
	void sortToTimeStampAscending(std::vector<std::string>& records);
	void removeOldRecords(int cur, int diffMilis, std::deque<std::pair<int, double>>& d, double& averSum);
	//class ComputeBVDIFF {
	//	std::ofstream& ofs_;
	//public:
	//	ComputeBVDIFF(std::ofstream& ofs);
	//	void computeBVDIFF(std::vector<int> &endPos, std::string &line, std::string &curTimeStamp);
	//};

	//class ComputeBVRATIO {
	//	std::ofstream& ofs_;
	//public:
	//	ComputeBVRATIO(std::ofstream& ofs);
	//	void computeBVRATIO(std::vector<int> &endPos, std::string &line, std::string &curTimeStamp);
	//};

	

	

	/*
	rb_bv_DIFF
rb_bv_RATIO
rb_bo_3_SHIFT
rb_bo_5_AVG
ru_bv_DIFF
ru_bv_RATIO
ru_bo_10_SHIFT
ru_bo_30_AVG
cu_bv_DIFF
cu_bv_RATIO
cu_bo_3_SHIFT
cu_bo_10_AVG
al_bv_DIFF
al_bv_RATIO
al_bo_5_SHIFT
al_bo_10_AVG

*/
}