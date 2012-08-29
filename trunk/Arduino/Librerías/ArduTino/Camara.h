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
 * Basado la parte de los sensores en la libería SHT1X de Jonathan Oxer
 * y Maurice Ribble (también GPL)
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
    /** Obtiene la temperatura del sensor en Celsius
     *  @return Un entero con el valor 
     */
    int	obtenerTemperatura();
    /** Obtiene la humedad del sensor
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
    /** Obtiene el último valor conocido de la cámara sin obtener el
     * actual
     * @return verdadero si estaba encendida, o falso en caso contrario
     */
    bool obtenerUltimoEstado();	
    /** Comprueba el estado actual de la cámara con el último conocido
     * @return verdadero si el estado de la cámara a cambiado, o
     * falso en caso contrario
     */
    bool comprobarCambioEstado();
  private:
    int _estadoAntiguo;
    int _pinEstado;
    int _pinApagado;
    int _dataPin;
    int _clockPin;
    int _numBits;
    float readTemperatureC();
    float readTemperatureRaw();
    int shiftIn(int _dataPin, int _clockPin, int _numBits);
    void sendCommandSHT(int _command, int _dataPin, int _clockPin);
    void waitForResultSHT(int _dataPin);
    int getData16SHT(int _dataPin, int _clockPin);
    void skipCrcSHT(int _dataPin, int _clockPin);
};

#endif

