/* 
 * File:   Timeout.cpp
 * Author: marcone
 * 
 * Created on 25 de Setembro de 2019, 08:05
 */

#include "Timeout.h"

namespace Timeout {

Timeout::Timeout() : _event(0), _enable(0), _counter(0)
{
	// TODO Auto-generated constructor stub

}

Timeout::~Timeout() {
	// TODO Auto-generated destructor stub
}

void Timeout::config(uint32_t interval, CALLBACK_t callback)
{
	_enable = true;
	_callback = callback;
	_interval = interval;
}

void Timeout::checkTimeout()
{
	if (_enable) {
		_counter++;
		if (_counter == _interval) {
			_event = true;
			_counter = 0;
		}
	}

}

void Timeout::callback()
{
	if (_event and _enable) {
		(*_callback)();
		_event = false;
	}
}

bool Timeout::event()
{
	return _event;
}

void Timeout::enable()
{
	_enable = true;
}

void Timeout::disable()
{
	_enable = false;
}
}