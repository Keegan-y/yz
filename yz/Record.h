#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <deque>
#include <functional>
namespace SignalCaculator {
//	struct Record {
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


//	};

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
		using Callable = std::function<void(std::vector<size_t> &endPos, std::string &line, std::ofstream &ofs, std::string &curTimeStamp)>;
		ComputeSignal(const std::string& inputFileName, const std::string& outputFileName);
		ComputeSignal() = default;
		void setInputFileName(const std::string& name) { inFile_ = name; }
		void setOutputFileName(const std::string& name) { outFile_ = name; }
		//void computeBVDIFF(std::vector<int> &endPos, std::string &line, std::ofstream &ofs, std::string &curTimeStamp);
		int process();
//		void computeBOAVG(std::vector<int>& endPos, std::string & line, int & curTSMilis, std::deque<std::pair<int, double>>& acc, double & accSum, std::ofstream & ofs, std::string & curTimeStamp, int diff);
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
	std::vector<size_t> getEndPos(const std::string& line);
	std::string getField(const std::vector<size_t>& endPos, int index, const std::string & line);
	std::string getTimeStampStr(const std::string & s);
	int matchAndExtractNumber(const std::string & s, int index, int lenMax, int factor);
	int strToMillisecond(const std::string & s);
//	int timeDiff(const std::string & prev, const std::string & cur);
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