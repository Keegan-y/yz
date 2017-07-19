#include <iostream>
#include "Record.h"
#include "ComputeBO.h"
#include "ComputeBV.h"

using namespace std;
using namespace SignalCaculator;
using namespace std::placeholders;

int main() {
	ComputeSignal cs("ru1705_20161205.csv", "output.txt");

	std::function<int(const string&, const string&)> fbvdiff(&diff);
	ComputeBV<int, const string&, const string&> bvdiff("rb_bv_DIFF", fbvdiff);
	cs.addTask(std::bind(&ComputeBV<int, const string&, const string&>::compute, &bvdiff, _1, _2, _3, _4));


	AVGCaculator avg;
	std::function<double(int, int, double)> fboavg5 = std::bind(&AVGCaculator::avg, &avg, _1, _2, _3);
	ComputeBO<double, int, int, double> boavg5("rb_bo_5_AVG", 5000, fboavg5);
	cs.addTask(std::bind(&ComputeBO<double, int, int, double>::compute, &boavg5, _1, _2, _3, _4));


	auto ret = cs.process();
	cout << "processing finished" << endl;
	return ret;
}