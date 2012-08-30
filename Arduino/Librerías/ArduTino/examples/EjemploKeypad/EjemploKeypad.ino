/**
 * EjemploKeypad
 *
 * Copyright 2012 Israel Santana <isra@miscorreos.org>
 * 
 * Este ejemplo se ha hecho con un Arduino Mega, pero debería funcionar
 * sin problemas en otros Arduino
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
const int pinKeypad = 3;
// Valores del teclado, siempre se debería poner un número algo mayor del
// que nos devuelva el keypad
int valoresAnalogicos = {50, 200, 400, 600, 800};
// Relación teclas nombre
char* Teclas[] = {"Derecha", "Izquierda", "Arriba", "Abajo", "Intro"};
// Definimos el teclado en sí
Teclado teclado(pinKeypad, valoresAnalogicos);

void setup()
{
  Serial.begin(9600); // Definimos el puerto serial
	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
}

void loop()
{
  if (teclado.comprobarPulsacionNueva);
}
