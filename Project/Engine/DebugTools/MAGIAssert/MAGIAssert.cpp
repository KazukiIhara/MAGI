#include "MAGIAssert.h"

#include <cstdlib> 
#include <cassert>

void MAGIAssert::Assert(bool test, const std::string& message) {
	if (test) {
		return;
	}

	Logger::Log("Assertion Failed!\n");
	Logger::Log(message);
	Logger::Finalize();
	std::abort();
}
