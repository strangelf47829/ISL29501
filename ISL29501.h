#include <Wire.h>
#include "Statistics.hpp"

#ifndef ISL29501_H
#define ISL29501_H

#ifndef ISL29501_ADDR
#define ISL29501_ADDR 0x57
#endif

#ifndef ISL29501_TIMEOUT
#define ISL29501_TIMEOUT 1000
#endif

enum ISL29501Registers
{	
	Id = 0x00,
	Enable,
	Status,
	IntegrationPeriod = 0x10,
	SamplePeriod,
	SamplePeriodRange,
	SampleControl,
	AGCControl = 0x19,
	Crosstalk_I_Exponent = 0x24,
	Crosstalk_I_MSB,
	Crosstalk_I_LSB,
	Crosstalk_Q_Exponent,
	Crosstalk_Q_MSB,
	Crosstalk_Q_LSB,
	Crosstalk_Gain_MSB,
	Crosstalk_Gain_LSB,
	Magnitude_Reference_Exp,
	Magnitude_Reference_MSB,
	Magnitude_Reference_LSB,
	Phase_Offset_MSB,
	Phase_Offset_LSB,
	TemperatureReference,
	PhaseExponent = 0x33,
	PhaseTemperatureB,
	PhaseAmbientB = 0x36,
	PhaseTemperatureA = 0x39,
	PhaseAmbientA = 0x3B,
	InterruptControl = 0x60,
	DriverRange = 0x90,
	EmitterDAC,
	DriverControl,
	ThresholdDAC,
	EmitterOffset = 0xA5,
	CommandRegister = 0xB0,

	DistanceReadout = 0xD1,
	Precision = 0xD3,
	MagnitudeExponent,
	MagnitudeSignificandMSB,
	MagnitudeSignificandLSB,
	PhaseReadoutMSB,
	PhaseReadoutLSB,
	IRawExponent,
	IRawMSB,
	IRawLSB,
	QRawExponent,
	QRawMSB,
	QRawLSB,
	EmitterVoltage = 0xE1,
	DieTemperature,
	Vga1,
	Vga2,
	GainMSB,
	GainLSB
};

enum ISL29501Commands 
{
	Sample = 0x49,
	Reset = 0xD7,
	Clear = 0xD1
};

struct ISL29501_Frame
{
	float distance;
	float precision;
	float phase;
	float magnitude;

	uint16_t rawDistance;
	uint16_t rawPrecision;
	uint16_t rawPhase;
	uint16_t rawMagnitude;
	uint8_t magnitudeExponent;
};

int initializeISL29501();

int ISL29501_Write(unsigned char _register, unsigned char value);
int ISL29501_Write16(unsigned char _register, uint16_t value);
int ISL29501_Write16(unsigned char _regsiter1, unsigned char _register2, unsigned char value);

uint8_t ISL29501_Read8(unsigned char _register);
uint16_t ISL29501_Read16(unsigned char _register);
uint16_t ISL29501_Read16(unsigned char _register1, unsigned char _register2);

uint16_t ISL29501_rawConversion(float);

ISL29501_Frame ISL29501_QuickMeasure();
ISL29501_Frame ISL29501_Measure(unsigned int);

unsigned int ISL29501_Delay();

int ISL29501_QuickCalibrate(float knownDistance, float precision);
int ISL29501_QuickCalibrate(float knownDistance, float precision, unsigned int delay);
int ISL29501_QuickCalibrate(float knownDistance, float precision, unsigned int delay, unsigned int timeout);

int ISL29501_Calibrate(float knownDistance, float precision);
int ISL29501_Calibrate(float knownDistance, float precision, unsigned int delay);
int ISL29501_Calibrate(float knownDistance, float precision, unsigned int delay, unsigned int timeout);


#endif 