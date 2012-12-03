// Librería Camara
// 
// Copyright 2012 Israel Santana <isra@miscorreos.org>
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
//


#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "Camara.h"

// Copiado de Sensirion
extern "C" {
  // AVR LibC Includes
  #include <stddef.h>
  #include <stdint.h>
  #include <math.h>
}
/******************************************************************************
 * Definitions
 ******************************************************************************/

// Sensirion command definitions:      adr command r/w
const uint8_t MEAS_TEMP   = 0x03;   // 000  0001   1
const uint8_t MEAS_HUMI   = 0x05;   // 000  0010   1
const uint8_t STAT_REG_W  = 0x06;   // 000  0011   0
const uint8_t STAT_REG_R  = 0x07;   // 000  0011   1
const uint8_t SOFT_RESET  = 0x1e;   // 000  1111   0

// Status register writable bits
const uint8_t SR_MASK     = 0x07;

// getByte flags
const bool noACK  = false;
const bool ACK    = true;

// Temperature & humidity equation constants
const float D1  = -40.1;          // for deg C @ 5V
const float D2h =   0.01;         // for deg C, 14-bit precision
const float D2l =   0.04;         // for deg C, 12-bit precision

const float C1  = -2.0468;        // for V4 sensors
const float C2h =  0.0367;        // for V4 sensors, 12-bit precision
const float C3h = -1.5955E-6;     // for V4 sensors, 12-bit precision
const float C2l =  0.5872;        // for V4 sensors, 8-bit precision
const float C3l = -4.0845E-4;     // for V4 sensors, 8-bit precision

const float T1  =  0.01;          // for V3 and V4 sensors
const float T2h =  0.00008;       // for V3 and V4 sensors, 12-bit precision
const float T2l =  0.00128;       // for V3 and V4 sensors, 8-bit precision
// Fin del copiado

/* ================  Constructor ================ */

Camara::Camara(int pinEstado, int pinApagado, int pinDatosSensor, int pinRelojSensor)
{
	// El estado de la camara al iniciarse siempre pensamos que es true
	this->estado = true;
	
	this->_pinEstado	= pinEstado;
	this->_pinApagado	=	pinApagado;
	
	// Pongo los valores a 0 de los sensores
	
  this->temperatura = 0;
  this->humedad     = 0;
	
	// Parte propia de los sensores
	this->_pinData 		= pinDatosSensor;
	this->_pinClock 	= pinRelojSensor;
	this->_presult		= NULL;
	this->_stat_reg		= 0x00;
	
	pinMode(this->_pinClock, OUTPUT);
	resetConnection();
	putByte(SOFT_RESET);
	
	
	
}

Camara::Camara(int pinEstado, int pinApagado, int pinDatosSensor, int pinRelojSensor, int id)
{
  // El id de la camara
  this->id    = id;
	// El estado de la camara al iniciarse siempre pensamos que es true
	this->estado = true;
	
	this->_pinEstado	= pinEstado;
	this->_pinApagado	=	pinApagado;
	
	// Pongo los valores a 0 de los sensores
	
  this->temperatura = 0;
  this->humedad     = 0;
	
	// Parte propia de los sensores
	this->_pinData 		= pinDatosSensor;
	this->_pinClock 	= pinRelojSensor;
	this->_presult		= NULL;
	this->_stat_reg		= 0x00;
	
	pinMode(this->_pinClock, OUTPUT);
	resetConnection();
	putByte(SOFT_RESET);
	
	
	
}
/* ================  Métodos Públicos ================ */


int Camara::obtenerTemperatura()
{
	meas(0, &this->_rawData, true);
	this->temperatura = (int)round(calcTemp(this->_rawData));
  return this->temperatura;
}


int Camara::obtenerHumedad()
{
  // Primero tengo que obtener la temperatura y luego lo paso calculo la humedad
	float aux;
	meas(0, &this->_rawData, true);
  aux = calcTemp(this->_rawData);

	// Ahora me pongo con la humead en si
	meas(1, &this->_rawData, true);

  this->humedad = (int)round(calcHumi(this->_rawData, aux) );
  return this->humedad;
}


void Camara::apagar()
{
	pinMode(this->_pinApagado, OUTPUT); // Ponemos el pin de salida
	
	digitalWrite(this->_pinApagado, HIGH);
	delay(3000);
	digitalWrite(this->_pinApagado, LOW);
}


bool Camara::obtenerEstado()
{
	pinMode(this->_pinEstado, INPUT); // Ponemos el pin de entrada
	
	int lectura; // Defino el valor para leer el estado del pin
	
	lectura = digitalRead(this->_pinEstado); // leo el estado del pin
	
	if ( lectura == HIGH )
	{
    this->estado = true; // cambiamos el estado
		return true; // la cámara está encendida
	}
	else
	{
		this->estado = false; // cambiamos el estado
		return false; // la cámara está apagada
	}

}



bool Camara::comprobarCambioEstado()
{
  bool estadoAntes; // Variable para comparar
  estadoAntes = this->estado;

	obtenerEstado();

	if (this->estado != estadoAntes)
	{
		return true;
	}
	else
	{
		return false;
	}
}



/* ================ Private methods ================ */
/* 
 * Copiado directamente del original Sensirion
 */


	// Initiate measurement.  If blocking, wait for result
	uint8_t Camara::meas(uint8_t cmd, uint16_t *result, bool block) {
	  uint8_t error, i;
	#ifdef CRC_ENA
	  _crc = bitrev(_stat_reg & SR_MASK);  // Initialize CRC calculation
	#endif
	  startTransmission();
	  if (cmd == TEMP)
	    cmd = MEAS_TEMP;
	  else
	    cmd = MEAS_HUMI;
	  if (error = putByte(cmd))
	    return error;
	#ifdef CRC_ENA
	  calcCRC(cmd, &_crc);              // Include command byte in CRC calculation
	#endif
	  // If non-blocking, save pointer to result and return
	  if (!block) {
	    _presult = result;
	    return 0;
	  }
	  // Otherwise, wait for measurement to complete with 720ms timeout
	  i = 240;
	  while (digitalRead(_pinData)) {
	    i--;
	    if (i == 0)
	      return S_Err_TO;              // Error: Timeout
	    delay(3);
	  }
	  error = getResult(result);
	  return error;
	}


	// Get measurement result from sensor (plus CRC, if enabled)
	uint8_t Camara::getResult(uint16_t *result) {
	  uint8_t val;
	#ifdef CRC_ENA
	  val = getByte(ACK);
	  calcCRC(val, &_crc);
	  *result = val;
	  val = getByte(ACK);
	  calcCRC(val, &_crc);
	  *result = (*result << 8) | val;
	  val = getByte(noACK);
	  val = bitrev(val);
	  if (val != _crc) {
	    *result = 0xFFFF;
	    return S_Err_CRC;
	  }
	#else
	  *result = getByte(ACK);
	  *result = (*result << 8) | getByte(noACK);
	#endif
	  return 0;
	}



	/******************************************************************************
	 * Sensirion data communication
	 ******************************************************************************/

	// Write byte to sensor and check for acknowledge
	uint8_t Camara::putByte(uint8_t value) {
	  uint8_t mask, i;
	  uint8_t error = 0;
	  pinMode(_pinData, OUTPUT);        // Set data line to output mode
	  mask = 0x80;                      // Bit mask to transmit MSB first
	  for (i = 8; i > 0; i--) {
	    digitalWrite(_pinData, value & mask);
	    PULSE_SHORT;
	    digitalWrite(_pinClock, HIGH);  // Generate clock pulse
	    PULSE_LONG;
	    digitalWrite(_pinClock, LOW);
	    PULSE_SHORT;
	    mask >>= 1;                     // Shift mask for next data bit
	  }
	  pinMode(_pinData, INPUT);         // Return data line to input mode
	#ifdef DATA_PU
	  digitalWrite(_pinData, DATA_PU);  // Restore internal pullup state
	#endif
	  digitalWrite(_pinClock, HIGH);    // Clock #9 for ACK
	  PULSE_LONG;
	  if (digitalRead(_pinData))        // Verify ACK ('0') received from sensor
	    error = S_Err_NoACK;
	  PULSE_SHORT;
	  digitalWrite(_pinClock, LOW);     // Finish with clock in low state
	  return error;
	}

	// Read byte from sensor and send acknowledge if "ack" is true
	uint8_t Camara::getByte(bool ack) {
	  uint8_t i;
	  uint8_t result = 0;
	  for (i = 8; i > 0; i--) {
	    result <<= 1;                   // Shift received bits towards MSB
	    digitalWrite(_pinClock, HIGH);  // Generate clock pulse
	    PULSE_SHORT;
	    result |= digitalRead(_pinData);  // Merge next bit into LSB position
	    digitalWrite(_pinClock, LOW);
	    PULSE_SHORT;
	  }
	  pinMode(_pinData, OUTPUT);
	  digitalWrite(_pinData, !ack);     // Assert ACK ('0') if ack == 1
	  PULSE_SHORT;
	  digitalWrite(_pinClock, HIGH);    // Clock #9 for ACK / noACK
	  PULSE_LONG;
	  digitalWrite(_pinClock, LOW);     // Finish with clock in low state
	  PULSE_SHORT;
	  pinMode(_pinData, INPUT);         // Return data line to input mode
	#ifdef DATA_PU
	  digitalWrite(_pinData, DATA_PU);  // Restore internal pullup state
	#endif
	  return result;
	}


	/******************************************************************************
	 * Sensirion signaling
	 ******************************************************************************/

	// Generate Sensirion-specific transmission start sequence
	// This is where Sensirion does not conform to the I2C standard and is
	// the main reason why the AVR TWI hardware support can not be used.
	//       _____         ________
	// DATA:      |_______|
	//           ___     ___
	// SCK : ___|   |___|   |______
	void Camara::startTransmission(void) {
	  digitalWrite(_pinData, HIGH);  // Set data register high before turning on
	  pinMode(_pinData, OUTPUT);     // output driver (avoid possible low pulse)
	  PULSE_SHORT;
	  digitalWrite(_pinClock, HIGH);
	  PULSE_SHORT;
	  digitalWrite(_pinData, LOW);
	  PULSE_SHORT;
	  digitalWrite(_pinClock, LOW);
	  PULSE_LONG;
	  digitalWrite(_pinClock, HIGH);
	  PULSE_SHORT;
	  digitalWrite(_pinData, HIGH);
	  PULSE_SHORT;
	  digitalWrite(_pinClock, LOW);
	  PULSE_SHORT;
	  // Unnecessary here since putByte always follows startTransmission
	//  pinMode(_pinData, INPUT);
	}

	// Communication link reset
	// At least 9 SCK cycles with DATA=1, followed by transmission start sequence
	//      ______________________________________________________         ________
	// DATA:                                                      |_______|
	//          _    _    _    _    _    _    _    _    _        ___     ___
	// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
	void Camara::resetConnection(void) {
	  uint8_t i;
	  digitalWrite(_pinData, HIGH);  // Set data register high before turning on
	  pinMode(_pinData, OUTPUT);     // output driver (avoid possible low pulse)
	  PULSE_LONG;
	  for (i = 0; i < 9; i++) {
	    digitalWrite(_pinClock, HIGH);
	    PULSE_LONG;
	    digitalWrite(_pinClock, LOW);
	    PULSE_LONG;
	  }
	  startTransmission();
	}


	/******************************************************************************
	 * Helper Functions
	 ******************************************************************************/

	// Calculates temperature in degrees C from raw sensor data
	float Camara::calcTemp(uint16_t rawData) {
	  if (_stat_reg & LOW_RES)
	    return D1 + D2l * (float) rawData;
	  else
	    return D1 + D2h * (float) rawData;
	}

	// Calculates relative humidity from raw sensor data
	//   (with temperature compensation)
	float Camara::calcHumi(uint16_t rawData, float temp) {
	  float humi;
	  if (_stat_reg & LOW_RES) {
	    humi = C1 + C2l * rawData + C3l * rawData * rawData;
	    humi = (temp - 25.0) * (T1 + T2l * rawData) + humi;
	  } else {
	    humi = C1 + C2h * rawData + C3h * rawData * rawData;
	    humi = (temp - 25.0) * (T1 + T2h * rawData) + humi;
	  }
	  if (humi > 100.0) humi = 100.0;
	  if (humi < 0.1) humi = 0.1;
	  return humi;
	}


	#ifdef CRC_ENA
	// Calculate CRC for a single byte
	void Camara::calcCRC(uint8_t value, uint8_t *crc) {
	  const uint8_t POLY = 0x31;   // Polynomial: x**8 + x**5 + x**4 + 1
	  uint8_t i;
	  *crc ^= value;
	  for (i = 8; i > 0; i--) {
	    if (*crc & 0x80)
	      *crc = (*crc << 1) ^ POLY;
	    else
	      *crc = (*crc << 1);
	  }
	}

	// Bit-reverse a byte (for CRC calculations)
	uint8_t Camara::bitrev(uint8_t value) {
	  uint8_t i;
	  uint8_t result = 0;
	  for (i = 8; i > 0; i--) {
	    result = (result << 1) | (value & 0x01);
	    value >>= 1;
	  }
	  return result;
	}
	#endif

