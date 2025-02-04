#include "MCP23017.h"

#define IODIRA   0x00 // Registr pro nastaven� sm�ru pin� (A)
#define IODIRB   0x01 // Registr pro nastaven� sm�ru pin� (B)
#define GPIOA    0x12 // Stav GPIO A
#define GPIOB    0x13 // Stav GPIO B
#define OLATA    0x14 // V�stupn� stav A
#define OLATB    0x15 // V�stupn� stav B

MCP23017::MCP23017(uint8_t i2c_address) {
    _i2c_address = i2c_address;
    _gpioState = 0xFFFF; // V�choz� stav v�ech pin� na HIGH (pull-up)
}

void MCP23017::begin() {
    Wire.begin();
    writeRegister(IODIRA, 0x00); // V�echny piny A jako v�stup
    writeRegister(IODIRB, 0x00); // V�echny piny B jako v�stup
}

void MCP23017::pinMode(uint8_t pin, uint8_t mode) {
    uint8_t reg = (pin < 8) ? IODIRA : IODIRB;
    uint8_t bit = pin % 8;
    uint8_t current = readRegister(reg);

    if (mode == OUTPUT) {
        current &= ~(1 << bit);
    }
    else {
        current |= (1 << bit);
    }

    writeRegister(reg, current);
}

void MCP23017::digitalWrite(uint8_t pin, uint8_t value) {
    uint8_t reg = (pin < 8) ? OLATA : OLATB;
    uint8_t bit = pin % 8;

    if (value == HIGH) {
        _gpioState |= (1 << pin);
    }
    else {
        _gpioState &= ~(1 << pin);
    }

    writeRegister(reg, (_gpioState >> (pin < 8 ? 0 : 8)) & 0xFF);
}

int MCP23017::digitalRead(uint8_t pin) {
    uint8_t reg = (pin < 8) ? GPIOA : GPIOB;
    uint8_t bit = pin % 8;
    return (readRegister(reg) & (1 << bit)) ? HIGH : LOW;
}

void MCP23017::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t MCP23017::readRegister(uint8_t reg) {
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(_i2c_address, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0;
}
