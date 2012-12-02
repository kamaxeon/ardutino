/**
 * EjemploCamara
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
 * - Leer la temperatura y humedad del sensor SHT1X
 * - Comprobar el estado de la cámara
 * - Apagar la cámara
 * 
 */

// Incluímos la librería
#include <Camara.h>

// Definimos los pin que vamos a usar, lo definiremos como constantes

const int pinEstado         = 29;
const int pinApagado        = 51;
const int pinDatosSensor    = 40;
const int pinRelojSensor    = 44;


Camara camara(pinEstado, pinApagado, pinDatosSensor, pinRelojSensor);

int temperatura;
int humedad;
String textoEstado;
boolean estado;
boolean cambioEstado;
String textoCambioEstado;
boolean ultimoEstado;
String textoUltimoEstado;

void setup()
{
	Serial.begin(9600); // Definimos el puerto serial
	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
}


void loop()
{
	// Recogemos los valores
	temperatura   = camara.obtenerTemperatura();
	humedad       = camara.obtenerHumedad();
	estado        = camara.obtenerEstado();
	ultimoEstado  = camara.obtenerUltimoEstado();
	cambioEstado  = camara.comprobarCambioEstado();
	if (sestado)
	{
		textoEstado = "encendida";
	}
	else
	{
		textoEstado = "apagada";
	}

	if (cambioEstado)
	{
		textoCambioEstado = "El estado de la camara ha cambiado";
	}
	else
	{
		textoCambioEstado = "El estado de la camara no ha cambiado";
	}

	if (ultimoEstado)
	{
		textoUltimoEstado = "encendida";
	}
	else
	{
		textoUltimoEstado = "apagada";
	}
	Serial.print("Temperatura: ");
	Serial.println(temperatura);
	Serial.print("Humedad: ");
	Serial.println(humedad);
	Serial.print("El ultimo estado conocido de la camara era ");
	Serial.println(textoUltimoEstado);
	Serial.print("Estado: camara ");
	Serial.println(textoEstado);
	Serial.println(textoCambioEstado);
	if (UltimoEstado)
	{
		Serial.println("Apagando la camara");
		camara.apagar();
		Serial.println("Camara apagada");
	}
	delay(3000);
}
