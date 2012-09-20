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

#ifndef Menu_h
#define Menu_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <WString.h>

class Menu
{
  public:
   /** Constructor del menu, si no se usa con un menu anterior.
    * Este constructor se debe usar cuando es el primer menu, el menu top
    * @param nombre, es el texto que se muestra en el display
    * @param id, es el identificador que se usa para localizar el menu
    * por un id
    */
    Menu (String nombre, int id);


    /** Constructor del menu, usado cuando se quiere crear una lista
     * entre todos los elementos del menú
     * @param nombre, es el texto que se muestra en el display
     * @param id, es el identificador que se usa para localizar el menu
     * por un id
     * @param menuInicial, es el menu primero, a partir de ahí se llega al
	 * último y se enlaza con este en una lista enlazada
     */
    Menu (String nombre,int id, Menu &menuIncial);

    /** Constructor del menu, usado cuando se quiere crear una lista
     * entre todos los elementos del menú
     * @param nombre, es el texto que se muestra en el display
     * @param id, es el identificador que se usa para localizar el menu
     * por un id
     * @param menuInicial, es el menu primero, a partir de ahí se llega al
	 * último y se enlaza con este en una lista enlazada
     * @param menuPadre es el menu padre del menu que estamos creando
     */
    Menu (String nombre,int id, Menu &menuIncial, Menu &menuPadre);


    /** Contrucye el menú con una función determinada
     * @param nombre, es el texto que se muestra en el display
     * @param *c llamada a la función del menú
     */
    Menu (String nombre, boolean (*c)(Menu&));
    /** Función que modifica el texto a mostrar en el LCD de un elemento
     * del menú
     * @param nuevoNombre, es el nuevo texto que se mostrará en el display
     */ 
    void ponerNombre(String nuevoNombre);


    /** Función que devuelve el texto a mostrar del menú
     * @return un texto con el nombre a mostrar en el menú
     */
    String obtenerNombre();

    /** Función que devuelve el texto a mostrar del menú
     * @return id del menú
     */
    int obtenerId();
    
    /** Función que se llamada cuando se usa el menú, funcion tipo puntero
      * @param Menu, es el menu al que queremos acceder
      * @return verdadero si se puede acceder o falso en caso contrario
      */    
    boolean (*puedeEntrar)(Menu&);

     /** Función para añadir como hijo al menú, en caso que el menú
       * actual ya tenga un hijo, se le preguntará al hijo de ponerlo
       * como hermano
       * @param menuHijo, el menú que se usará como menú hijo
       */
    void ponerHijo(Menu &menuHijo);

    /** Función que devuelve un puntero a un menú
     * @param numeroHijo, es el menú hijo, n siendo n
     * numeroHijo
     * @return puntero al menú solicitado o NULL en caso de no existir
     */
    Menu * obtenerHijo(int numeroHijo);

    /** Funcion que 
    /** Función que devuelve un puntero a un menú hermano
     * @param numeroHermano, es el número de hermano que se pretende obtener
     * @return puntero al menú solicitado o NULL en caso de no existir
     */
    Menu * obtenerHermano(int numeroHermano);

    /** Función que devuelve un puntero al padre del menú
     * @return puntero al menú solicitado o NULL en caso de no existir
     */
    Menu * obtenerPadre();
    
    /** Función que devuelve el menú siguiente la lista enlazada
     * @return puntero al siguiente menú
     */
    Menu * obtenerSiguiente();

	String obtenerNombrePorId(int id);
	
	void ponerNombrePorId(int id, String nuevoNombre);

  
  private:
    String _nombre; // El texto a mostrar en el LCD
    int _id; // El identificador del menú
    Menu * _padre; // Menú padre, NULL si estamos en el primero (top)
    Menu * _hijo; // Primer menú hijo, NULL si no existen hijo
    Menu * _hermano; // Siguiente hermano, MULL si es el último hermano
    Menu * _siguiente; // Menú siguiente en una lista simple enlazada

    /** Función que asigna un padre al menú
     * @param &menuPadre, es el menú que se pondrá como padre
     */
    void ponerPadre(Menu &menuPadre);


    /** Función que asigna un menú hermano con un padre en común. En
     * caso de tener ya un hermano, le pregunta para añadirlo.
     */
    void ponerHermano(Menu &menuHermano, Menu &menuPadre);
    
    /** Función que asigna el siguiente al menú anterior
     * @param menuAnterior, menú anterior que debe ser siguiente=NULL
     * @param menuSiguiente, menú que pasará a ser el último
     */
    void ponerAnterior(Menu &menuAnterior);
};


#endif //Menu_h
