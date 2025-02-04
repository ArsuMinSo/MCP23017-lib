#ifndef MCP23017_H
#define MCP23017_H

#include <Arduino.h>
#include <Wire.h>

class MCP23017 {
public:
    MCP23017(uint8_t i2c_address = 0x20);  // Výchozí I2C adresa
    void begin();
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t value);
    int digitalRead(uint8_t pin);

private:
    uint8_t _i2c_address;
    uint16_t _gpioState; // Ukládá stav pinù
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
};

#endif
