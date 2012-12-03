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
/** @brief Clase para usar en las cámaras, cada cámara consta de un sensor,
 * un testigo para comprobar su estado, y una salida para apagado.
 *
 * Basada la parte de los sensores en la libería Sensirion de Markus Schatzl
 * y Carl Jackson (también GPL)
 *
 * @author Israel Santana <isra@miscorreos.org>
 * @date Agosto 2012

*/

#ifndef Camara_h
#define Camara_h



#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


// Copiado de Sensirion
#include <stdint.h>

// Enable CRC checking
#define CRC_ENA

// Enable ('1') or disable ('0') internal pullup on DATA line
// Commenting out this #define saves code space but leaves internal pullup
//   state undefined (ie, depends on last bit transmitted)
#define DATA_PU 1

// Clock pulse timing macros
// Lengthening these may assist communication over long wires
#define PULSE_LONG  delayMicroseconds(3)
#define PULSE_SHORT delayMicroseconds(1)

// Useful macros
#define measTemp(result)  meas(TEMP, result, BLOCK)
#define measHumi(result)  meas(HUMI, result, BLOCK)

// User constants
const uint8_t TEMP     =     0;
const uint8_t HUMI     =     1;
const bool    BLOCK    =  true;
const bool    NONBLOCK = false;

// Status register bit definitions
const uint8_t LOW_RES  =  0x01;  // 12-bit Temp / 8-bit RH (vs. 14 / 12)
const uint8_t NORELOAD =  0x02;  // No reload of calibrarion data
const uint8_t HEAT_ON  =  0x04;  // Built-in heater on
const uint8_t BATT_LOW =  0x40;  // VDD < 2.47V

// Function return code definitions
const uint8_t S_Err_NoACK  = 1;  // ACK expected but not received
const uint8_t S_Err_CRC    = 2;  // CRC failure
const uint8_t S_Err_TO     = 3;  // Timeout
const uint8_t S_Meas_Rdy   = 4;  // Measurement ready


class Camara
{
  public:
	/** Inicializa la camara, suponiendo siempre que la cámara está
	 * encendida
	 * @param pinEstado Pin que comprueba el estado de la cámara
	 * @param pinApagado Pin para lanzar el apagado de la cámara
	 * @param pinDatosSensor Pin de datos del sensor SHT1X
	 * @param pinRelojSensor Pin de reloj del sensor SHT1X
	 */
    Camara(int pinEstado, int pinApagado, int pinDatosSensor, int pinRelojSensor);
	/** Inicializa la camara, suponiendo siempre que la cámara está
	 * encendida
	 * @param pinEstado Pin que comprueba el estado de la cámara
	 * @param pinApagado Pin para lanzar el apagado de la cámara
	 * @param pinDatosSensor Pin de datos del sensor SHT1X
	 * @param pinRelojSensor Pin de reloj del sensor SHT1X
	 */
    Camara(int pinEstado, int pinApagado, int pinDatosSensor, int pinRelojSensor, int id);
    /** Obtiene la temperatura del sensor en Celsius
     *  @return Un entero con el valor 
     */
    int	obtenerTemperatura();
    /** Obtiene la temperatura del sensor
     * @return Un entero con el valor
     */
    int	obtenerHumedad();   
    /** Activa el interruptor de apagado de la cámara durante 3 segundos
     */
    void apagar();
    /** Obtiene el estado actual de la cámara
     * @return verdadero en caso de estar encendida, o falso en caso
     * contrario
     */
    bool obtenerEstado();
    /** Comprueba el estado actual de la cámara con el último conocido
     * @return verdadero si el estado de la cámara a cambiado, o
     * falso en caso contrario
     */
    bool comprobarCambioEstado();
    int humedad;
    int temperatura;
    int estado;
    int id;
  private:
    int _pinEstado;
    int _pinApagado;
		uint8_t _pinData;
		uint8_t _pinClock;
		uint16_t *_presult;
		uint8_t _stat_reg;
		#ifdef CRC_ENA
		uint8_t _crc;
		#endif
		uint8_t getResult(uint16_t *result);
		uint8_t putByte(uint8_t value);
		uint8_t getByte(bool ack);
		void startTransmission(void);
		void resetConnection(void);
		#ifdef CRC_ENA
		void calcCRC(uint8_t value, uint8_t *crc);
		uint8_t bitrev(uint8_t value);
		#endif
		uint16_t _rawData;
    uint8_t meas(uint8_t cmd, uint16_t *result, bool block);
    float calcTemp(uint16_t rawData);
    float calcHumi(uint16_t rawData, float temp);
};

#endif

