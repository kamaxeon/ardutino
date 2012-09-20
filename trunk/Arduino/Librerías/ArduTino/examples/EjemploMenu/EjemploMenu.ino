/**
 * EjemploMenu
 *
 * Copyright 2012 Israel Santana <isra@miscorreos.org>
 * 
 * Este ejemplo se ha hecho con un Arduino Mega, pero debería funcionar
 * sin problemas en otros Arduino
 * 
 * En este ejemplo se prueban todas las funcionalidades que presenta
 * la librería:
 * 
 * 
 */

// Incluímos la librería
#include <Menu.h>


int aux;
Menu menu("panchito", 0);
Menu menu1("panchote", 1, menu, menu);
Menu menu2("lala", 2, menu, menu);
Menu menu12("lolo", 3, menu, menu1);
Menu * tmpMenu;
String texto;
String texto2;
String t;
String t2;

void setup()
{
	//menu1.ponerSiguiente(menu);
	//menu2.ponerSiguiente(menu1);
	Serial.begin(9600); // Definimos el puerto serial
	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
	aux = menu.obtenerId();
	//tmpMenu = menu1;
	texto = menu1.obtenerNombre();
	menu1.ponerNombre("langostino");
	texto2 = menu1.obtenerNombre();
	t = menu.obtenerNombrePorId(2);
	menu.ponerNombrePorId(2, "Kalimero");
	t2 = menu.obtenerNombrePorId(2);
	menu.ponerHijo(menu1);

}

void loop()
{
	// Serial.println(aux);
	// Serial.println(texto);
	// Serial.println(texto2);
	Serial.println(t);
	Serial.println(t2);
	delay(2000);
}
 