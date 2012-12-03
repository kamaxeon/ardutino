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


String textoEstado;
boolean cambioEstado;
String textoCambioEstado;
boolean ultimoEstado;
String textoUltimoEstado;

void setup()
{
	Serial.begin(9600); // Definimos el puerto serial
	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
  delay(500);
}


void loop()
{
	// Recogemos los valores
	camara.obtenerTemperatura();
	camara.obtenerHumedad();
	ultimoEstado  = camara.obtenerEstado();
	if (ultimoEstado)
	{
		textoEstado = "encendida";
	}
	else
	{
		textoEstado = "apagada";
	}

	if (camara.comprobarCambioEstado())
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
	Serial.println(camara.temperatura);
	Serial.print("Humedad: ");
	Serial.println(camara.humedad);
	Serial.print("El ultimo estado conocido de la camara era ");
	Serial.println(textoUltimoEstado);
	Serial.print("Estado: camara ");
	Serial.println(textoEstado);
	Serial.println(textoCambioEstado);
	if (ultimoEstado)
	{
		Serial.println("Apagando la camara");
		camara.apagar();
		Serial.println("Camara apagada");
	}
	delay(3000);
}
