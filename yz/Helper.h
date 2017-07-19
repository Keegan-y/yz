#pragma once
#include <memory>
#include <algorithm>
#include "Record.h"


namespace SignalCaculator {
	void addDiffTask(ComputeSignal& cs, const std::string& signalName);

	void addRatioTask(ComputeSignal& cs, const std::string& signalName);


	void addAVGTask(ComputeSignal& cs, const std::string& signalName, int timeDiffMils);

	void addSHIFTTask(ComputeSignal& cs, const std::string& signalName, int timeDiffMils);

	void batch();
}