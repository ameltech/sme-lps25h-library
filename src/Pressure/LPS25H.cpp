/*
* LPS25H.c
*
* Created: 02/01/2015 20:50:20
*  Author: speirano
*/
#include <Wire.h>
#include "LPS25H.h"
#include "LPS25HReg.h"


LPS25H::LPS25H(void) : _address(LPS25H_ADDRESS)
{
     _pressure = 0;
     _temperature = 0;
}


bool LPS25H::begin(void)
{
    uint8_t data;

    data = readRegister(_address, WHO_AM_I);
    if (data == WHO_AM_I_RETURN){
        if (activate()){
            return true;
        }
    }
    return false;
}



bool
LPS25H::activate(void)
{
    uint8_t data;

    data = readRegister(_address, CTRL_REG1);
    data |= POWER_UP;
    data |= ODR0_SET;
    writeRegister(_address, CTRL_REG1, data);

    return true;
}


bool LPS25H::deactivate(void)
{
    uint8_t data;

    data = readRegister(_address, CTRL_REG1);
    data &= ~POWER_UP;
    writeRegister(_address, CTRL_REG1, data);
    return true;
}



int
LPS25H::readTemperature(void)
{
    unsigned int   data = 0;
    unsigned char  read = 0;
    double         t_temp = 0.0;

    read = readRegister(_address, STATUS_REG);
    if (read & TEMPERATURE_READY) {
        read = readRegister(_address, TEMP_L_REG);
        data = read;      // LSB

        read = readRegister(_address, TEMP_H_REG);
        data |= read << 8; // MSB
        _temperature = data;
        // Decode Temperature
        t_temp = 42.5 +(_temperature/480.0);
        _temperature  = t_temp;  // temp in Celsius degree
    }
    return _temperature;
}

int
LPS25H::readPressure(void)
{
    unsigned long data   = 0;
    double        p_temp = 0.0;
    unsigned char read   = 0;

    read = readRegister(_address, STATUS_REG);
    if (read & PRESSURE_READY) {
        read = readRegister(_address, PRESSURE_H_REG);
        data = read << 16;  // MSB
        read = readRegister(_address, PRESSURE_L_REG);
        data |= read << 8; // LSB
        read = readRegister(_address, PRESSURE_XL_REG);
        data |= read; // XLSB

        // Decode pressure
        p_temp = ((long) data) / 4096.0;
        _pressure = p_temp;
        return _pressure;
    }

    return _pressure;
}



// Read a single byte from addressToRead and return it as a byte
byte LPS25H::readRegister(byte slaveAddress, byte regToRead)
{
    Wire.beginTransmission(slaveAddress);
    Wire.write(regToRead);
    Wire.endTransmission(false); //endTransmission but keep the connection active

    Wire.requestFrom(slaveAddress, 1); //Ask for 1 byte, once done, bus is released by default

    while(!Wire.available()) ; //Wait for the data to come back
    return Wire.read(); //Return this one byte
}

// Writes a single byte (dataToWrite) into regToWrite
bool LPS25H::writeRegister(byte slaveAddress, byte regToWrite, byte dataToWrite)
{
    Wire.beginTransmission(slaveAddress);

    if (!Wire.write(regToWrite)) {
        return false;
    }
    if (!Wire.write(dataToWrite)) {
        return false;
    }

    uint8_t errorNo = Wire.endTransmission(); //Stop transmitting
    return (errorNo == 0);
}



LPS25H smePressure;
