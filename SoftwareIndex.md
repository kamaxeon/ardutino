## Convenciones ##

Se ha utilizado aunque sea un poco más lento tipos **int** y **String**, en detrimento de tipos como **uint8\_t**, **uint16\_t**, o vectores de **char**. El código cabe perfectamente en un Arduino Mega, y por eso se quiere facilitar la lectura y uso del proyecto a costa de disminuir el tamaño y aumentar la velocidad, cuando son despreciables en este caso.

## Librerías propias ##

Para la correcta realización del proyecto se ha desarrollado varias librerías para el uso del ArduTino, con la idea siempre en mente que el código de control sea lo más pequeño, limpio y claro de entender. Para esto nos basamos en estas librerías:

### Camara ###

#### Uso y definición ####
  * Es la encargada de comprobar el estado de la camara, leer los valores del sensor que lleva asociado y su apagado, cada cámara lleva asociado un sensor tipo SHT1X. La lectura de los sensores se ha realizado usando la librería de [Sensirion](http://code.google.com/p/arms22/downloads/detail?name=Sensirion-arduino-100.zip&can=2&q=)

#### Constructor ####
  * El constructor es muy simple, lo único que debemos pasarle son los valores de los pines para comprobar el estado, apagado, el pin de datos y reloj del sensor, el parámetro de id es opcional.

  * Ejemplo
```

#include <Camara.h>

const int pinEstado1         = 29;
const int pinApagado1        = 51;
const int pinDatosSensor1    = 40;
const int pinRelojSensor1    = 44;
const int id1                = 1;


Camara camara1(pinEstado1, pinApagado1, pinDatosSensor1, pinRelojSensor1, id1);
```

#### Atributos ####
  * La clase presenta 4 atributos públicos:
    * **humedad**-> Valor entero que representa el % de la humedad relativa
    * **temperatura**-> Valor entero que representa la temperatura en grados Celsius
    * **estado**-> Valor booleano que representa el estado de la cámara
    * **id**-> Valor entero que representa el id dado a la cámara

#### Métodos ####
  * La clase presenta los siguientes métodos público:
    * **actualizarValores()** -> no necesita ningún argumento ni devuelve nada, simplemente actualiza los atributos **humedad**, **temperatura** y **estado**.
    * **apagar()** -> no necesita ningún argumento ni devuelve nada, lo único que hace es activar durante 3 segundos el pin que debe estar conectado al apagado de la cámara
    * **comprobarCambioEstado()** -> no necesita ningún argumento y devuelve true o false si ha cambiado el estado, también actualiza el valor del atributo **estado**

### Teclado ###

#### Uso y definición ####
  * Librería creada específicamente para manejar el keypad de analógico de [Seeed Studio](http://www.seeedstudio.com/depot/electronic-brick-adkey-module-p-500.html)

#### Constructor ####
  * Para crear un objeto lo único que debemos pasarle es el pin de lectura, un vector de enteros con los valores que debemos recibir al pulsar los botones y el número de botones.

  * Ejemplo
```

#include <Teclado.h>

// El pin de lectura
int pinKeypad = 0;

// Valores del teclado, siempre se debería poner un número algo mayor del
// que nos devuelva el keypad
int valoresAnalogicos[] = {200, 450, 800, 970, 1100};


// Definimos el número de teclas, también podemos poner el número
// directamente
int numeroTeclas = sizeof(valoresAnalogicos)/sizeof(int);

// Definimos el teclado en sí
Teclado teclado(pinKeypad, valoresAnalogicos, numeroTeclas);
```

#### Atributos ####
  * Sólo presenta un atributo público
    * **ultimaTecla** -> Valor entero representado la tecla pulsada en relación al vector de valores pasados al constructor

#### Métodos ####
  * Sólo presenta un método público
    * **comprobarPulsacionNueva()** -> no necesita ningún parámetro y devuelve true o false en caso de haber pulsado una tecla nueva, a su vez actualiza el valor del atributo **ultimaTecla**

### Modem ###

#### Uso y definición ####
  * Librería creadad para controla a un modem TC35 conectado al Arduino, la función de dicha clase es controlar el modem y recoger quien llama o envía sms, el modem debe estar conectado al Serial1 del Arduino, es un requisito fundamental.

#### Constructor ####
  * Para crear un objeto tipo modem, lo único que debemos pasarle es la velocidad de comunicación en baudios.

  * Ejemplo
```
// Incluímos la librería del modem
#include <Modem.h>

// Creacion del objeto modem
const int velocidad = 9600;
Modem modem(velocidad);
```

#### Atributos ####
  * La clase presenta los siguientes atributos públicos:
    * **idLlamada** -> Valor entero largo, que representa un identificador interno de cuando se recibió la última llamada
    * **numeroLlamada** -> String que representa el DiD de la última llamada
    * **idSms** -> Valor entero largo, que representa un identificador interno de cuando se recibió el último Sms
    * **numeroSms** -> String que representa el DiD del último Sms
    * **textoSms** -> String que representa el texto del último Sms
    * **fechaSms** -> String que devuelve la fecha del último Sms recogida de la hora GSM en formato aammddhhmm

#### Métodos ####
  * La clase presenta los siguientes métodos públicos
    * **inicilizar()** -> no necesita ningún parámetro y es necesario llamarlo dentro de la función **setup()** del sketch de Arduino, borrar los diez primeros sms de la SIM, en teoría no debería hacer ninguno pero se borrar 10 por si hubiera alguno.
    * **comprobarSerial()** -> no necesita ningún argumento y no devuelve nada, lo único que hace es leer el serial y en caso de recibir una llamada o un sms actua. En caso de recibir una llamada automáticamente la corta y actualiza **idLlamada** y **numeroLlamada**, en caso de recibir un sms lo lee actualizando las variables **idSms**, **numeroSms** y **textoSms** y luego borra el sms recibido de la tarjeta SIM.
    * **enviarSms(tfno, texto)** -> necesita dos argumentos tipo string, no devuelve nada, enviando un sms.


  * **Menú y MenúLCD**: usadas para crear el menú de usuario y su presentación en el LCD (por documentar)