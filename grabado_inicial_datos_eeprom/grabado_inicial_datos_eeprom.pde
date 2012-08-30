// Estructura 15 bytes
// 9 primeros numero telefono
// 10 en blanco
// 11 activacion envio sms
// 12 en blanco
// 13 activacion recepcion llamadas
// 14, 15 libres sin usar (futuros uso)


// Libreria
#include <EEPROM.h>

// Variables

int Free[13] = { 
  48, 48, 48, 48, 48, 48, 48, 48, 48, 32, 68, 32, 68 };
int aux;
char valor;


void setup()

{
  Serial.begin(9600);
  // Grabado en blanco
  for (int i = 0 ; i < 8 ; i ++)
  {
		for (int a = 0 ; a < 13; a ++ ) 
		{
			EEPROM.write(i*15+a,Free[a]);
		}
	}
}

void loop()
{
  for (int a = 0 ; a < 118 ; a++ )
  {
    aux = EEPROM.read(a);
    valor = aux;
    Serial.print("Posicion de memoria ");
    Serial.print(a);
    Serial.print(", valor ");
    Serial.print(valor);
    Serial.println();
    delay(10);
  }
  delay(600000);
}




