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

/*
 * Macro para saber la longitud del vector de valores analogicos 
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
	
	this->_pinKeypad	      = pinKeypad;
  this->_numeroTeclas     = numeroTeclas;
  for (int i = 0; i < this->_numeroTeclas ; i++)
	{
		this->_valoresKeypad[i] = valoresKeypad[i];
	}
	
  // Pongo un valor alto para que pueda recoger la primera pulsación
  this->_ultimaTecla      = 99 ;
  
	
}

/* ================  Métodos Públicos ================ */


int Teclado::obtenerUltimaTecla()
{
  return this->_ultimaTecla;
}

bool Teclado::comprobarPulsacionNueva()
{
  int nuevaTecla;
  int auxTecla;

  auxTecla = obtenerUltimaTecla();
  nuevaTecla = obtenerTeclaActual();

  // Compruebo si ha cambiado la tecla
  if (nuevaTecla != auxTecla)
  {
    // Espero un tiempo prudencial y vuelvo a probar
    delay(50);
    nuevaTecla = obtenerTeclaActual();
    delay(50);
    if ( nuevaTecla != auxTecla)
    {
      if ( nuevaTecla >=0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;  
  }
}


int Teclado::obtenerTest()
{
	return obtenerTeclaActual();
}

int devolverADC()
{
  unsigned int valorAnalogico; // Defino una variable para la lectura
  valorAnalogico = analogRead(_pinKeypad); // Recojo la lectura del pin
  return valoresAnalogico;
}
/* ================  Métodos Privados ================ */

int Teclado::obtenerTeclaActual()
{
  unsigned int valorAnalogico; // Defino una variable para la lectura
  valorAnalogico = analogRead(_pinKeypad); // Recojo la lectura del pin

  // Devuelvo el valor convirtiendo de analógico a la posición del vector
  this->_ultimaTecla = convertirADC(valorAnalogico);
  return this->_ultimaTecla;
}

int Teclado::convertirADC(unsigned int valorAnalogico)
{
  int i;
  for (i = 0 ; i < this->_numeroTeclas; i++)
  {
    if (valorAnalogico < this->_valoresKeypad[i])
    {
      return i;
    }
  }

  if (i >= this->_numeroTeclas)
  {
    return -1; // No pulsación no ha sido correcta
  }
}
