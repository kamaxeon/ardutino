
// // Librería Teclado
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

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "Teclado.h"


/* ================  Constructor ================ */

Teclado::Teclado(int pinKeypad, int valoresKeypad[], int numeroTeclas)
{
	int i;
	this->_pinKeypad	      = pinKeypad;
  this->_numeroTeclas     = numeroTeclas;
  // Pongo un valor alto para que pueda recoger la primera pulsación
  this->_ultimaTecla      = 101 ;
  // this->_valoresKeypad    = new int[numeroTeclas];
  // int *_cursor = this->_valoresKeypad;
	//   for (i = 0; i < numeroTeclas ; i++)
	// {
	// 	*_cursor = *(valoresKeypad + i);
	//     _cursor++;
	// }
  for (i = 0; i < numeroTeclas ; i++)
 {
	 this->_valoresKeypad[i] = *(valoresKeypad + i);
 }
  

  
	
}

/* ================  Métodos Públicos ================ */



int Teclado::obtenerUltimaTecla()
{
  return this->_ultimaTecla;
}

bool Teclado::comprobarPulsacionNueva()
{
	// Compruebo si hay una tecla pulsada primero
	if (comprobarPulsacion())
	{
		int nuevaTecla;
		int auxTecla;

		auxTecla = obtenerUltimaTecla();
		nuevaTecla = obtenerTeclaActual();

		// Compruebo si ha cambiado la tecla
		if (nuevaTecla != auxTecla)
		{
			// Espero un tiempo prudencial y vuelvo a probar
			delay(10);
			nuevaTecla = obtenerTeclaActual();
			if ( nuevaTecla != auxTecla)
			{
				if ( nuevaTecla >=0)
				{
					return true;
				}
			}
		}
		else
		{
			return false;  
		}
	}
	else
	{
		// Para liberar la posición de última tecla cuando se libera
		this->_ultimaTecla = 101;
		return false;
	}
}




/* ================  Métodos Privados ================ */

bool Teclado::comprobarPulsacion()
{
	if (obtenerADC() < 10)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int Teclado::obtenerADC()
{
  unsigned int valorAnalogico; // Defino una variable para la lectura
  valorAnalogico = analogRead(_pinKeypad); // Recojo la lectura del pin
  return valorAnalogico;
}

int Teclado::obtenerTeclaActual()
{
  unsigned int valorAnalogico; // Defino una variable para la lectura
  valorAnalogico = analogRead(_pinKeypad); // Recojo la lectura del pin

  // Devuelvo el valor convirtiendo de analógico a la posición del vector
  this->_ultimaTecla = convertirADC(valorAnalogico);
  return this->_ultimaTecla;
}



int Teclado::convertirADC(int valorAnalogico)
{
  int i;
  for (i = 0 ; i < this->_numeroTeclas; i++)
  {
    if (valorAnalogico < _valoresKeypad[i])
    {
      return i;
    }
  }

  if (i >= this->_numeroTeclas)
  {
    return -1; // No pulsación no ha sido correcta
  }
}
