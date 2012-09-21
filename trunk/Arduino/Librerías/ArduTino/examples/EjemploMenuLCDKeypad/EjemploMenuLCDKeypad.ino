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
 * En este ejemplo se prueban todas las funcionalidades que presenta
 * la librería:
 * 
 * Se ha usado un shield LCDKeypad de DFROBOT
 * http://www.robotshop.com/dfrobot-lcd-keypad-shield-arduino-1.html
 */

// Librerías a usar
#include <LiquidCrystal.h>
#include <Menu.h>
#include <MenuLCD.h>
#include <Teclado.h>


// Definiciones propias del Keypad
const int teclaDerecha		= 0;
const int teclaArriba			=	1;
const int teclaAbajo			=	2;
const int teclaIzquierda	=	3;
const int teclaEnter			= 4;
// Pin de lectura

const int pinKeypad = 0;

// Valores del teclado, siempre se debería poner un número algo mayor del
// que nos devuelva el keypad
int valoresAnalogicos[] = {50, 195, 380, 550, 790};

// Relación teclas nombre
String Teclas[] = {"Derecha", "Arriba", "Abajo", "Izquierda", "Enter"};

// Definimos el número de teclas, también podemos poner el número
// directamente
int numeroTeclas = sizeof(valoresAnalogicos)/sizeof(int);

// Aquí definimos si cuando no pulsamos ninguna tecla da 1024 ó 0
// Verdadero cuando es 1024, falso cuando es 0
boolean sinPulsarAbierto = true;

// Definimos el teclado en sí
Teclado teclado(pinKeypad, valoresAnalogicos, numeroTeclas, sinPulsarAbierto);

// Valor índice del vector de resultados
int i;
// Nombre de la tecla
String teclaNombre;

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


void tecla(int tecla)
{
		    if (tecla == teclaArriba) 
		{
		      Root.goUp();
		      Serial.println("'Boton Arriba'");
		    }
		    else if(tecla = teclaAbajo) {
		      Root.goDown();
		      Serial.println("'Boton Abajo'");
		    }
		    else if(tecla == teclaEnter) {
		      Root.goEnter();
			pulsacion_enter = true;
		      Serial.println("'Boton Enter'");
		    }
		    else if(tecla == teclaIzquierda) {
		      Root.goBack();
		      Serial.println("'Tecla Izquierda'");
		    }
		    else if(tecla == teclaDerecha) {
		      Root.goEnter();
			pulsacion_enter = true;
		      Serial.println("'Tecla Derecha'");
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
  lcd.createChar(0, arrow_up);    
  lcd.createChar(1, arrow_down); 
	Root.display();
  // Inicalizamos el puerto de consola
  Serial.begin(9600);
  Serial.println("Empezando el ejemplo:");
  Serial.println("================");
}


void loop()
{

  if (teclado.comprobarPulsacionNueva())
  {
		i = teclado.obtenerUltimaTecla();
		Serial.println(analogRead(0));
		Serial.println(i);
		tecla(i);
		comprobarFuncion();
	}

}
