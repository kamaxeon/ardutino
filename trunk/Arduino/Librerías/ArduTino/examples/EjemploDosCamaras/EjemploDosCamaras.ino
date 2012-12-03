/**
 * EjemploDosCamaras
 *
 * Copyright 2012 Israel Santana <isra@miscorreos.org>
 * 
 * Este ejemplo se ha hecho con un Arduino Mega, pero debería funcionar
 * sin problemas en otros Arduino. La versión usada es del software de
 * Arduino es la 1.0.1, debería funcionar con cualquier versión 1.x,
 * en versiones anteriores puede haber alguna modificación.
 * 
 * Simplemente vamos a mostrar la temperatura y humedad de dos camaras
 */


// Incluímos la librería
#include <Camara.h>


// Definimos la primera camara

const int pinEstado1         = 29;
const int pinApagado1        = 51;
const int pinDatosSensor1    = 40;
const int pinRelojSensor1    = 44;


Camara camara1(pinEstado1, pinApagado1, pinDatosSensor1, pinRelojSensor1);

// Definimos la segunda camara

const int pinEstado2         = 25;
const int pinApagado2        = 47;
const int pinDatosSensor2    = 48;
const int pinRelojSensor2    = 52;


Camara camara2(pinEstado2, pinApagado2, pinDatosSensor2, pinRelojSensor2);



void setup () {
	Serial.begin(9600); // Definimos el puerto serial
	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
  delay(500); 
}

void loop() {
  
  // Camara 1
  // Recogida de datos
  camara1.obtenerTemperatura();
	camara1.obtenerHumedad();
	// Impresion por el serial
  Serial.println("Mediciones para la primera camara");
  Serial.print("Temperatura: ");
  Serial.print(camara1.temperatura);
  Serial.print(" Humedad: ");
  Serial.println(camara1.temperatura);
  
  Serial.println("");
  Serial.println("-----------------------------");
  Serial.println("");  
  

  // Camara 2
  // Recogida de datos
  camara2.obtenerTemperatura();
	camara2.obtenerHumedad();
	// Impresion por el serial
  Serial.println("Mediciones para la segunda camara");
  Serial.print("Temperatura: ");
  Serial.print(camara2.temperatura);
  Serial.print(" Humedad: ");
  Serial.println(camara2.humeda);
 
  
  Serial.println("");
  Serial.println("-----------------------------");
  Serial.println("");  
 
  
  // Esperamos 3 segundos y volvemos a leer
  delay(3000);
  
}