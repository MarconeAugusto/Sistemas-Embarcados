/* 
 * File:   GPIO.h
 * Author: marcone
 *
 * Created on 9 de Setembro de 2019, 07:47
 */

#ifndef GPIO_H
#define	GPIO_H

#include <stdint.h>

class GPIO {
public:
    enum PortDirection_t{
        INPUT = 0,
        OUTPUT = 1
    };
    GPIO(uint8_t id, PortDirection_t dir);
    ~GPIO();
    
    bool get();
    void set(bool val = 1);
    void clear();
    void toggle();
 
private:
    uint8_t _bit;
    uint8_t _id;
};

#endif	/* GPIO_H */

