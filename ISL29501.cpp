#include <Wire.h>
#include <Arduino.h>
#include "ISL29501.h"


int initializeISL29501()
{
	// Soft reset the ISL29501.
	int status = ISL29501_Write(CommandRegister, Reset);
	if (status)
		return status;
	
	delay(400);
	return status;
}

int ISL29501_Write(unsigned char _register, unsigned char value)
{
	// Begin transmission with ISL29501
	Wire.beginTransmission(ISL29501_ADDR);

	// Write register, then value
	(void) Wire.write(_register);
	(void) Wire.write(value);

	// End transmission with stop bit.
	return Wire.endTransmission(true);
}

int ISL29501_Write16(unsigned char _register, uint16_t value)
{
	// Write 16 register and register + 1
	return ISL29501_Write16(_register, _register + 1, value);
}

int ISL29501_Write16(unsigned char _register1, unsigned char _register2, uint16_t value)
{
	// Calculate values for register 1 and 2
	uint8_t value_reg1 = (value >> 8);
	uint8_t value_reg2 = value & 0xFF;

	// Since datasheet does not say multiple writes are possible, just write both sequentially
	int reg1_ret = ISL29501_Write(_register1, value_reg1);
	int reg2_ret = ISL29501_Write(_register2, value_reg2);

	// Return or-ed value
	return reg1_ret | reg2_ret;
}

uint8_t ISL29501_Read8(unsigned char _register)
{
	// Start transmission with ISL29501
	Wire.beginTransmission(ISL29501_ADDR);

	// Write register to ISL29501, then end transmission
	(void) Wire.write(_register);
	(void) Wire.endTransmission();

	// Request value from ISL29501
	(void) Wire.requestFrom(ISL29501_ADDR, 1);

	// Wait until data is available
	uint16_t cycles = 0;
	while (!Wire.available() && cycles++ < ISL29501_TIMEOUT)
	{ }

	// Return data
	return Wire.read();
}

uint16_t ISL29501_Read16(unsigned char _register)
{
	// Read registers _register and _register + 1

	// Start transmission
	Wire.beginTransmission(ISL29501_ADDR);
	(void) Wire.write(_register);
	(void) Wire.endTransmission();

	// Request 2 bytes from ISL29501
	(void) Wire.requestFrom(ISL29501_ADDR, 2);

	// Wait until data is available
	uint16_t cycles = 0;
	while(!Wire.available() && cycles++ < ISL29501_TIMEOUT)
	{ }

	// Read both bytes
	uint8_t value_reg1 = Wire.read();
	uint8_t value_reg2 = Wire.read();

	// Calculate total
	uint16_t returnValue = value_reg2 & 0xFF;
	returnValue = ((uint16_t)value_reg1 << 8) | returnValue;

	// Return value
	return returnValue;
}

uint16_t ISL29501_Read16(unsigned char _register1, unsigned char _register2)
{
	// Explicitly measure both registers seperately.
	uint8_t value_reg1 = ISL29501_Read8(_register1);
	uint8_t value_reg2 = ISL29501_Read8(_register2);

	// calculate total
	uint16_t returnValue = value_reg2 & 0xFF;
	returnValue = ((uint16_t)value_reg1 << 8) | returnValue;

	// Return value
	return returnValue;
}

ISL29501_Frame ISL29501_QuickMeasure()
{
	// tell ISL29501 to start measurement
	(void) ISL29501_Write(CommandRegister, Sample);
	(void) ISL29501_Read8(InterruptControl);

	// Create empty frame
	ISL29501_Frame frame = ISL29501_Frame();

	// Read data from I2C bus
	uint16_t distance = ISL29501_Read16(DistanceReadout);
	uint16_t precision = ISL29501_Read16(Precision);
	uint16_t phase = ISL29501_Read16(PhaseReadoutMSB);

	// Save data
	frame.rawDistance = distance;
	frame.rawPrecision = precision;
	frame.rawPhase = phase;

	// Return data
	return frame;
}