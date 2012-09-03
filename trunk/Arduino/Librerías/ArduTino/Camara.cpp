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


/* ================  Constructor ================ */

Camara::Camara(int pinEstado, int pinApagado, int pinDatosSensor, int pinRelojSensor)
{
	// El estado de la camara al iniciarse siempre es true
	this->_estadoAntiguo = true;
	
	this->_pinEstado	= pinEstado;
	this->_pinApagado	=	pinApagado;
	this->_dataPin 		= pinDatosSensor;
	this->_clockPin 	= pinRelojSensor;
	
}

/* ================  Métodos Públicos ================ */


int Camara::obtenerTemperatura()
{

  return (int)round(readTemperatureC());
}


int Camara::obtenerHumedad()
{
  int _val; // Raw humidity value returned from sensor
  float _linearHumidity; // Humidity with linear correction applied
  float _correctedHumidity; // Temperature-corrected humidity
  float _temperature; // Raw temperature value

  // Conversion coefficients from SHT15 datasheet
  const float C1 = -4.0; // for 12 Bit
  const float C2 = 0.0405; // for 12 Bit
  const float C3 = -0.0000028; // for 12 Bit
  const float T1 = 0.01; // for 14 Bit @ 5V
  const float T2 = 0.00008; // for 14 Bit @ 5V

  // Command to send to the SHT1x to request humidity
  int _gHumidCmd = 0b00000101;

  // Fetch the value from the sensor
  sendCommandSHT(_gHumidCmd, this->_dataPin, this->_clockPin);
  waitForResultSHT(this->_dataPin);
  _val = getData16SHT(this->_dataPin, this->_clockPin);
  skipCrcSHT(this->_dataPin, this->_clockPin);

  // Apply linear conversion to raw value
  _linearHumidity = C1 + C2 * _val + C3 * _val * _val;

  // Get current temperature for humidity correction
  _temperature = readTemperatureC();

  // Correct humidity value for current temperature
  _correctedHumidity = (_temperature - 25.0 ) * (T1 + T2 * _val) + _linearHumidity;

  return (int)round((_correctedHumidity));
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
		return true; // la cámara está encendida
    this->_estadoAntiguo = true; // cambiamos el estado
	}
	else
	{
		return false; // la cámara está apagada
    this->_estadoAntiguo = false; // cambiamos el estado
	}

}


bool Camara::obtenerUltimoEstado()
{
	return this->_estadoAntiguo;
}

bool Camara::comprobarCambioEstado()
{
	bool estadoNuevo; // Variable para comparar
  bool estadoAux;
  estadoAux = obtenerUltimoEstado();

	estadoNuevo = obtenerEstado();

	if (estadoNuevo != estadoAux)
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
 * Copiado directamente del original SHT1X
 */
 
float Camara::readTemperatureC()
{
  int _val; // Raw value returned from sensor
  float _temperature; // Temperature derived from raw value

  // Conversion coefficients from SHT15 datasheet
  const float D1 = -40.0; // for 14 Bit @ 5V
  const float D2 = 0.01; // for 14 Bit DEGC

  // Fetch raw value
  _val = readTemperatureRaw();

  // Convert raw value to degrees Celsius
  _temperature = (_val * D2) + D1;

  return (_temperature);
}

/**
* Reads the current raw temperature value
*/
float Camara::readTemperatureRaw()
{
  int _val;

  // Command to send to the SHT1x to request Temperature
  int _gTempCmd = 0b00000011;

  sendCommandSHT(_gTempCmd, this->_dataPin, this->_clockPin);
  waitForResultSHT(this->_dataPin);
  _val = getData16SHT(this->_dataPin, this->_clockPin);
  skipCrcSHT(this->_dataPin, this->_clockPin);

  return (_val);
}

/**
*/
int Camara::shiftIn(int _dataPin, int _clockPin, int _numBits)
{
  int ret = 0;
  int i;

  for (i=0; i<_numBits; ++i)
  {
     digitalWrite(_clockPin, HIGH);
     delay(10); // I don't know why I need this, but without it I don't get my 8 lsb of temp
     ret = ret*2 + digitalRead(_dataPin);
     digitalWrite(_clockPin, LOW);
  }

  return(ret);
}

/**
*/
void Camara::sendCommandSHT(int _command, int _dataPin, int _clockPin)
{
  int ack;

  // Transmission Start
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clockPin, LOW);

  // The command (3 msb are address and must be 000, and last 5 bits are command)
  shiftOut(_dataPin, _clockPin, MSBFIRST, _command);

  // Verify we get the correct ack
  digitalWrite(_clockPin, HIGH);
  pinMode(_dataPin, INPUT);
  ack = digitalRead(_dataPin);
  if (ack != LOW) {
    //Serial.println("Ack Error 0");
  }
  digitalWrite(_clockPin, LOW);
  ack = digitalRead(_dataPin);
  if (ack != HIGH) {
    //Serial.println("Ack Error 1");
  }
}

/**
*/
void Camara::waitForResultSHT(int _dataPin)
{
  int i;
  int ack;

  pinMode(_dataPin, INPUT);

  for(i= 0; i < 100; ++i)
  {
    delay(10);
    ack = digitalRead(_dataPin);

    if (ack == LOW) {
      break;
    }
  }

  if (ack == HIGH) {
    //Serial.println("Ack Error 2"); // Can't do serial stuff here, need another way of reporting errors
  }
}

/**
*/
int Camara::getData16SHT(int _dataPin, int _clockPin)
{
  int val;

  // Get the most significant bits
  pinMode(_dataPin, INPUT);
  pinMode(_clockPin, OUTPUT);
  val = shiftIn(_dataPin, _clockPin, 8);
  val *= 256;

  // Send the required ack
  pinMode(_dataPin, OUTPUT);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);

  // Get the least significant bits
  pinMode(_dataPin, INPUT);
  val |= shiftIn(_dataPin, _clockPin, 8);

  return val;
}

/**
*/
void Camara::skipCrcSHT(int _dataPin, int _clockPin)
{
  // Skip acknowledge to end trans (no CRC)
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);

  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);
}
