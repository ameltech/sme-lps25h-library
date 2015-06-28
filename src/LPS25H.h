/*
 * LPS25H.h
 *
 * Created: 02/01/2015 20:50:30
 *  Author: speirano
 */


#ifndef LPS25H_H_
#define LPS25H_H_

#include <Arduino.h>


class LPS25H
{
    private:
    uint8_t _address;
    byte readRegister(byte slaveAddress, byte regToRead);
    bool writeRegister(byte slaveAddress, byte regToWrite, byte dataToWrite);

public:
    LPS25H(void);
    bool begin(void);
    bool activate(void);
    bool deactivate(void);

    int readPressure(void);
    int readTemperature(void);

protected:
    int _pressure;
    int16_t _temperature;
};


extern LPS25H smePressure;

#endif /* LPS25H_H_ */
