#pragma once
#include "Record.h"

namespace SignalCaculator {

	template<class Return, class Param1, class Param2>
	class ComputeBV {
		std::string algorithm_;
		std::function<Return(Param1, Param2)> task_;
	public:
		ComputeBV(const std::string& algorithm, const std::function<Return(Param1, Param2)>& task);
		void compute(std::vector<size_t> &endPos, std::string &line, std::ofstream& ofs ,std::string &curTimeStamp );
	};

	template<class Return, class Param1, class Param2>
	inline ComputeBV<Return, Param1, Param2>::ComputeBV(const std::string & algorithm, const std::function<Return(Param1, Param2)>& task)
		:
		algorithm_(algorithm)
		, task_(task)
	{
	}

	template<class Return, class Param1, class Param2>
	inline void ComputeBV<Return, Param1, Param2>:: compute(std::vector<size_t> &endPos, std::string &line, std::ofstream& ofs, std::string &curTimeStamp)
	{
		using namespace std;
		string bidVol1 = getField(endPos, RecordFields::BIDVOL1, line);
		string askVol1 = getField(endPos, RecordFields::ASKVOL1, line);
		//int bv1 = stoi(bidVol1);
		//int av1 = stoi(askVol1);
		//double result = log(1.0*bv1 / av1);
		auto result = task_(bidVol1, askVol1);
		ofs << curTimeStamp << " " << algorithm_ << " " << result << endl;
	}

	double ratio(const std::string& bidVol1, const std::string& askVol1) {
		int bv1 = stoi(bidVol1);
		int av1 = stoi(askVol1);
		double result = log2(1.0 * bv1 / av1);
		return result;
	}

	int diff(const std::string& bidVol1, const std::string& askVol1) {
		int bv1 = stoi(bidVol1);
		int av1 = stoi(askVol1);
		int result = bv1 - av1;
		return result;
	}
}