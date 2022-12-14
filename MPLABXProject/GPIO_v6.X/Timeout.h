/* 
 * File:   Timeout.h
 * Author: marcone
 *
 * Created on 25 de Setembro de 2019, 08:05
 */
#ifndef TIMEOUT_H_
#define TIMEOUT_H_
#include <stdint.h>

namespace Timeout {

typedef void (*CALLBACK_t)(void);

class Timeout {
public:
	Timeout();
	~Timeout();

	void config(uint32_t interval, CALLBACK_t callback);
	void checkTimeout();
	void callback();
	void enable();
	void disable();
	bool event();

private:
	bool _event;
	bool _enable;
	CALLBACK_t _callback;
	uint32_t _counter;
	uint32_t _interval;
};

}

#endif /* TIMEOUT_H_ */



