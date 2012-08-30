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

#define length(_valoresKeypad) (sizeof(_valoresKeypad)/sizeof( *_valoresKeypad))

#include "Teclado.h"


/* ================  Constructor ================ */

Teclado::Teclado(int pinKeypad)
{
	
	_pinKeypad	      = pinKeypad;
  _valoresKeypad[]  = valoresKeypad[];
  _numeroTeclas     = sizeof _valoresKeypad/sizeof valoresKeypad[0];
  // Pongo un valor alto para que pueda recoger la primera pulsación
  _ultimaTecla      = 99 ;
  
	
}

/* ================  Métodos Públicos ================ */


int obtenerUltimaTecla()
{
  return _ultimaTecla;
}

bool comprobarPulsacionNueva()
{
  int nuevaTecla;
  int auxTecla;

  auxTecla = _obtenerUltimaTecla();
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


/* ================  Métodos Privados ================ */

int obtenerTeclaActual()
{
  unsigned int valorAnalogico; // Defino una variable para la lectura
  valorAnalogico = analogRead(_pinKeypad); // Recojo la lectura del pin

  // Devuelvo el valor convirtiendo de analógico a la posición del vector
  _ultimaTecla = convertirADC(unsigned int valorAnalogico);
  return _ultimaTecla;
}

int convertirADC(unsigned int valorAnalogico)
{
  int i;
  for (i = 0 ; i < _numeroTeclas; i++)
  {
    if (valorAnalogico < valoresKeypad[i])
    {
      return i;
    }
  }

  if (i >= _numeroTeclas)
  {
    return -1; // No pulsación no ha sido correcta
  }
}
