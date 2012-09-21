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
/** @brief Clase para usar administrar los menus del display
 *
 *
 * @author Israel Santana <isra@miscorreos.org>
 * @date Septiembre 2012
*/



#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include "Menu.h"
#include <WString.h>

/* ================  Constructor ================ */

Menu::Menu(String nombre, int id)
{
	this->_nombre			= nombre;
	this->_id					= id;
	puedeEntrar				= NULL;
}

Menu::Menu(String nombre, int id, Menu &menuInicial)
{
	this->_nombre			= nombre;
	this->_id					= id;
	puedeEntrar				= NULL;
	ponerAnterior(menuInicial);
}

Menu::Menu(String nombre, int id, Menu &menuInicial, Menu &menuPadre)
{
	this->_nombre			= nombre;
	this->_id					= id;
	puedeEntrar				= NULL;
	ponerAnterior(menuInicial);
	menuPadre.ponerHijo(*this);
}
Menu::Menu(String nombre,boolean (*c)(Menu&))
{
  this->_nombre=nombre;
  puedeEntrar=c;
}
/* ================  Métodos Públicos ================ */

void Menu::ponerNombre(String nuevoNombre)
{
  this->_nombre = nuevoNombre;
}

String Menu::obtenerNombre()
{
  return this->_nombre;
}

int Menu::obtenerId()
{
  return this->_id;
}

void Menu::ponerHijo(Menu &menuHijo)
{
  if (this->_hijo)
  {
		this->_hijo->ponerHermano(menuHijo, *this);
  }
  else
  {
    this->_hijo=&menuHijo;
    this->_hijo->ponerPadre(*this);
  }
}

Menu * Menu::obtenerHijo(int numeroHijo)
{
  if (this->_hijo)
  {
    return this->_hijo->obtenerHermano(numeroHijo);
  }
  else
  {
    return NULL;
  }
}

Menu * Menu::obtenerHermano(int numeroHermano)
{
  if (numeroHermano == 0)
  {
    return this;
  }
  else if (this->_hermano)
  {
    return this->_hermano->obtenerHermano(numeroHermano-1);
  }
  else
  {
    return NULL;
  }
}

Menu * Menu::obtenerPadre()
{
  if (this->_padre)
  {
    return this->_padre;
  }
  else
  {
    return this;
  }
}

Menu * Menu::obtenerSiguiente()
{
  if (this->_siguiente)
  {
    return this->_siguiente;
  }
  else
  {
    return NULL;
  }
}

String Menu::obtenerNombrePorId(int id)
{
	Menu * menuAux;
	menuAux = this;
	while (menuAux != NULL)
	{
		if (menuAux->_id == id)
		{
			return menuAux->_nombre;
		}
		menuAux=menuAux->_siguiente;
	}
	
	return ""; // En el caso que no encuentre nada
}

void Menu::ponerNombrePorId(int id, String nuevoNombre)
{
	Menu * menuAux;
	menuAux = this;
	while (menuAux != NULL)
	{
		if (menuAux->_id == id)
		{
			Serial.println("hola");
			menuAux->_nombre = nuevoNombre;
			Serial.println(menuAux->_nombre);
			break;
		}
		menuAux=menuAux->_siguiente;
	}
}
/* ================  Métodos Privados ================ */

void Menu::ponerPadre(Menu &menuPadre)
{
  this->_padre = &menuPadre;
}

void Menu::ponerAnterior(Menu &menuAnterior)
{
	Menu * menuAux;
	menuAux = &menuAnterior;
	while (menuAux->_siguiente != NULL)
	{
		menuAux=menuAux->_siguiente;
	}
    menuAux->_siguiente = this;
}

// void Menu::ponerSiguiente(Menu &)
void Menu::ponerHermano(Menu &menuHermano, Menu &menuPadre)
{
  if (this->_hermano)
  {
		this->_hermano->ponerHermano(menuHermano,menuPadre);
  }
  else
 {
  	this->_hermano=&menuHermano;
  	ponerPadre(menuPadre);
 }
}
