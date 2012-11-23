// Librería Teclado
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
/** @brief Clase para usar el keypad del panel de control del secadero
 *
 *
 * @author Israel Santana <isra@miscorreos.org>
 * @date Agosto 2012
*/

#ifndef Teclado_h
#define Teclado_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class Teclado
{
	public:
		/** Inicializa el keypad del ArduTino que controla los secadero
		 * @param pinKeypad
     * @param valoresKeypad, es un vector con los valores que devuelve
     * el ping dependiendo de la tecla pulsada
     * @param numeroTeclas el número de teclas que tiene el keypad
     * @warning Ningún valor del Keypad analógico puede ser superior a
     * 1024
		 */
		Teclado(int pinKeypad, int valoresKyepad[], int numeroTeclas);
		/** Inicializa el keypad del ArduTino que controla los secadero
		 * @param pinKeypad
     * @param valoresKeypad, es un vector con los valores que devuelve
     * el ping dependiendo de la tecla pulsada
     * @param numeroTeclas el número de teclas que tiene el keypad
     * @param noTeclaAbierto, el un valor cuando no pulsamos una tecla, suele
     * ser 0, cuando actuan todas las resistencias o 1024 cuando no actua ninguna
     * @warning Ningún valor del Keypad analógico puede ser superior a
     * 1024
		 */
		Teclado(int pinKeypad, int valoresKyepad[], int numeroTeclas, bool noTeclaAbierto);

		/** Obtiene la última tecla pulsada sin obtener la actual
		 * @return un entero con el valor
		 */
		int obtenerUltimaTecla();

		/** Obtiene si hay diferencias entre la última tecla pulsada
		 * y la actual
		 * @return verdadero si hay diferencias, o falso en caso contrario
		 */
		bool comprobarPulsacionNueva();

		

	
	private:
		int convertirADC(int valorAnalogico);
		int obtenerADC();
		bool comprobarPulsacion();
    bool _noTeclaAbierto;
		int _pinKeypad;
		int _ultimaTecla;
    int _numeroTeclas;
    //int * _valoresKeypad;
    int _valoresKeypad[20]; //Máximo numero de teclas admitidas
    int obtenerTeclaActual();
    //~ int convertirADC(unsigned int valorAnalogico);
};



#endif
