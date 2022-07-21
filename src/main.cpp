#include "common.hpp"
#include <fs>

extern "C" void _start() {
	// Let's create the base structure
	loCreate(getParts("/u"));
	loCreate(getParts("/g"));
	publish();
}
