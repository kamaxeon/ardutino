/**
 * EjemploDosCamarasLCD
 *
 * Copyright 2012 Israel Santana <isra@miscorreos.org>
 * 
 * Este ejemplo se ha hecho con un Arduino Mega, pero debería funcionar
 * sin problemas en otros Arduino. La versión usada es del software de
 * Arduino es la 1.0.1, debería funcionar con cualquier versión 1.x,
 * en versiones anteriores puede haber alguna modificación.
 * 
 * Simplemente vamos a mostrar la temperatura y humedad de dos camaras
 * y mostrarlos en un display LCD de 20x4
 */


// Incluímos la librería
#include <Camara.h>
#include <LiquidCrystal.h>


// Definimos la primera camara

const int pinEstado1         = 29;
const int pinApagado1        = 51;
const int pinDatosSensor1    = 40;
const int pinRelojSensor1    = 44;
const int id1                = 1;


Camara camara1(pinEstado1, pinApagado1, pinDatosSensor1, pinRelojSensor1, id1);

// Definimos la segunda camara

const int pinEstado2         = 25;
const int pinApagado2        = 47;
const int pinDatosSensor2    = 48;
const int pinRelojSensor2    = 52;
const int id2                = 2;

Camara camara2(pinEstado2, pinApagado2, pinDatosSensor2, pinRelojSensor2, id2);

// Definimos las variables que usaremos para almacenar los valores de las camaras
int t1, t2, h1, h2;
boolean e1, e2;


// Ahora empezamos con la definicion del LCD
LiquidCrystal lcd(7, 8, 9, 10, 11, 12 );
const int lcdFilas = 4;
const int lcdColumnas = 20;


void mostrarValores(int temp, int hum, boolean estado, int camara)
{


  int aux;
  // Supongo que la camara está encendida
  String textoEstado = "On";
  
  // Temperatura
  // Los valores van desde -40 a 128
  // Calculamos en funcion del valor la posicion dentro de la linea
  // Parto de la posicion negativa que sería la posicion 5
  lcd.setCursor(5, camara);
  
  // Compruebo si es positivo y menor de 100
  if (temp > 0 and temp < 100)
  {
    lcd.print(" ");
  }
  
  // Ahora compruebo si es positivo y menor de 10
  if (temp > 0 and temp < 10)
  {
    lcd.print(" ");
  }
  lcd.print(temp);
  
  
  
  // Humedad
  // Los valores van desde 0 hasta 100 %
  lcd.setCursor(11, camara);
  

  // Compruebo que es mejor del 10 %
  if (hum < 10)
  {
    lcd.print("  ");
  }
  
    // Ahora compruebo si está entre el 10 y el 99 %, lo normal 
  else if (hum < 100)
  {
    lcd.print(" ");
  }
  
  lcd.print(hum);
  
  // Estado
  
  // Compruebo el estado de no esté apagada, en el principio de la
  // función he supuesto que estaba encedida
  
  if (! estado)
  {
    textoEstado = "Off";
  }
  
  lcd.setCursor(17, camara);
  lcd.print(textoEstado);
  
  // Si vengo de un Off y paso a un On me sobra una f, por eso pongo el espacio
  // al On para evitar eso problemas
  if (estado)
  {
    lcd.print(" ");
  }
  
}

void mostrarPlantilla ()
{
  // Esta funcion solo vale para configurar la pantalla
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ArduTino 1.0");
  lcd.setCursor(0,1);
  lcd.print("C1 T:    H:    E:");
  lcd.setCursor(0,2);
  lcd.print("C2 T:    H:    E:");
  lcd.setCursor(0,3);
  lcd.print("Red: Off Grupo: Off");
}
void setup () {
	Serial.begin(9600); // Definimos el puerto serial
	Serial.println("Arrancando el ejemplo"); // Empezamos ;-)
  lcd.begin(lcdColumnas, lcdFilas);
  lcd.print("Ejemplo ArduTino");
  lcd.setCursor(0,1);
  lcd.print("solo sensores");
  delay(2000); 
  mostrarPlantilla();
}

void loop() {
  
  // Camara 1
  // Recogida de datos
  camara1.obtenerTemperatura();
	camara1.obtenerHumedad();
	camara1.obtenerEstado();
	// Impresion por el serial
  Serial.println("Mediciones para la primera camara");
  Serial.print("Temperatura: ");
  Serial.print(camara1.temperatura);
  Serial.print(" Humedad: ");
  Serial.println(camara1.humedad);
  
  Serial.println("");
  Serial.println("-----------------------------");
  Serial.println("");  
  // Ahora vamos a mostarlos en el LCD
  mostrarValores(camara1.temperatura, camara1.humedad, camara1.estado, camara1.id);

  // Camara 2
  // Recogida de datos
  camara2.obtenerTemperatura();
	camara2.obtenerHumedad();
	camara2.obtenerEstado();
	// Impresion por el serial
  Serial.println("Mediciones para la segunda camara");
  Serial.print("Temperatura: ");
  Serial.print(camara2.temperatura);
  Serial.print(" Humedad: ");
  Serial.println(camara2.temperatura);
 
  
  Serial.println("");
  Serial.println("-----------------------------");
  Serial.println(""); 
  // Ahora vamos a mostarlos en el LCD
  mostrarValores(camara2.temperatura, camara2.humedad, camara2.estado, camara2.id); 
  

 
  
  // Esperamos 3 segundos y volvemos a leer
  delay(3000);
  
}