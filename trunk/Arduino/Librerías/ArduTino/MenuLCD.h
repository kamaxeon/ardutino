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



#ifndef MenuLCD_h
#define MenuLCD_h


#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "WString.h"
#include <Menu.h>
#include "../LiquidCrystal/LiquidCrystal.h"

//# Lcd Menu 2
//# =======================
class MenuLCD
{
    private:
        LiquidCrystal * lcd;
        Menu * rootMenu;
        Menu * curMenu;     

        int cols;               // Spalten
        int rows;               // Reihen
        
        int layer_save[8];      // Speichert Cursor Position bis zur 6 Ebene
        //int layer;              // Ebenen
        int back;               // Zwischenspeicher

        int arrow_up;           // Position in DisplayCach
        int arrow_down;         // Position in DisplayCach                 
       
        int curloc;             // Aktuelle Cursor Position
        int scroll;             // Aktuelle Scroll Position

        int cursor_pos;         // Speichert die letze Cursor position
    public:
    	  int id;					  // Menu id
    	  int layer;
        String curfuncname;     // Speicher den letzten Funktionsnamen
        String funcname[5];     // Speicher die letzten Funktionsnamen, bis zur 3 Ebene
//        char * curfuncname;     // Speicher den letzten Funktionsnamen
//        char * funcname[5];     // Speicher die letzten Funktionsnamen, bis zur 3 Ebene       
        
        MenuLCD(Menu &r,LiquidCrystal &d, int row, int cols, int a_up, int a_down);

        void setCursor();       // Setz den Cursor
        void doScroll();        // Scrollt zur passenden Stelle
        void goMenu(Menu &m);   // Go to Menu m
       
        void goUp();            // Move cursor up
        void goDown();          // Move cursor down
        void goBack();          // Move to the parent Menu
        void goEnter();         // Activate the Menu under the cursor
        
        void display();         // Display the current menu on the LCD         
};
#endif // MenuLCD_h
