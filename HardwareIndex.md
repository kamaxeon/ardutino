## Hardware ##
Para la realización del proyecto se ha contado con componentes estándar de bajo coste que puedan ser adquiridos en cualquier sitio. La mayoría de los componentes han sido comprados en ebay, aliexpress, ... Para el montaje se ha optado por diseñar un PCB para que sea más cómodo la puesta en marcha y el futuro mantenimiento del mismo.

### Hardware comprado ###

  * **Sensores**: Basados en el sensor SHT1X
    * Especificaciones:
      * _Tipo de interfaz_: Serial (<sub>I</sub>2C no estándar)
      * _Rango humedad_: 0 - 100% humedad relativa
      * _Precisión humedad: 4,5% HR
      *_Rango temperatura_: -40 - 128 ºC
      *_Precisión temperatura_: 0.5 ºC (a 25 ºC)_

![http://ardutino.googlecode.com/svn/wiki/imagenes/sensor_sht1x.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/sensor_sht1x.jpg)

  * **Display**: Un display 20x4 basado en el chipset Hitachi HD47780

![http://ardutino.googlecode.com/svn/wiki/imagenes/lcd.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/lcd.jpg)

  * **Teclado**: Un teclado de lectura analógica de 5 teclas (basado en resistencias)

![http://ardutino.googlecode.com/svn/wiki/imagenes/keypad.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/keypad.jpg)

  * **Modem**: Un modem siemens tc 35 (debe funcionar con otros AT con ligeras variaciones)

![http://ardutino.googlecode.com/svn/wiki/imagenes/siemens.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/siemens.jpg)

  * **Serial**: Un puerto serie basado en Maxim MAX232 para comunicar el ArduTino con el Modem para el envío de alertas.

![http://ardutino.googlecode.com/svn/wiki/imagenes/rs232.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/rs232.jpg)

  * **Relés**: Relés varios para convertir de 12/24 V a 5 V (comunicación del ArduTino con las cámaras, red eléctrica, y grupo electrógeno).

![http://ardutino.googlecode.com/svn/wiki/imagenes/reles.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/reles.jpg)

  * **Reloj**: Con el fin de guardar la hora, se añade un DS1307 para almacenar la hora, aún cuando apaguemos el ArduTino.

![http://ardutino.googlecode.com/svn/wiki/imagenes/ds1307.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/ds1307.jpg)

### Hardware desarrollado ###

Con el fin de que la implementación final, puesta en marcha y futuro mantenimiento sea lo más fácil posible se ha diseñado un placa para unir todos los componentes que son parte de la solución. Esta ha sido diseñada también usando Software Libre con Kicad.

A continuación se puede ver el esquema eléctrico y el PCB en sí:

  * Esquema eléctrico

![http://ardutino.googlecode.com/svn/wiki/imagenes/ArduTino_esquema_electrico.png](http://ardutino.googlecode.com/svn/wiki/imagenes/ArduTino_esquema_electrico.png)

  * Prototipo

![http://ardutino.googlecode.com/svn/wiki/imagenes/Ardutino_prototipo1.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/Ardutino_prototipo1.jpg)

![http://ardutino.googlecode.com/svn/wiki/imagenes//Ardutino_prototipo2.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes//Ardutino_prototipo2.jpg)

### Creación del cable modem nulo ###

El cable debe tener conectores db9 machos (aunque en una imagen diga que son hembras), en estas tres imágenes se puede ver sin problemas.

  * Vista física del cable

![http://ardutino.googlecode.com/svn/wiki/imagenes/null.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/null.jpg)

  * Vista desde atrás

![http://ardutino.googlecode.com/svn/wiki/imagenes/nullcable.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/nullcable.jpg)

  * Vista desde delante

![http://ardutino.googlecode.com/svn/wiki/imagenes/rs232_2.jpg](http://ardutino.googlecode.com/svn/wiki/imagenes/rs232_2.jpg)