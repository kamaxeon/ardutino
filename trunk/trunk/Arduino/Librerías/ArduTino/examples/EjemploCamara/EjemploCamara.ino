/**
 * EjemploCamara
 *
 * Copyright 2012 Israel Santana <isra@miscorreos.org>
 * 
 * Este ejemplo se ha hecho con un Arduino Mega, pero debería funcionar
 * sin problemas en otros Arduino
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

const int pinEstado  			= 29;
const int pinApagado 			= 51;
const int pinDatosSensor		= 40;
const int pinRelojSensor		= 44;


Camara camara(pinEstado, pinApagado, pinDatosSensor, pinRelojSensor);

int temperatura;
int humedad;
String textoEstado;
boolean estado;
boolean cambioEstado;
String textoCambioEstado;

void setup()
{
	Serial.begin(9600); // Definimos el puerto serial
	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
}


void loop()
{
	// Recogemos los valores
	temperatura 	= camara.leerTemperatura();
	humedad 		= camara.leerHumedad();
	estado			= camara.leerEstado();
	cambioEstado	= camara.comprobarCambioEstado();
	if ( estado == true )
	{
		textoEstado = "encendida";
	}
	else
	{
		textoEstado = "apagada";
	}

	if (cambioEstado == true)
	{
		textoCambioEstado = "El estado de la camara ha cambiado";
	}
	else
	{
		textoCambioEstado = "El estado de la camara no ha cambiado"
	}
	
	Serial.print("Temperatura: ");
	Serial.println(temperatura);
	Serial.print("Humedad: ");
	Serial.println(humedad);
	Serial.print("Estado: camara ");
	Serial.println(textoEstado);
	Serial.println(textoCambioEstado);
	Serial.println("Apagando la camara");
	camara.apagar();
	Serial.println("Camara apagada");
	
	delay(3000);
}
