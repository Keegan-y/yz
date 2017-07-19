#pragma once
#include <memory>
#include <algorithm>
#include "Record.h"
#include "ComputeBO.h"
#include "ComputeBV.h"

namespace SignalCaculator {
	void addDiffTask(ComputeSignal& cs, const std::string& signalName) {
		using namespace std;
		using namespace placeholders;
		std::function<int(const string&, const string&)> fbvdiff(&diff);
		ComputeBV<int, const string&, const string&> bvdiff(/*"rb_bv_DIFF"*/signalName, fbvdiff);
		//std::shared_ptr<ComputeBV<int, const string&, const string&>> bvdiff(new ComputeBV<int, const string&, const string&>(signalName, fbvdiff));
		//copy bvdiff
		cs.addTask(std::bind(&ComputeBV<int, const string&, const string&>::compute, bvdiff, _1, _2, _3, _4));
	}

	void addRatioTask(ComputeSignal& cs, const std::string& signalName) {
		using namespace std;
		using namespace placeholders;
		std::function<double(const string&, const string&)> fbvratio(&ratio);
		ComputeBV<double, const string&, const string&> bvratio(/*"rb_bv_DIFF"*/signalName, fbvratio);
		//std::shared_ptr<ComputeBV<double, const string&, const string&>> bvratio(new ComputeBV<double, const string&, const string&>(signalName, fbvratio));;
		cs.addTask(std::bind(&ComputeBV<double, const string&, const string&>::compute, bvratio, _1, _2, _3, _4));
	}


	void addAVGTask(ComputeSignal& cs, const std::string& signalName, int timeDiffMils) {
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

	void addSHIFTTask(ComputeSignal& cs, const std::string& signalName, int timeDiffMils) {
		using namespace std;
		using namespace placeholders;
		std::shared_ptr<SHIFTCaculator> shift(new SHIFTCaculator);
		std::function<double(int, int, double)> fboshift = std::bind(&SHIFTCaculator::shift, shift, _1, _2, _3);
		//ComputeBO<double, int, int, double> boshift(signalName, timeDiffMils, fboshift);
		auto boshift = make_shared<ComputeBO<double, int, int, double>>(signalName, timeDiffMils, fboshift);
		cs.addTask(std::bind(&ComputeBO<double, int, int, double>::compute, boshift, _1, _2, _3, _4));
	}


}