// Librería MenuLCD
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
// Basada en LCDMenu2 de Jomelo, básicamente es una adapación al
// y adapatada al Ide de arduino > 1.0

/** @brief Clase para mostrar los menús en el LCD
 *
 *
 * @author Israel Santana <isra@miscorreos.org>
 * @date Septiembre 2012
*/

#include "MenuLCD.h"


MenuLCD::MenuLCD(Menu &r,LiquidCrystal &d, int ro, int co, int a_up, int a_down)
{
    rootMenu        = &r;
    curMenu         = rootMenu;
    lcd             = &d;
    back            = 0;
    cols            = co;
    rows            = ro;
    arrow_up        = a_up;
    arrow_down      = a_down;
    cursor_pos      = 0;
    layer           = 0;
    layer_save[0]   = 0;
}

void MenuLCD::setCursor()
{
    if(cursor_pos > curloc-scroll) {
        lcd->setCursor(0,cursor_pos);
        lcd->write(0x20);
    }
    else if(cursor_pos < curloc-scroll) {
       lcd->setCursor(0,curloc-scroll-1);
       lcd->write(0x20);
    }
    cursor_pos = curloc-scroll;

    lcd->setCursor(0,curloc-scroll);
    lcd->write(0x7E);
}

void MenuLCD::doScroll()
{
    if (curloc<0) {
        curloc=0;
    }
    else {
        while (curloc>0&&!curMenu->obtenerHijo(curloc))//Only allow it to go up to Menu item (one more if back button enabled)
        {
            curloc--;
        }
    }

    if (curloc>=(rows+scroll)) {
        scroll++;
        display();
    }
    else if (curloc<(scroll)) {
        scroll--;
        display();
    }
    else {
        setCursor();
    }
}

void MenuLCD::goUp()
{
    curloc-=1;
    doScroll();
}

void MenuLCD::goDown()
{
    curloc+=1;
    doScroll();
}

void MenuLCD::goBack()
{
    if(layer > 0) {
        back = 1;
        goMenu(*curMenu->obtenerPadre());
    }

}

void MenuLCD::goEnter()
{
    Menu *tmp;
    tmp=curMenu;
    if ((tmp=tmp->obtenerHijo(curloc))) {//The child exists
        if (tmp->puedeEntrar) {//canEnter function is set
            if (tmp->puedeEntrar(*tmp)) {//It wants us to enter
                goMenu(*tmp);
            }
        }
        else {//canEnter function not set, assume entry allowed
            goMenu(*tmp);
            curfuncname = tmp->obtenerNombre();
            id = tmp->obtenerId();
        }
    }
    else {//Child did not exist  The only time this should happen is one the back Menu item, so go back
        goBack();
    }
    
}



void MenuLCD::goMenu(Menu &m)
{
    curMenu=&m;

    if(layer < 8) {
        int diff;
        scroll = 0;

        if(back == 0) {
            layer_save[layer] = curloc;
            layer++;
            curloc = 0;
        } else {
            back = 0;

            if(layer > 0) {
                layer--;
                curloc = layer_save[layer];

                if(curloc >= rows) {
                    diff = curloc-(rows-1);
                    for(int i=0; i<diff; i++) {
                        doScroll();
                    }
                }
            }
        }
    }


    if(layer >= 0 && layer <5) {
      funcname[layer-1] = curMenu->obtenerNombre();
    }


    display();
}

void MenuLCD::display()
{
    Menu * tmp;
    int i = scroll;
    int j = 0;
    int maxi=(rows+scroll);

    lcd->clear();

    // Anzahl ermitteln
    if ((tmp=curMenu->obtenerHijo(0))) {
        do {
            j++;
        } while ((tmp=tmp->obtenerHermano(1)));
    }
    j--; // Korrektur

    if ((tmp=curMenu->obtenerHijo(i))) {
        do {
            lcd->setCursor(0,i-scroll);
            lcd->write(0x20);
            lcd->print(tmp->obtenerNombre());
            i++;

        } while ((tmp=tmp->obtenerHermano(1))&&i<maxi);


        // edit 11.09.2009
        if(j>(rows-1)) {
          // edit end

            if(curloc == 0) {
                lcd->setCursor((cols-1),(rows-1));
                lcd->write(arrow_down);
            }
            else if(curloc == j) {
                lcd->setCursor((cols-1),0);
                lcd->write(arrow_up);
            }
            else {
                lcd->setCursor((cols-1),0);
                lcd->write(arrow_up);

                lcd->setCursor((cols-1),(rows-1));
                lcd->write(arrow_down);
            }
        }
    }
    else { // no children
        goBack();
        // function can work
    }
    setCursor();
}
