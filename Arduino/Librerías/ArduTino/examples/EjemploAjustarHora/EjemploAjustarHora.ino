/**
 * EjemploAjustarHora
 *
 * Copyright 2012 Israel Santana <isra@miscorreos.org>
 * 
 * Este ejemplo se ha hecho con un Arduino Mega, pero debería funcionar
 * sin problemas en otros Arduino. La versión usada es del software de
 * Arduino es la 1.0.1, debería funcionar con cualquier versión 1.x,
 * en versiones anteriores puede haber alguna modificación.
 * 
 * Con este ejemplo se pretende ver la funcionalidad de la librería
 * RTClib usada para controlar la hora del ArduTino
 * 
 * Se pretende ver como ajustar la hora a una fecha determinada en el
 * RTC DS1307
 * 
 */
 
// Incluímos las librerías necesarias.
#include <Wire.h>
#include <RTClib.h>


// Creamos el objeto
RTC_DS1307 rtc;

void setup () {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
  Serial.println("---------------------");
  delay(1000); 
  
  // Ahora comprobamos que está el Reloj conectado y mostramos el resultado
  if (rtc.isrunning()) {
    Serial.println("Hemos conectado con el reloj");
  }
  else
  {
    Serial.println("Ha habido un problema para conectarnos al reloj");
  }
  
  
  // Ahora vamos a adjustar la hora, para eso le vamos a pasar una nosotros
  // aunque podríamos usar la fecha de compilación de este sketch usando
  // rtc.adjust(DateTime(__DATE__, __TIME__));
  
  // Creamos la variable de tipo DateTime con la fecha deseada
  DateTime fecha (2012, 12, 2, 21, 30, 0);
  
  // Ahora guardamos la fecha en el rtc
  
  if (rtc.isrunning())
  {
    rtc.adjust(fecha);
  }

}

void loop() {
  
  // Obtenemos la fecha
  DateTime now = rtc.now();
  // Ahora sólo nos queda mostrar la fecha por el serial
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  Serial.println("---------------------");
  
  delay(5000);
}