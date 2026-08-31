#pragma once

#include "rtos.hpp"

class Display {
public:
	Display(RTOS& rtos) : _rtos(rtos) {};

	void init();

private:
	RTOS& _rtos;
};
