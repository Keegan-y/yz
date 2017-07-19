#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <deque>
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

	class ComputeSignal {
	public:
		ComputeSignal(const std::string& inputFileName, const std::string& outputFileName)
			: inFile_(inputFileName)
			, outFile_(outputFileName){}
		int process();
	private:
		std::string inFile_;
		std::string outFile_;

		std::vector<int> getEndPos(const std::string& line);
		std::string getField(const std::vector<int>& endPos, int index, const std::string & line);
		std::string getTimeStampStr(const std::string & s);
		int matchAndExtractNumber(const std::string & s, int index, int lenMax, int factor);
		int strToMillisecond(const std::string & s);
		int timeDiff(const std::string & prev, const std::string & cur);
		int timeDiff(int prev, int cur);
		std::vector<std::string> readLinesFromFile(std::ifstream & ifs);
		void sortToTimeStampAscending(std::vector<std::string>& records);
		std::vector<std::string> preprocess(std::ifstream & ifs);
		void checkRecord(const std::string & line);
		//void removeOldRecords(const std::string & cur, int diffMilis, std::deque<std::pair<std::string, double>>& d);
		void removeOldRecords(int cur, int diffMilis, std::deque<std::pair<int, double>>& d, double& averSum);
	};
}