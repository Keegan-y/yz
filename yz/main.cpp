#include <iostream>
#include "Record.h"

using namespace std;
using namespace SignalCaculator;

int main() {
	ComputeSignal cs("ru1705_20161205.csv", "output.txt");
	auto ret = cs.process();
	cout << "processing finished" << endl;
	return ret;
}