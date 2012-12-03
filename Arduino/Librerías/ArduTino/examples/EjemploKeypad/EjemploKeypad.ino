/**
 * EjemploKeypad
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
 * Ejemplo usando un ADKeyboard de Seeed Studio
 * http://www.seeedstudio.com/depot/electronic-brick-adkey-module-p-500.html
 * 
 */

// Incluímos la librería
#include <Teclado.h>

// Pin de lectura
const int pinKeypad = 0;

// Valores del teclado, siempre se debería poner un número algo mayor del
// que nos devuelva el keypad
int valoresAnalogicos[] = {200, 450, 800, 970, 1100};

// Relación teclas nombre
String Teclas[] = {"Intro", "Derecha", "Arriba", "Abajo", "Izquierda"};

// Definimos el número de teclas, también podemos poner el número
// directamente
int numeroTeclas = sizeof(valoresAnalogicos)/sizeof(int);

// Definimos el teclado en sí
Teclado teclado(pinKeypad, valoresAnalogicos, numeroTeclas);

// Valor índice del vector de resultados
int i;
// Nombre de la tecla
String teclaNombre;

void setup()
{
  Serial.begin(9600); // Definimos el puerto serial
	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
}

void loop()
{
  if (teclado.comprobarPulsacionNueva())
  {
		i = teclado.obtenerUltimaTecla();
		teclaNombre = Teclas[i];
		Serial.print("La tecla pulsada es ");
		Serial.println(teclaNombre);
	}
	delay(200);
}
