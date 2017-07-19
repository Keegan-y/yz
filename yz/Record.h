#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <deque>
namespace SignalCaculator {
	struct Record {
		/*������, 0
		��Լ����, 
		����������, 
		��Լ�ڽ������Ĵ���, 
		���¼�, 
		�ϴν����, 5
		������, 
		��ֲ���, 
		����, 
		��߼�, 
		��ͼ�, 10
		����, 
		�ɽ����, 
		�ֲ���, 
		������, 
		���ν����, 15
		��ͣ���, 
		��ͣ���, 
		����ʵ��, 
		����ʵ��, 
		����޸�ʱ��, 20
		����޸ĺ���, 
		�����һ, 
		������һ, 
		������һ, 
		������һ, 25
		����۶�, 
		��������, 
		�����۶�, 
		��������, 
		�������, 30
		��������, 
		��������, 
		��������, 
		�������, 
		��������, 35
		��������, 
		��������, 
		�������, 
		��������, 
		��������, 40
		��������, 
		���վ���, 
		ҵ������
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