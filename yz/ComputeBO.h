#pragma once
#include "Record.h"
namespace SignalCaculator {

	template<class Return, class Param1, class Param2, class Param3>
	class ComputeBO {
		std::string algorithm_;
		int param_;
		std::function<Return(Param1, Param2, Param3)> task_;
	public:
		ComputeBO(const std::string& algorithm, int param, const std::function<Return(Param1, Param2, Param3)>& task);
		void compute(std::vector<int> &endPos, std::string &line, std::ofstream & ofs, std::string &curTimeStamp);
	};

	template<class Return, class Param1, class Param2, class Param3>
	inline ComputeBO<Return, Param1, Param2, Param3>::ComputeBO(const std::string & algorithm, int param, const std::function<Return(Param1, Param2, Param3)>& task)
		: algorithm_(algorithm)
		, param_(param)
		, task_(task)
	{
	}

	template<class Return, class Param1, class Param2, class Param3>
	inline void ComputeBO< Return,  Param1,  Param2,  Param3>::compute(std::vector<int>& endPos, std::string & line, std::ofstream& ofs, std::string & curTimeStamp)
	{
		using namespace std;

		string askPrice1 = getField(endPos, RecordFields::ASKPRICE1, line);
		string bidPrice1 = getField(endPos, RecordFields::BIDPRICE1, line);
		double ap1 = stod(askPrice1);
		double bp1 = stod(bidPrice1);
		double curDiff = ap1 - bp1;
		int curTSMili = strToMillisecond(curTimeStamp);
		//int curTSMilis, int timeRange, int curDiff
		auto result = task_(curTSMili, param_, curDiff);
		ofs << curTimeStamp << " " << algorithm_ << " " << result << endl;
	}

	class AVGCaculator {
		std::deque<std::pair<int, double>> acc_;
		double accSum_ = 0;
	public:
		double avg(int curTSMilis, int timeRange, double curDiff) {
			using namespace std;
			//if any record is not within the recent 5s, remove it
			removeOldRecords(curTSMilis, timeRange, acc_, accSum_);

			double result = curDiff;
			if (!acc_.empty()) {
				result += accSum_ / static_cast<double>(acc_.size());
			}

			acc_.push_back(make_pair(curTSMilis, curDiff));
			accSum_ += curDiff;

			return result;
		}
	};

	class SHIFTCaculator {
		std::deque<std::pair<int, double>> acc_;
	public:
		double shift(int curTSMilis, int timeRange, double curDiff) {
			using namespace std;
			//if any record is not within the recent 5s, remove it
			double dummy;
			removeOldRecords(curTSMilis, timeRange, acc_, dummy);

			double result = curDiff;
			if (!acc_.empty()) {
				auto ref = acc_.front();
				if (curTSMilis - ref.first == timeRange) {
					result -= ref.second;
				}
			}

			acc_.push_back(make_pair(curTSMilis, curDiff));
			return result;
		}
	};
}