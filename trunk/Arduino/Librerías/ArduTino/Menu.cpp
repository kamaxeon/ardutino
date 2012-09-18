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

/* ================  Constructor ================ */

Menu:Menu(String nombre, int id, Menu &menuAnterior=NULL);
{
  this->_nombre     = nombre;
  this->_id         = id;
  puedeEntrar = NULL;

  // Si existe pasamos algo lo tomará como anterior, en caso contrario
  // pensará que es el primer elemento, no hay que hacer nada para ponerlo
  // a NULL el puntero ya lo está.
  if (menuAnterior)
  {
    ponerSiguiente(Menu &menuAnterior, Menu *this);
  }
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
  if (hijo)
  {
    hijo->ponerHermano(menuHijo, *this):
  }
  else
  {
    hijo=&menuHijo;
    hijo->ponerPadre(*this);
  }
}

Menu * Menu::obtenerHijo(int numeroHijo)
{
  if (hijo)
  {
    return hijo->obtenerHermano(numeroHijo);
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
  else if (hermano)
  {
    return hermano->obtenerHermano(numeroHermano-1);
  }
  else
  {
    return NULL;
  }
}

Menu * Menu::obtenerPadre()
{
  if (padre)
  {
    return parent;
  }
  else
  {
    return this;
  }
}

Menu * Menu::obtenerSiguiente()
{
  if (siguiente)
  {
    return this->siguiente;
  }
  else
  {
    return NULL;
  }
}

/* ================  Métodos Privados ================ */

void Menu::ponerPadre(Menu &menuPadre)
{
  padre = &menuPadre;
}

void Menu::ponerSiguiente(Menu &menuAnterior, Menu &menuSiguiente)
{
  // Si por casualidad ya apunta a otro lado que no sea null no hacemos
  // nada
  if ! (menuAnterior->siguiente)
  {
    menuAnterior->siguiente = &menuSiguiente;
  }
}

void Menu::ponerHermano(Menu &menuHermano, Menu &menuPadre)
{
  if (hermano)
  {
  hermano->ponerHermano(menuHermano,menuPadre);
  }
  else
 {
  hermano=&menuHermano;
  hermano->ponerPadre(&menuPadre);
 }
}
