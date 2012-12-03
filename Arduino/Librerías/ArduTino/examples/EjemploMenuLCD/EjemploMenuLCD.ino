/**
 * EjemploMenuLCD
 *
 * Copyright 2012 Israel Santana <isra@miscorreos.org>
 * 
 * Este ejemplo se ha hecho con un Arduino Mega, pero debería funcionar
 * sin problemas en otros Arduino. La versión usada es del software de
 * Arduino es la 1.0.1, debería funcionar con cualquier versión 1.x,
 * en versiones anteriores puede haber alguna modificación.
 * 
 * 
 * En este ejemplo se prueban todas las funcionalidades que presenta
 * la librería:
 * 
 * Se ha usado un LCD de 16x2 para este ejemplo
 * 
 * El uso del ejemplo es para usar el arduino conectado al serial con el 
 * ordenador, con la siguiente correspondencía de tecla valor
 * - w -> Arriba
 * - s -> Abajo
 * - q -> Atrás
 * - e -> Enter
 */

// Librerías a usar
#include <LiquidCrystal.h>
#include <Menu.h>
#include <MenuLCD.h>


// Valores del LCD
const int _LCD_rows	=	2;
const int _LCD_cols	=	16;





//# Caracteres propios
//# ==================================
uint8_t arrow_up[8]    = {0x4,0xE,0x15,0x4,0x4,0x4,0x0};
uint8_t arrow_down[8]  = {0x0,0x4,0x4,0x4,0x15,0xE,0x4};

// Creamos el objeto del display
LiquidCrystal lcd(8,9,4,5,6,7); 

// LLamamos al menú principal
// Para definirlo sólo debemos pasarle como parámetros el nombre y un id
Menu top("Root",0);

// // menu, lcd, rows, cols, arrow_up_pos, arrow_down_pos
MenuLCD Root(top, lcd , _LCD_rows, _LCD_cols, 0, 1);


// Pulsacion de enter
boolean pulsacion_enter = false;

// Menu
// La creación de cualquier menú que no sea el raíz debemos pasarle los siguientes
// parámetros nombre, id(único), el menú raíz, y su menú padre (el predecedor)
Menu Item1("Menu1->Item1", 1, top, top);
Menu Item2("Menu1->Item2", 2, top, top);
Menu Item3("Menu1->Item3", 3, top, top);
Menu Item4("Menu1->Item4", 4, top, top);
Menu Item5("Menu1->Item5", 5, top, top);
Menu Item6("Menu1->Item6", 6, top, top);
Menu Item7("Menu1->Item7", 7, top, top);
Menu Item8("Menu1->Item8", 8, top, top);
Menu Item9("Menu1->Item9", 9, top, top);

Menu Item11("Menu1.1->Item1", 11, top, Item1);
Menu Item12("Menu1.1->Item2", 12, top, Item1);
Menu Item13("Menu1.1->Item3", 13, top, Item1);
Menu Item14("Menu1.1->Item4", 14, top, Item1);
Menu Item15("Menu1.1->Item5", 15, top, Item1);
Menu Item16("Menu1.1->Item6", 16, top, Item1);

Menu Item21("Menu1.2->Item1", 21, top, Item2);
Menu Item22("Menu1.2I->tem2", 22, top, Item2);

Menu Item31("Menu1.3->Item1", 31, top, Item3);
Menu Item32("Menu1.3->Item2", 32, top, Item3);


void tecla(char select='z')
{
    if(select == 'w') {
      Root.goUp();
      Serial.println("'Tecla w -> Arriba'");
    }
    else if(select == 's') {
      Root.goDown();
      Serial.println("'Tecla s -> Abajo'");
    }
    else if(select == 'e') {
      Root.goEnter();
			pulsacion_enter = true;
      Serial.println("'Tecla e -> Enter'");
    }
    else if(select == 'q') {
      Root.goBack();
      Serial.println("'Tecla q ->Atras'");
    }
}

void comprobarTecla()
{
    if(Serial.available()) {
        char c = Serial.read();
        tecla(c);
    } 
}


void comprobarFuncion()
{
	if(pulsacion_enter) {
		pulsacion_enter = false;
		Serial.print("Ha pulsado enter dentro del menu: ");
		Serial.println(top.obtenerNombrePorId(Root.id));        
    }
}



void setup()
{
	// Inicializamos el display
  lcd.begin(_LCD_rows,_LCD_cols);
  // Creamos los caracteres especiales
  lcd.createChar(0,       arrow_up);    
  lcd.createChar(1,       arrow_down); 
	Root.display();
  // Inicalizamos el puerto de consola
  Serial.begin(9600);
  Serial.println("Empezando el ejemplo:");
  Serial.println("================");
}


void loop()
{
  comprobarTecla();
  comprobarFuncion();
}
