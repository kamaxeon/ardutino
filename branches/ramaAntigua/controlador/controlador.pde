// Varios

// Estructura dejamos 15 byte para cada telefono, solo usamos 11 pero asi no aseguramos futuras
// ampliaciones
// 0 - 8 -> Numeros telefono
// 10 -> Activar envio sms
// 12 -> Permitir llamar al invento

//# Librerias
//# ==================================
#include <LiquidCrystal.h>
#include <Menu.h>
#include <LCDMenu2.h>
#include <EEPROM.h>
#include <WProgram.h>
#include <Wire.h>
#include <DS1307.h>
#include <SHT1x.h>




///////////////////////////////////////////////////////////////
///                                                         ///
///                         Macros                          ///
///                                                         ///
///////////////////////////////////////////////////////////////
#define FILAS_LCD       	        2
#define COLUMNAS_LCD       	        16

#define TECLA_DERECHA 			0
#define TECLA_ARRIBA 			1
#define TECLA_ABAJO 		        2
#define TECLA_IZQUIERDA 		3
#define TECLA_SELECCIONAR 	        4

#define NOMBRE_APLICACION 	        "GSM ArduTino"
#define VERSION_APLICACION 		"1.0"
#define DESHABILITAR_SMS 		"Des. envio sms"
#define HABILITAR_SMS 			"Act. envio sms"
#define DESHABILITAR_LLAMADAS 		"Des. llamadas"
#define HABILITAR_LAMADAS		"Act. llamadas"
#define TAMANO_REGISTRO			15
#define DESPLAZAMIENTO_SMS		10
#define DESPLAZAMIENTO_LLAMADAS		12
#define LONGITUD_TELEFONO		9
#define NUMERO_TOTAL_TELEFONOS		8





#define TFNO1				0
#define TFNO2			        1
#define TFNO3				2
#define TFNO4				3
#define TFNO5				4
#define TFNO6			        5
#define TFNO7			        6
#define TFNO8				7

#define ENVIAR_SMS			0
#define PERMITIR_LLAMADAS		1
#define CAMBIO				"Cambio aplicado"
#define CORRECTO			"correctamente"
#define ERROR_TFNO			"Error: telefono"
#define NO_DEFINIDO			"no definido"

#define SIN_RELLENAR			""
#define EDITAR_NUMERO		        "Editar numero"
#define BORRAR_NUMERO			"Borrar numero"
#define ENV_SMS				"Enviar sms"

#define CONFIGURAR			"Configurando"
#define MODEM				"el modem gsm"

//#define ATENCION    "Atencion"
#define ENVIANDO_SMS      "Enviando sms"
#define ENVIADO_SMS       "Alerta enviada"


#define A0				48 // Caracter ascii para el 0
#define A1				49 // Caracter ascii para el 1
#define A2			        50 // Caracter ascii para el 2
#define A3				51 // Caracter ascii para el 3
#define A4				52 // Caracter ascii para el 4
#define A5				53 // Caracter ascii para el 5
#define A6				54 // Caracter ascii para el 6
#define A7				55 // Caracter ascii para el 7
#define A8				56 // Caracter ascii para el 8
#define A9				57 // Caracter ascii para el 9

#define AOK				65 // Caracter ascii para Activado
#define AKO				68 // Caracter ascii para la D (Desactivado)

#define NUM_KEYS  			5

#define MENU				0
#define EDICION				1
#define PANTALLA			2

// Pines fisicos usados

#define TESTIGO_SECADO		    23
#define GRUPO				          25
#define RED				            27
#define INTERRUPTOR_SECADO		29
#define LED_VERDE			        31
#define LED_ROJO			        33
#define PIN_RELOJ             35
#define PIN_DATOS             37
#define INTERRUPTOR			      39




#define TIEMPO_ESPERA			15 // Tiempo de espera del display
// para mostrar el estado

#define TIEMPO_ESPERA_CAIDA_ELECTRICA_COMPROBAR_CAMARA 300 //Eso
#define TIEMPO_ESPERA_MODEM		500


///////////////////////////////////////////////////////////////
///                                                         ///
///                       Variables                         ///
///                                                         ///
///////////////////////////////////////////////////////////////

SHT1x sensor(PIN_DATOS, PIN_RELOJ);

byte celsius[8] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

int modo_teclado = MENU;
int movilIndice;
int movilAux;
int vectorMovil[9];

boolean smsValido = false;

boolean modo_mantinimiento = false;

boolean limpiarPantalla = true;

unsigned long millisAntes = 0;
unsigned long millisCaidaElectrica = 0;

String origenSms;

int numeroRing = 0;
char caracter=0;
//~ String movilIsra="655191999"; // Solo para pruebas

unsigned long millisLlamada = 0;
//# Caracteres especiales
//# ==================================
uint8_t flechaArriba[8]    = {
  0x4,0xE,0x15,0x4,0x4,0x4,0x0};
uint8_t flechaAbajo[8]  = {
  0x0,0x4,0x4,0x4,0x15,0xE,0x4};


//# Lectura teclado
//# ==================================
int  adc_key_val[5] ={ 
  30, 200, 400, 600, 800 };
int adc_key_in;
int key=-1;
int oldkey=-1;
int valor;


//# Clases
//# ==================================
LiquidCrystal lcd(8, 11, 9, 4, 5, 6, 7);


Menu top("Root",0);

// menu, lcd, rows, cols, flechaArriba_pos, flechaAbajo_pos
LCDMenu2 Root(top, lcd , FILAS_LCD, COLUMNAS_LCD, 0, 1);



boolean modo_fecha = false; // El modo de configuracion de fecha

// Variable electricas

boolean red			= true ; // Asumo que hay tension de la red
boolean auxRed ;
boolean grupo 	= false; // Asumo que el grupo esta parado
boolean auxGrupo ;
boolean camara  = true ; // Asumo que la camara esta encendida
boolean auxCamara;
boolean comprobrar_camara = true ; // Variable que se usa para tener
// entre que se cae la red electrica y si vuelve a comprobar la camara

String telefono;


int vectorFecha[5]; //hora, minutos, dia, mes, ano
int fechaIndice ;
int vectorPosicionesFecha [5] = { 12, 15, 9, 12, 15 };
///////////////////////////////////////////////////////////////
///                                                         ///
///                         Menu                            ///
///                                                         ///
///////////////////////////////////////////////////////////////







//# Menu
//# ==================================

// Menu inicial
Menu Item1("Num. admitidos",1);
Menu Item2("Config. fecha", 2);
Menu Item3("Config. modem", 3);
Menu Item4("Acerca de", 4);

// Submenu 1
Menu Item11(SIN_RELLENAR, 11);
Menu Item12(SIN_RELLENAR, 12);
Menu Item13(SIN_RELLENAR, 13);
Menu Item14(SIN_RELLENAR, 14);
Menu Item15(SIN_RELLENAR, 15);
Menu Item16(SIN_RELLENAR, 16);
Menu Item17(SIN_RELLENAR, 17);
Menu Item18(SIN_RELLENAR, 18);

// Submenu 1.1
Menu Item111(SIN_RELLENAR, 111); // Activar desactivar alarma sms
Menu Item112(SIN_RELLENAR, 112); // Activar desactivar autorizar llamada
Menu Item113(EDITAR_NUMERO, 113); // Editar numero
Menu Item114(BORRAR_NUMERO, 114); //Borrar Numero
Menu Item115(ENV_SMS, 115); // Enviar sms de pruebas

// Submenu 1.2
Menu Item121(SIN_RELLENAR, 121); // Activar desactivar alarma sms
Menu Item122(SIN_RELLENAR, 122); // Activar desactivar autorizar llamada
Menu Item123(EDITAR_NUMERO, 123); // Editar numero
Menu Item124(BORRAR_NUMERO, 124); //Borrar Numero
Menu Item125(ENV_SMS, 125); // Enviar sms de pruebas

// Submenu 1.3
Menu Item131(SIN_RELLENAR, 131); // Activar desactivar alarma sms
Menu Item132(SIN_RELLENAR, 132); // Activar desactivar autorizar llamada
Menu Item133(EDITAR_NUMERO, 133); // Editar numero
Menu Item134(BORRAR_NUMERO, 134); //Borrar Numero
Menu Item135(ENV_SMS, 135); // Enviar sms de pruebas

// Submenu 1.4
Menu Item141(SIN_RELLENAR, 141); // Activar desactivar alarma sms
Menu Item142(SIN_RELLENAR, 142); // Activar desactivar autorizar llamada
Menu Item143(EDITAR_NUMERO, 143); // Editar numero
Menu Item144(BORRAR_NUMERO, 144); //Borrar Numero
Menu Item145(ENV_SMS, 145); // Enviar sms de pruebas

// Submenu 1.5
Menu Item151(SIN_RELLENAR, 151); // Activar desactivar alarma sms
Menu Item152(SIN_RELLENAR, 152); // Activar desactivar autorizar llamada
Menu Item153(EDITAR_NUMERO, 153); // Editar numero
Menu Item154(BORRAR_NUMERO, 154); //Borrar Numero
Menu Item155(ENV_SMS, 155); // Enviar sms de pruebas

// Submenu 1.6
Menu Item161(SIN_RELLENAR, 161); // Activar desactivar alarma sms
Menu Item162(SIN_RELLENAR, 162); // Activar desactivar autorizar llamada
Menu Item163(EDITAR_NUMERO, 163); // Editar numero
Menu Item164(BORRAR_NUMERO, 164); //Borrar Numero
Menu Item165(ENV_SMS, 165); // Enviar sms de pruebas

// Submenu 1.7
Menu Item171(SIN_RELLENAR, 171); // Activar desactivar alarma sms
Menu Item172(SIN_RELLENAR, 172); // Activar desactivar autorizar llamada
Menu Item173(EDITAR_NUMERO, 173); // Editar numero
Menu Item174(BORRAR_NUMERO, 174); //Borrar Numero
Menu Item175(ENV_SMS, 175); // Enviar sms de pruebas

// Submenu 1.8
Menu Item181(SIN_RELLENAR, 181); // Activar desactivar alarma sms
Menu Item182(SIN_RELLENAR, 182); // Activar desactivar autorizar llamada
Menu Item183(EDITAR_NUMERO, 183); // Editar numero
Menu Item184(BORRAR_NUMERO, 184); //Borrar Numero
Menu Item185(ENV_SMS, 185); // Enviar sms de pruebas

// Submenu 2.1
//~ Menu Item21(SIN_RELLENAR, 21);

//# Function
//# ==================================

//## Menu Init
void IniciarMenu()
{
  // Menu principales
  top.addChild(Item1);
  top.addChild(Item3);
  top.addChild(Item2);
  top.addChild(Item4);

  // Menu 1 (Telefonos)
  Item1.addChild(Item11);
  Item1.addChild(Item12);
  Item1.addChild(Item13);
  Item1.addChild(Item14);
  Item1.addChild(Item15);
  Item1.addChild(Item16);
  Item1.addChild(Item17);
  Item1.addChild(Item18);

  // Menu 1.1 (Telefono 1)
  Item11.addChild(Item111);
  Item11.addChild(Item112);
  Item11.addChild(Item113);
  Item11.addChild(Item114);
  Item11.addChild(Item115);

  // Menu 1.2 (Telefono 2)
  Item12.addChild(Item121);
  Item12.addChild(Item122);
  Item12.addChild(Item123);
  Item12.addChild(Item124);
  Item12.addChild(Item125);

  // Menu 1.3 (Telefono 3)
  Item13.addChild(Item131);
  Item13.addChild(Item132);
  Item13.addChild(Item133);
  Item13.addChild(Item134);
  Item13.addChild(Item135);

  // Menu 1.4 (Telefono 4)
  Item14.addChild(Item141);
  Item14.addChild(Item142);
  Item14.addChild(Item143);
  Item14.addChild(Item144);
  Item14.addChild(Item145);

  // Menu 1.5 (Telefono 5)
  Item15.addChild(Item151);
  Item15.addChild(Item152);
  Item15.addChild(Item153);
  Item15.addChild(Item154);
  Item15.addChild(Item155);

  // Menu 1.6 (Telefono 6)
  Item16.addChild(Item161);
  Item16.addChild(Item162);
  Item16.addChild(Item163);
  Item16.addChild(Item164);
  Item16.addChild(Item165);

  // Menu 1.7 (Telefono 7)
  Item17.addChild(Item171);
  Item17.addChild(Item172);
  Item17.addChild(Item173);
  Item17.addChild(Item174);
  Item17.addChild(Item175);

  // Menu 1.8 (Telefono 8)
  Item18.addChild(Item181);
  Item18.addChild(Item182);
  Item18.addChild(Item183);
  Item18.addChild(Item184);
  Item18.addChild(Item185);


  // Usa vez creado mostramos el display :-) 

  Root.display();
}    

//## Tecla
void ComprobarTecla(int tecla, int modo_teclado)
{
  switch (modo_teclado)
  {
  case MENU:
    switch (tecla) 
    {

    case TECLA_ARRIBA:
      Root.goUp();
      //~ Serial.println("'up'");
      break;

    case TECLA_ABAJO:
      Root.goDown();
      //~ Serial.println("'down'");
      break;

    case TECLA_SELECCIONAR:
      Root.goEnter();
      //~ Serial.println("'enter'");
      break;

    case TECLA_IZQUIERDA:
      Root.goBack();
      //~ Serial.println("'back'"); 
      break;
    }
    break;
  case EDICION:
    switch (tecla)
    {
    case TECLA_ARRIBA:
      if (modo_fecha == false) 
      {
        EditarNumeroVertical(1);
      }
      else
      {
        EditarFechaVertical(1);
      }
      //~ Serial.println("Arriba edicion");
      break;

    case TECLA_ABAJO:
      if (modo_fecha == false)
      {
        EditarNumeroVertical(-1);
      }
      else
      {
        EditarFechaVertical(-1);
      }
      //~ Serial.println("Abajo edicion");
      break;

    case TECLA_SELECCIONAR:
      if (modo_fecha == false)
      {
        GuardarNumero();
      }
      else
      {
        GuardarFecha();
      }
      break;

    case TECLA_IZQUIERDA:
      if (modo_fecha == false)
      {
        EditarNumeroHorizontal(-1); 
      }
      else
      {
        EditarFechaHorizontal(-1);
      }
      break;
    case TECLA_DERECHA:
      if (modo_fecha == false)
      {
        EditarNumeroHorizontal(1);
      }
      else
      {
        EditarFechaHorizontal(1);
      }
      break;
    }
    break;
  case PANTALLA:
    DesactivarMostrarSensores();
    break;

  }
}

//##Funcion para generar todos el menu de los numeros
void GenerarMenuNumerosAdmitidos() 
{
  for (int i = 0 ; i < NUMERO_TOTAL_TELEFONOS ; i ++ )
  {
    String aux;
    // Leemos la eeprom TAMANO_REGISTRO
    for (int a = 0; a < 13 ; a ++)
    {
      aux.concat(EEPROM.read(i*TAMANO_REGISTRO + a));
    }
    // Ponemos el numero al display
    switch(i) {
    case TFNO1:
      Item11.SetName(aux);
      break;
    case TFNO2:
      Item12.SetName(aux);
      break;
    case TFNO3:
      Item13.SetName(aux);
      break;
    case TFNO4:
      Item14.SetName(aux);
      break;
    case TFNO5:
      Item15.SetName(aux);
      break;
    case TFNO6:
      Item16.SetName(aux);
      break;
    case TFNO7:
      Item17.SetName(aux);
      break;
    case TFNO8:
      Item18.SetName(aux);
      break;  

    }
    //Serial.println(aux);
    // Ahora tocamos los menus internos de ese numero
    // Envio de sms
    if ( aux[DESPLAZAMIENTO_SMS] == 'A')
    {
      switch(i) {
      case TFNO1:
        Item111.SetName(DESHABILITAR_SMS);
        break;
      case TFNO2:
        Item121.SetName(DESHABILITAR_SMS);
        break;
      case TFNO3:
        Item131.SetName(DESHABILITAR_SMS);
        break;
      case TFNO4:
        Item141.SetName(DESHABILITAR_SMS);
        break;
      case TFNO5:
        Item151.SetName(DESHABILITAR_SMS);
        break;
      case TFNO6:
        Item161.SetName(DESHABILITAR_SMS);
        break;
      case TFNO7:
        Item171.SetName(DESHABILITAR_SMS);
        break;
      case TFNO8:
        Item181.SetName(DESHABILITAR_SMS);
        break;
      }
    }
    else
    {
      switch(i) {
      case TFNO1:
        Item111.SetName(HABILITAR_SMS);
        break;
      case TFNO2:
        Item121.SetName(HABILITAR_SMS);
        break;
      case TFNO3:
        Item131.SetName(HABILITAR_SMS);
        break;
      case TFNO4:
        Item141.SetName(HABILITAR_SMS);
        break;
      case TFNO5:
        Item151.SetName(HABILITAR_SMS);
        break;
      case TFNO6:
        Item161.SetName(HABILITAR_SMS);
        break;
      case TFNO7:
        Item171.SetName(HABILITAR_SMS);
        break;
      case TFNO8:
        Item181.SetName(HABILITAR_SMS);
        break;
      }
    }
    // Permitir llamadas
    if ( aux[DESPLAZAMIENTO_LLAMADAS] == 'A')
    {
      switch(i) {
      case TFNO1:
        Item112.SetName(DESHABILITAR_LLAMADAS);
        break;
      case TFNO2:
        Item122.SetName(DESHABILITAR_LLAMADAS);
        break;
      case TFNO3:
        Item132.SetName(DESHABILITAR_LLAMADAS);
        break;
      case TFNO4:
        Item142.SetName(DESHABILITAR_LLAMADAS);
        break;
      case TFNO5:
        Item152.SetName(DESHABILITAR_LLAMADAS);
        break;
      case TFNO6:
        Item162.SetName(DESHABILITAR_LLAMADAS);
        break;
      case TFNO7:
        Item172.SetName(DESHABILITAR_LLAMADAS);
        break;
      case TFNO8:
        Item182.SetName(DESHABILITAR_LLAMADAS);
        break;
      }
    }
    else
    {
      switch(i) {
      case TFNO1:
        Item112.SetName(HABILITAR_LAMADAS);
        break;
      case TFNO2:
        Item122.SetName(HABILITAR_LAMADAS);
        break;
      case TFNO3:
        Item132.SetName(HABILITAR_LAMADAS);
        break;
      case TFNO4:
        Item142.SetName(HABILITAR_LAMADAS);
        break;
      case TFNO5:
        Item152.SetName(HABILITAR_LAMADAS);
        break;
      case TFNO6:
        Item162.SetName(HABILITAR_LAMADAS);
        break;
      case TFNO7:
        Item172.SetName(HABILITAR_LAMADAS);
        break;
      case TFNO8:
        Item182.SetName(HABILITAR_LAMADAS);
        break;
      }
    }
  }
}


boolean ComprobarNumeroInicializado(int movil)
{
  if ( EEPROM.read(TAMANO_REGISTRO*movil) == '0' )
  {
    return false;
  }
  else
  {
    return true;
  }
}


void CambiarMenusActivacion(int movil, int opcion)
{
  // Comprueba que el numero esta definido 
  if ( ComprobarNumeroInicializado(movil)) {
    int position;
    // Opcion -> 0 para el cambio de sms
    if ( opcion == ENVIAR_SMS )
    {
      position = TAMANO_REGISTRO*movil + DESPLAZAMIENTO_SMS;
    }
    // Opcion -> 1 para el cambio de llamadas
    else if ( opcion == PERMITIR_LLAMADAS ) 
    {
      position = TAMANO_REGISTRO*movil + DESPLAZAMIENTO_LLAMADAS ;
    }
    char valor = EEPROM.read(position);
    if ( valor == 'A') 
    {
      EEPROM.write(position, AKO);
    }
    else
    {
      EEPROM.write(position, AOK);
    }
    MostrarInfo(CAMBIO, CORRECTO);
  }
  else
  {
    // Numero no definido
    MostrarInfo(ERROR_TFNO,NO_DEFINIDO);
  }

}

void BorrarNumero(int movil)
{
  if ( ComprobarNumeroInicializado(movil)) {
    int position = TAMANO_REGISTRO*movil;
    // Es una chorrada de funcion, pongo los bits a 0 y listo
    for (int c = position ; c < position + LONGITUD_TELEFONO ; c ++)
    {
      EEPROM.write(c, A0);
    }
    // Ahora deshabilito el envio de sms y llamadas
    EEPROM.write(position+DESPLAZAMIENTO_SMS, AKO);
    EEPROM.write(position+DESPLAZAMIENTO_LLAMADAS, AKO);
    MostrarInfo(CAMBIO, CORRECTO);
  }
  else
  {
    // Numero no definido
    MostrarInfo(ERROR_TFNO, NO_DEFINIDO);
  }
}


//## Function Check
void ComprobarFuncion()
{
  //Quitamos las opciones del display
  lcd.noCursor();
  switch(Root.id) {

  case 1: // Entramos al menus telefonico
    GenerarMenuNumerosAdmitidos();
    break;

  case 2: // Configuracion de la fecha del RTC
    EditarFecha();
    break;
    
  case 3: // Menu de modem
    ConfigurarModem();
    MostrarInfo(CONFIGURAR, MODEM);
    break;

  case 4:
    MostrarInfo(NOMBRE_APLICACION, VERSION_APLICACION);
    break;

    // Opciones telefono 1
  case 111: // Telefono 1 - envio sms
    CambiarMenusActivacion(TFNO1, ENVIAR_SMS);
    break;
  case 112: // Telefono 1 - permitir llamadas
    CambiarMenusActivacion(TFNO1, PERMITIR_LLAMADAS);
    break;
  case 113: // Telefono 1 - editar numero
    EditarNumero(TFNO1);
    break;
  case 114: // Telefono 1 - borrar numero
    BorrarNumero(TFNO1);
    break;
  case 115: // Telefono 1 - enviar sms
    EnviarSmsDirecto(TFNO1);
    break;    

    // Opciones telefono 2    
  case 121: // Telefono 2 - envio sms
    CambiarMenusActivacion(TFNO2, ENVIAR_SMS);
    break;
  case 122: // Telefono 2 - permitir llamadas
    CambiarMenusActivacion(TFNO2, PERMITIR_LLAMADAS);
    break;
  case 123: // Telefono 2 - editar numero
    EditarNumero(TFNO2);
    break;
  case 124: // Telefono 2 - borrar numero
    BorrarNumero(TFNO2);
    break;
  case 125: // Telefono 2 - enviar sms
    EnviarSmsDirecto(TFNO2);
    break;    

    // Opciones telefono 3     
  case 131: // Telefono 3 - envio sms
    CambiarMenusActivacion(TFNO3, ENVIAR_SMS);
    break;
  case 132: // Telefono 3 - permitir llamadas
    CambiarMenusActivacion(TFNO3, PERMITIR_LLAMADAS);
    break;
  case 133: // Telefono 3 - editar numero
    EditarNumero(TFNO3);
    break;
  case 134: // Telefono 3 - borrar numero
    BorrarNumero(TFNO3);
    break;
  case 135: // Telefono 3 - enviar sms
    EnviarSmsDirecto(TFNO3);
    break;    

    // Opciones telefono 4
  case 141: // Telefono 4 - envio sms
    CambiarMenusActivacion(TFNO4, ENVIAR_SMS);
    break;
  case 142: // Telefono 4 - permitir llamadas
    CambiarMenusActivacion(TFNO4, PERMITIR_LLAMADAS);
    break;
  case 143: // Telefono 4 - editar numero
    EditarNumero(TFNO4);
    break;
  case 144: // Telefono 4 - borrar numero
    BorrarNumero(TFNO4);
    break;
  case 145: // Telefono 4 - enviar sms
    EnviarSmsDirecto(TFNO4);
    break;    

    // Opciones telefono 5
  case 151: // Telefono 5 - envio sms
    CambiarMenusActivacion(TFNO5, ENVIAR_SMS);
    break;
  case 152: // Telefono 5 - permitir llamadas
    CambiarMenusActivacion(TFNO5, PERMITIR_LLAMADAS);
    break;
  case 153: // Telefono 5 - editar numero
    EditarNumero(TFNO5);
    break;
  case 154: // Telefono 5 - borrar numero
    BorrarNumero(TFNO5);
    break;
  case 155: // Telefono 5 - enviar sms
    EnviarSmsDirecto(TFNO5);
    break;    

    // Opciones telefono 6
  case 161: // Telefono 6 - envio sms
    CambiarMenusActivacion(TFNO6, ENVIAR_SMS);
    break;
  case 162: // Telefono 6 - permitir llamadas
    CambiarMenusActivacion(TFNO6, PERMITIR_LLAMADAS);
    break;
  case 163: // Telefono 6 - editar numero
    EditarNumero(TFNO6);
    break;
  case 164: // Telefono 6 - borrar numero
    BorrarNumero(TFNO6);
    break;
  case 165: // Telefono 6 - enviar sms
    EnviarSmsDirecto(TFNO6);
    ;
    break;    

    // Opciones telefono 7
  case 171: // Telefono 7 - envio sms
    CambiarMenusActivacion(TFNO7, ENVIAR_SMS);
    break;
  case 172: // Telefono 7 - permitir llamadas
    CambiarMenusActivacion(TFNO7, PERMITIR_LLAMADAS);
    break;   
  case 173: // Telefono 7 - editar numero
    EditarNumero(TFNO7);
    break;
  case 174: // Telefono 7 - borrar numero
    BorrarNumero(TFNO7);
    break;
  case 175: // Telefono 7 - enviar sms
    EnviarSmsDirecto(TFNO7);
    break;    

    // Opciones telefono 8
  case 181: // Telefono 8 - envio sms
    CambiarMenusActivacion(TFNO8, ENVIAR_SMS);
    break;
  case 182: // Telefono 8 - permitir llamadas
    CambiarMenusActivacion(TFNO8, PERMITIR_LLAMADAS);
    break;  
  case 183: // Telefono 8 - editar numero
    EditarNumero(TFNO8);
    break;
  case 184: // Telefono 8 - borrar numero
    BorrarNumero(TFNO8);
    break;
  case 185: // Telefono 8 - enviar sms
    EnviarSmsDirecto(TFNO8);
    ;
    break;          
  }
  // Si elegimos editar telefono no queremos mostar ningun menu
  // por eso revisamos aqui que no hemos cambiado de modo de teclado
  if (modo_teclado == MENU)
  {
    GenerarMenuNumerosAdmitidos();
    // Aqui tengo que poner la funciones
    Root.display();    
  }

}

void MostrarInfo(String a, String b)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(a);
  lcd.setCursor(0,1);
  lcd.print(b);
  delay(1500); 
}

void EditarNumero(int movil)
{
  // Primero que nada ponemos el booleano editarNumero a true para cambiar el
  // comportamiento del teclado en el loop principal
  modo_teclado = EDICION;
  movilAux = movil;
  movilIndice = 7;

  // Recogemos el valor del telefono de la eeprom
  int position = TAMANO_REGISTRO*movil;
  String aux;
  for (int c = position ; c < position + LONGITUD_TELEFONO ; c ++)
  {
    aux.concat(EEPROM.read(c));
  }
  for (int c = 0 ; c < LONGITUD_TELEFONO ; c ++)
  {
    // Si el primer caracter es 0, ponemos un 6 para facilitar la cosa
    if (c == 0  && aux[c] == '0')
    { 
      vectorMovil[c] = 6;  
      aux[0] = '6';

    }
    else
    {
      vectorMovil[c] = aux[c]-48; // 48 es el caracter 0 en ASCII
    }
  }
  // Ahora hacemos la parte del display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Guardar -> OK");
  lcd.setCursor(7,1);
  lcd.print(aux);
  lcd.setCursor(7,1);
  lcd.cursor();
}

void EditarNumeroHorizontal(int aux)
{
  // Desplazamiento a la izquierda
  if (aux == -1 && movilIndice > 7 )
  {
    movilIndice -= 1;
  }
  // Desplazamiento a la derecha
  if ( aux == 1 && movilIndice < 15)
  {
    movilIndice += 1; 
  }
  lcd.setCursor(movilIndice,1);
}

void EditarNumeroVertical(int aux)
{
  int menor = 0; 
  int mayor = LONGITUD_TELEFONO;
  if (movilIndice == 7)
  {
    menor = 6;
  }
  // Sumando
  if ( aux == 1 )
  { 
    // Si no hemos llegado al valor mayor sumamos uno
    if (vectorMovil[movilIndice-7] < mayor)
    {
      vectorMovil[movilIndice-7] += 1;
    }
    // Si hemos llegado a valor mayor volvemos al menor
    // 9 + 1 = 0
    else
    {
      vectorMovil[movilIndice-7] = menor;
    }
  }
  // Restando
  if ( aux == -1 )
  {
    // Si no hemos llegado al valor menor restamos uno
    if (vectorMovil[movilIndice-7] > menor)
    {
      vectorMovil[movilIndice-7] += -1;
    }
    // Si hemos llegado a valor menor volvemos al mayor
    // 0 - 1 = 9
    else
    {
      vectorMovil[movilIndice-7] = mayor;
    }
  }	
  lcd.print(vectorMovil[movilIndice-7]);
  lcd.setCursor(movilIndice,1);
}

void GuardarNumero()
{
  int position = TAMANO_REGISTRO*movilAux;
  for (int c = 0 ; c < LONGITUD_TELEFONO ; c++)
  {
    EEPROM.write(position+c,vectorMovil[c]+48);
  }
  // Quitamos el guion bajo  del cursor
  lcd.noCursor();
  // Mostramos la informacion
  MostrarInfo(CAMBIO,CORRECTO);
  // Salimos del modo registro
  modo_teclado = MENU;
  // Generamos los listados de numeros
  GenerarMenuNumerosAdmitidos();
  // Mostramos el menu
  Root.display();
}






///////////////////////////////////////////////////////////////
///                                                         ///
///                         Teclado                         ///
///                                                         ///
///////////////////////////////////////////////////////////////





int LeerTeclado()
{
  //leemos el sensor analogico
  adc_key_in = analogRead(0);    // read the value from the sensor  
  //digitalWrite(13, HIGH);
  key = ObtenerTecla(adc_key_in);		        // convert into key press

  if (key != oldkey)				    // if keypress is detected
  {
    delay(50);		// wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = ObtenerTecla(adc_key_in);		        // convert into key press
    digitalWrite(13, LOW);
    if (key != oldkey)				
    {			
      oldkey = key;
      if (key >=0){
        return key;
      }
      else {
        return -1;
      }
    }
    else {
      return -1;
    }

  }
  else {
    return -1;
  }
  //digitalWrite(13, LOW);
}
// Convert ADC value to key number
int ObtenerTecla(unsigned int input)
{
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }

  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed

  return k;
}

///////////////////////////////////////////////////////////////
///                                                         ///
///                       Sensores                          ///
///                                                         ///
///////////////////////////////////////////////////////////////


int  LeerTemperatura()
{
  return (int)round(sensor.readTemperatureC());
}

int LeerHumedad()
{
  return (int)round(sensor.readHumidity());
}

void MostrarSensores(boolean pantalla)
{
  lcd.noCursor(); // Por si las moscas
  if (pantalla == true)
  {
    modo_teclado = PANTALLA; 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("T.:      H.:   %");
    lcd.setCursor(6,0);
    lcd.write(3); // Soy asÃ­ de friki :-)
    lcd.setCursor(0,1);
    lcd.print("M.:      C.:   "); 		
    limpiarPantalla = false;
  }
  lcd.setCursor(3,0);
  if (LeerTemperatura()>0)
  {
    lcd.print(" ");
  }
  lcd.print(GenerarDigito(LeerTemperatura()));
  lcd.setCursor(12,0);
  if (LeerTemperatura()<99)
  {
    lcd.print(" ");
  }
  lcd.print(GenerarDigito(LeerHumedad()));
  

  // Ahora miramos en que modo estamos
  String aux2 = ObtenerModo();

  lcd.setCursor(4 ,1);
  lcd.print(aux2);

  String aux3 = ObtenerModoCamara();
  lcd.setCursor(13, 1);
  lcd.print(aux3);
  //delay(100);
}

void DesactivarMostrarSensores()
{
  modo_teclado = MENU;
  Root.display();
}




///////////////////////////////////////////////////////////////
///                                                         ///
///                        Sms			            ///
///                                                         ///
///////////////////////////////////////////////////////////////


void EnviarSmsDirecto(int movil)
{
  // Comprueba que el numero esta definido 
  if ( ComprobarNumeroInicializado(movil)) {
    MostrarInfo("Enviando sms", "de pruebas");
    String tfno = ObtenerTelefono(movil);
    String sms = CrearCuerpoSms();
    EnviarSms(tfno, sms);
    MostrarInfo("Sms de pruebas", "enviado");
    Root.display();
  }
  else
  {
    // Numero no definido
    MostrarInfo(ERROR_TFNO,NO_DEFINIDO);
  }
}


void EnviarSmsAlertaTension(int fuente, boolean estado)
{
  String textoFuente;
  String textoEstado;
  if (fuente == RED)
  {
    textoFuente = "de la red";
  }
  else
  {
    textoFuente = "del grupo";
  }

  if (estado == true)
  {
    textoEstado = "Arranque ";
  }
  else
  {
    textoEstado = "Caida ";
  }

  String sms;
  sms.concat(textoEstado);
  sms.concat(textoFuente);
  sms.concat("\n");
  sms.concat(CrearCuerpoSms());

  for (int a = 0 ; a < NUMERO_TOTAL_TELEFONOS ; a ++)
    if (EEPROM.read(TAMANO_REGISTRO*a + DESPLAZAMIENTO_SMS) == 'A')
    {
      String tfno = ObtenerTelefono(a);
      EnviarSms(tfno, sms);
    }

}

void EnviarSmsAlertaCamara(boolean estado)
{
  String textoAuxiliar;
  if ( estado == true )
  {
    textoAuxiliar = "Ha arrancada la camara";
  }
  else
  {
    textoAuxiliar = "Ha parado la camara";
  }

  String sms;
  sms.concat(textoAuxiliar);
  sms.concat("\n");
  sms.concat(CrearCuerpoSms());

  for (int a = 0 ; a < NUMERO_TOTAL_TELEFONOS ; a ++)
    if (EEPROM.read(TAMANO_REGISTRO*a + DESPLAZAMIENTO_SMS) == 'A')
    {
      String tfno = ObtenerTelefono(a);
      EnviarSms(tfno, sms);
    }

}

String CrearCuerpoSms()
{
  String aux1;
  String aux2;
  String camara = ObtenerModoCamara();
  String temp;
  String modo = ObtenerModo();

  if (camara == "on ")
  {
    aux1 = "encendida";
  }
  else
  {
    aux1 = "apagada";
  }
  if (modo == "gru")
  {
    aux2 = "grupo";
  }
  else if (modo == "red")
  {
    aux2 = "red electrica";
  }
  else
  {
    aux2 = "sin conexion";
  }

  
  String sms;
  sms.concat("Hora: ");
  sms.concat(GenerarDigito(RTC.get(DS1307_HR,true))); 
  sms.concat(":"); 
  sms.concat(GenerarDigito(RTC.get(DS1307_MIN,false))); 
  sms.concat("\n");
  sms.concat("Fecha: ");
  sms.concat(GenerarDigito(RTC.get(DS1307_DATE,false)));  
  sms.concat("/");
  sms.concat(GenerarDigito(RTC.get(DS1307_MTH,false)));
  sms.concat("/");
  sms.concat(RTC.get(DS1307_YR,false));
  sms.concat("\n");
  sms.concat("Temperatura: ");
  sms.concat(GenerarDigito(LeerTemperatura()));
  sms.concat(" grados");
  sms.concat("\n");
  sms.concat("Humedad: ");
  sms.concat(GenerarDigito(LeerHumedad()));
  sms.concat("%");
  sms.concat("\n");
  sms.concat("Modo: ");
  sms.concat(aux2);
  sms.concat("\n");	
  sms.concat("Camara: ");
  sms.concat(aux1);

  Serial.println(sms);
  return sms;
}

void EnviarSms(String tfno, String texto)
{
  String aux = "ArduTino informa:\n";
  aux.concat(texto);
  Serial1.print("AT");
  delay(TIEMPO_ESPERA_MODEM);
  Serial1.println("AT+CMGF=1");
  delay(TIEMPO_ESPERA_MODEM);
  Serial1.print("AT+CMGS=");
  delay(TIEMPO_ESPERA_MODEM);
  Serial1.print(tfno + "\r\n");
  delay(TIEMPO_ESPERA_MODEM);
  Serial1.println(aux);
  delay(TIEMPO_ESPERA_MODEM);
  Serial1.print(0x1A,BYTE);
  delay(TIEMPO_ESPERA_MODEM);
  Serial1.flush();
  delay(TIEMPO_ESPERA_MODEM);
}
///////////////////////////////////////////////////////////////
///                                                         ///
///                  Interruptor - Fisico                   ///
///                                                         ///
///////////////////////////////////////////////////////////////

void DesactivarEnvioSms(boolean modo)
{
  if ( modo == true)
  {
    // Pongo el invento en modo mantenimiento
    modo_mantinimiento = true;
    // Cambio el led y lo pongo en rojo
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, HIGH);
  }
  else
  {
    // Pongo el invento en modo normal
    modo_mantinimiento = false;
    // Cambio el led y lo pongo en verde
    digitalWrite(LED_VERDE, HIGH);		
    digitalWrite(LED_ROJO, LOW);
  }
}

///////////////////////////////////////////////////////////////
///                                                         ///
///                  Modem GSM				    ///
///                                                         ///
///////////////////////////////////////////////////////////////

void BorrarTodosSms() {
  // Solo haria falta borrar el primero, pero se borrar tres por capricho
  delay(TIEMPO_ESPERA_MODEM);
  Serial1.println("AT+CMGD=1");
  delay(1000);
  Serial1.println("AT+CMGD=2");
  delay(1000);
  Serial1.println("AT+CMGD=3");
  delay(1000);
}


void ConfigurarModem()
{
  MostrarInfo(CONFIGURAR, MODEM);
  Serial1.println("AT");
  delay(TIEMPO_ESPERA_MODEM);
  // Opciones de echo
  Serial1.println("ATE0");
  delay(TIEMPO_ESPERA_MODEM);
  // Identificacion de llamadas
  Serial1.println("AT+CLIP=1");
  delay(TIEMPO_ESPERA_MODEM);
  // Le decimos que nos devuelva por consola la entrada de sms
  Serial1.println("AT+CNMI=1,1");
  delay(TIEMPO_ESPERA_MODEM);
  // Borramos todos los sms
  BorrarTodosSms();
  Serial1.flush();
}


void ComprobarLineaModem(String linea)
{

  if (linea.startsWith("RING"))
  {
    Serial.println("Llamada nueva");
    Serial1.println("ATH");
    delay(TIEMPO_ESPERA_MODEM);
    //Serial.println("Cortamos la llamada");
  }
  if (linea.startsWith("+CLIP"))
  {
    //Serial.println("Podemos identificar el numero");

    int inicio = linea.indexOf('"') + 1; // Uno despues de "
    int final = linea.indexOf('"', inicio); // Uno antes de "
    // Solo me quedo con los ultimos 9 numeros del movil
    String numero = linea.substring(final-9,final); 

    numeroRing = 0;
    for (int a = 0 ; a < NUMERO_TOTAL_TELEFONOS ; a ++)
    {
      if (EEPROM.read(TAMANO_REGISTRO*a + DESPLAZAMIENTO_LLAMADAS) == 'A')
      {
        telefono = ObtenerTelefono(a);
      }
      if (numero == telefono)
      {
        unsigned long millisAhora = millis();
        // Solo admito una llamada cada 20 sg, para prevenir loops
        if (millisAhora - millisLlamada > 20000)
        {
          Serial.println(millisAhora - millisLlamada);
          millisLlamada = millis();
          String sms = CrearCuerpoSms();
          // Mostramos en el display que enviamos un sms
          MostrarInfo("Enviando estado", ENVIANDO_SMS);
          EnviarSms(telefono, sms);
          MostrarInfo("Evniando estado", ENVIADO_SMS);
          limpiarPantalla = true;
          millisAntes = millis() - 18000; 
        }
      }

    }


  }
  if (linea.startsWith("+CMTI: \"SM\""))
  {
    //Serial.println("Nos ha entrado un sms");
    //Serial.println("Vamos a leerlo");
    Serial1.flush();
    Serial1.println("at+cmgr=1");
    delay(400);
  }

  if (linea.startsWith("+CMGR:"))
  {
    // borramos los sms
    BorrarTodosSms();
    String auxSms;
    // El mensaje lo pongo como no valido hasta comprobarlo
    smsValido = false;

    //Serial.println("El numero del sms es");
    int b = 0;
    for (int a = 0; a < 4 ; a++)
    {
      //Recorremos y nos quedamos con la comilla final
      b = linea.indexOf('""', b) + 1;
    }
    auxSms = linea.substring(b-10,b-1);

    for (int a = 0 ; a < NUMERO_TOTAL_TELEFONOS ; a ++)
    {
      if (EEPROM.read(TAMANO_REGISTRO*a + DESPLAZAMIENTO_LLAMADAS) == 'A')
      {
        telefono = ObtenerTelefono(a);
      }
      if (auxSms == telefono)
      {
        smsValido = true;
        origenSms = telefono;
      }

    }		

  }

  if (linea.toLowerCase().startsWith("p") && smsValido == true)
  {
    // Mostramos en el display que enviamos un sms
    MostrarInfo("Ejecutando orden", ENVIANDO_SMS);
    smsValido = false;
    digitalWrite(INTERRUPTOR_SECADO, HIGH);
    delay(5000);
    digitalWrite(INTERRUPTOR_SECADO, LOW);
    String sms2 = "Orden ejecutada\n";
    sms2.concat(CrearCuerpoSms());
    EnviarSms(origenSms, sms2);
    MostrarInfo("Orden ejecutada", ENVIADO_SMS);
    limpiarPantalla = true;
    millisAntes = millis() - 18000; 
  }	


}

///////////////////////////////////////////////////////////////
///                                                         ///
///                  Funciones de fecha                     ///
///                                                         ///
///////////////////////////////////////////////////////////////

void EditarFecha()
{
  modo_teclado = EDICION;
  modo_fecha = true;
  
  vectorFecha[0] = RTC.get(DS1307_HR,true);
  vectorFecha[1] = RTC.get(DS1307_MIN,false);
  vectorFecha[2] = RTC.get(DS1307_DATE,false);
  vectorFecha[3] = RTC.get(DS1307_MTH,false);
  vectorFecha[4] = RTC.get(DS1307_YR,false) - 2000;
  
  // Ahora hacemos la parte del display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hora:");
  lcd.setCursor(11,0);
  lcd.print(GenerarDigito(vectorFecha[0]));
  lcd.print(":");
  lcd.print(GenerarDigito(vectorFecha[1]));
  lcd.setCursor(0,1);
  lcd.print("Fecha:");
  lcd.setCursor(8,1);
  lcd.print(GenerarDigito(vectorFecha[2]));
  lcd.print("/");
  lcd.print(GenerarDigito(vectorFecha[3]));
  lcd.print("/");
  lcd.print(GenerarDigito(vectorFecha[4]));
  lcd.setCursor(12,0);
  lcd.cursor();
  fechaIndice = 0; // Pongo el indice de posicion al principio
}


void EditarFechaHorizontal(int aux)
{
  int linea = 0;
  // Desplazamiento a la izquierda
  if (aux == -1 && fechaIndice > 0 )
  {
    fechaIndice -= 1;
  }
  // Desplazamiento a la derecha
  if ( aux == 1 && fechaIndice < 4)
  {
    fechaIndice += 1; 
  }
  
  // Ahora comprobamos la linea en la que estamos 
  if (fechaIndice > 1)
  {
    linea = 1;
  }
  lcd.setCursor(vectorPosicionesFecha[fechaIndice], linea);
}

void EditarFechaVertical(int aux)
{
  int linea = 0;
  // Matriz para saber los maximos y mÃ­nimos valores que puede alcanzar cada campo en la fecha
  int vectorFechaMaximo [5] = { 23, 59, 31, 12, 99 };
  int vectorFechaMinimo [5] = { 00, 00, 01, 01, 10 };
  if ( aux == 1 )
  { 
    // Si no hemos llegado al valor mayor sumamos uno
    if (vectorFecha[fechaIndice] < vectorFechaMaximo[fechaIndice])
    {
      vectorFecha[fechaIndice] += 1;
    }
    // Si hemos llegado a valor mayor volvemos al menor
    else
    {
      vectorFecha[fechaIndice] = vectorFechaMinimo[fechaIndice];
    }
  }
  // Restando
  if ( aux == -1 )
  {
    // Si no hemos llegado al valor menor restamos uno
    if (vectorFecha[fechaIndice] > vectorFechaMinimo[fechaIndice])
    {
      vectorFecha[fechaIndice] += -1;
    }
    // Si hemos llegado a valor menor volvemos al mayor
    else
    {
      vectorFecha[fechaIndice] = vectorFechaMaximo[fechaIndice];
    }
  }
    // Ahora comprobamos la linea en la que estamos 
  if (fechaIndice > 1)
  {
    linea = 1;
  }
  lcd.setCursor((vectorPosicionesFecha[fechaIndice])-1, linea);
  lcd.print(GenerarDigito(vectorFecha[fechaIndice]));
  lcd.setCursor(vectorPosicionesFecha[fechaIndice], linea);
}

void GuardarFecha()
{
  int diasMeses [12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  
  // Miramos si el ano es bisiesto
  if ( ComprobarAnoBisiesto(vectorFecha[4])) {
    // Le sumamos el dia en febrero del ano bisiesto
    diasMeses[1] = 29;
  }
  // Comprueba que no tenga mas dias que los que tiene el mes
  if ( diasMeses[(vectorFecha[3]-1)] < vectorFecha[2] ){
    // La fecha es incorrecta
    // Mostramos un error por el display
    MostrarInfo("Error: Fecha", "no correcta");
    // Volvemos a la editar la fecha
    EditarFecha();
  }
  // La fecha es la correcta y solo nos queda guardarla en el RTC
  else 
  {
    // Paramos el reloj
    RTC.stop();
    // Ponemos la nueva fecha
    RTC.set(DS1307_SEC,00);     // Segundos
    RTC.set(DS1307_HR, vectorFecha[0]);      // Horas 
    RTC.set(DS1307_MIN,vectorFecha[1]);     // Minutos 
    RTC.set(DS1307_DOW,CalcularDiaSemana(vectorFecha[4],vectorFecha[3],vectorFecha[2]));      // Dia de la semana
    RTC.set(DS1307_DATE,vectorFecha[2]);    // Dia del mes 
    RTC.set(DS1307_MTH,vectorFecha[3]);     // Mes 
    RTC.set(DS1307_YR,vectorFecha[4]);      // Ano 
    // Arrancamos el reloj
    RTC.start();   
    // Quitamos el guion bajo  del cursor
    lcd.noCursor();
    // Mostramos la informacion
    MostrarInfo(CAMBIO,CORRECTO);
    // Salimos del modo registro
    modo_teclado = MENU;
    // Salimos del modo editar fecha
    modo_fecha = false;
    // Mostramos el menu
    Root.display();
  }

}

int ComprobarAnoBisiesto(int ano){
  return ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0);
}

int CalcularDiaSemana(int y, int m, int d)	/* 0 = Sunday */
{
	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	y -= m < 3;
	return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}
///////////////////////////////////////////////////////////////
///                                                         ///
///                  Funciones auxiliares                   ///
///                                                         ///
///////////////////////////////////////////////////////////////

String ObtenerModo()
{
  if (grupo == true)
  {
    return "gru";
  }
  else if (red == true)
  {
    return "red";
  }
  else
  {
    return "des";
  }
}

String ObtenerModoCamara()
{
  if (digitalRead(TESTIGO_SECADO) == HIGH)
  {
    return "on ";
  }
  else 
  {
    return "off";
  }
}


String ObtenerTelefono(int movil)
{
  String tfno;
  for (int a = 0 ; a < LONGITUD_TELEFONO ; a ++)
  {
    tfno.concat(EEPROM.read(movil*TAMANO_REGISTRO+a));
  }
  return tfno;
}


String GenerarDigito(int numero)
{
	String digito;
	char aux[2];
	if ( numero < 10 )
	{
		digito.concat(0);
	}
	itoa(numero, aux, 10);
	digito.concat(aux);
	return digito;
}

String GenerarTextoAlertaDisplay(boolean estado, int alerta)
{

  String aux1;
  String aux2;
  if (estado == true)
  {
    aux1 = "Arranque ";
  }
  else
  {
    aux1 = "Parada ";
  }
  if (alerta == RED)
  {
    aux2 = "red";
  }
  else if (alerta == GRUPO)
  {
    aux2 = "grupo";
  }
  else
  {
    aux2 = "camara";
  }
  
  aux1.concat(aux2); // Construimos la alerta
  
  return aux1; // Devolvemos la alerta
  
  
}
///////////////////////////////////////////////////////////////
///                                                         ///
///                         Setup                           ///
///                                                         ///
///////////////////////////////////////////////////////////////






//# Setup
//# ==================================
void setup()
{
  // Inicializar el display
  lcd.begin(FILAS_LCD,COLUMNAS_LCD);
  lcd.createChar(0, flechaArriba);
  lcd.createChar(1, flechaAbajo); 
  MostrarInfo("Bienvenido a", NOMBRE_APLICACION); 
  



  // Defino los las entradas testigos de red y grupo
  pinMode(RED, INPUT);
  pinMode(GRUPO, INPUT);

  // Defino el testigo de la camara
  pinMode(TESTIGO_SECADO, INPUT);

  // Defino el interruptor de arranque de la camara
  pinMode(INTERRUPTOR_SECADO, OUTPUT);
  //~ ActivarCamara();

  // Defino el puerto del interruptor manual de corte de sms
  pinMode(INTERRUPTOR, INPUT);

  // Defino los led de interruptor manual de corte de sms
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);


  // Iniciamos el sensor de temperatura
  //sensorTemperatura.begin(PIN_TEMPERATURA);
  lcd.createChar(3, celsius);

  // Iniciamos el puerto del movil
  Serial1.begin(9600);
  ConfigurarModem();


  // Inciamos el Serial de consola
  Serial.begin(9600);
  Serial.println("DEBUG:");
  Serial.println("================");

  // Iniciamos el menu
  IniciarMenu();

}




///////////////////////////////////////////////////////////////
///                                                         ///
///                         Loop                            ///
///                                                         ///
///////////////////////////////////////////////////////////////

//# LOOP
//# ==================================
void loop()
{

  //////////////////////////////////////////////////////////////
  //                                                          //
  //                Inicio parte interruptor                  //
  //                                                          //
  //////////////////////////////////////////////////////////////

  if (digitalRead(INTERRUPTOR) == HIGH)
  {
    DesactivarEnvioSms(true);
  }
  else
  {
    DesactivarEnvioSms(false);
  }

  //////////////////////////////////////////////////////////////
  //                                                          //
  //                Inicio parte electrica                    //
  //                                                          //
  //////////////////////////////////////////////////////////////

  //~ // Comprobamos la tension en la red
  //~ if (digitalRead(RED) == HIGH)
  //~ {
    //~ // Tenemos tension
//~ 
    //~ // Compruebo si hay tension en grupo, el caso que ha vuelto la red
    //~ // Si es asi espero 0.2 sg para comprobar que es estable
    //~ // Vuelvo a leer si es estable marco
    //~ if (digitalRead(GRUPO) == HIGH && red == false)
    //~ {
      //~ delay (200);
      //~ if (digitalRead(RED) == HIGH)
      //~ {
        //~ // Comprobamos si tenemos que enviar el sms de recuperacion
        //~ if ( red == false )
        //~ {
          //~ Serial.println("Ha vuelto la red");
          //~ red = true;
          //~ if (modo_mantinimiento == false)
          //~ {
            //~ EnviarSmsAlertaTension(RED, true);
          //~ }
        //~ }
      //~ }
    //~ }
    //~ else
    //~ {
      //~ // Este cambio nunca se deberÃ­a dar pero por si las moscas
      //~ // Teoricamente siempre que vuelve la tensiÃ³n el grupo debe
      //~ // estar levantado, pero nunca se sabe
      //~ if ( red == false)
      //~ {
        //~ red = true;
        //~ if ( modo_mantinimiento == false)
        //~ {
          //~ EnviarSmsAlertaTension(RED, true);
        //~ }
      //~ }
    //~ }
//~ 
  //~ }
  //~ else
  //~ {
    //~ // No tenemos tension
    //~ // Volvemos a leer el valor pasado 0.2 segundos
    //~ delay(200);
    //~ if (digitalRead(RED) == LOW)
    //~ {
//~ 
      //~ // Comprobamos que no hemos enviado el sms
      //~ if (red == true )
      //~ {
        //~ Serial.println("Caida de red");
        //~ red = false;
        //~ // Marcamos el tiempo en que se cayo la red para la proxima
        //~ // comprobacion del estado de la camara
        //~ millisCaidaElectrica = millis();
        //~ comprobrar_camara = false;
        //~ if (modo_mantinimiento == false)
        //~ {
          //~ EnviarSmsAlertaTension(RED, false);
        //~ }
      //~ }
    //~ }
  //~ }

  //~ // Comprobamos el estado del grupo
  //~ if (digitalRead(GRUPO) == HIGH)
  //~ {
    //~ // El grupo no esta en marcha ??
    //~ if ( grupo == false )
    //~ {
      //~ Serial.println("Ha arrancado el grupo");
      //~ grupo = true;
      //~ if (modo_mantinimiento == false)
      //~ {
        //~ EnviarSmsAlertaTension(GRUPO, true);
      //~ }
    //~ }
  //~ }
  //~ else
  //~ {
    //~ if ( grupo == true)
    //~ {
      //~ Serial.println("Ha parado el grupo");
      //~ grupo = false;
      //~ if (modo_mantinimiento == false)
      //~ {
        //~ EnviarSmsAlertaTension(GRUPO, false);
      //~ }
    //~ }
  //~ }

  // Obtengo el estado de la red
  String alerta ; // usada para poner el texto de la alerta
  if (digitalRead(RED) == HIGH) 
  {
    auxRed = true;
  }
  else
  {
    auxRed = false;
  }
  if (digitalRead(GRUPO) == HIGH)
  {
    auxGrupo = true;
  }
  else
  {
    auxGrupo = false;
  }
  
  
  if ( auxRed != red )
  {
    red = auxRed ;
    alerta = GenerarTextoAlertaDisplay(red, RED);
    Serial.println(alerta);
    
    if ( red == false)
    {
      millisCaidaElectrica = millis() ;
      comprobrar_camara = false;
    }
    if ( modo_mantinimiento == false)
    {
      // Mostramos en el display que enviamos un sms
      MostrarInfo(alerta, ENVIANDO_SMS);
      EnviarSmsAlertaTension(red, RED);
      MostrarInfo(alerta, ENVIADO_SMS);
      limpiarPantalla = true;
      millisAntes = millis() - 18000; 
      
      
    } 
  }
  
  if ( auxGrupo != grupo )
  {
    grupo = auxGrupo;
    alerta = GenerarTextoAlertaDisplay(grupo, GRUPO);
    Serial.println(alerta);
    if (modo_mantinimiento == false)
    {
      // Mostramos en el display que enviamos un sms
      MostrarInfo(alerta, ENVIANDO_SMS);
      EnviarSmsAlertaTension(GRUPO, grupo);
      MostrarInfo(alerta, ENVIADO_SMS);
      limpiarPantalla = true;
      millisAntes = millis() - 18000; 

    }
  }

  //////////////////////////////////////////////////////////////
  //                                                          //
  //                Inicio parte de la camara                 //
  //                                                          //
  //////////////////////////////////////////////////////////////
  unsigned long millisCamara = millis();
  
  // Miramos si han pasado el tiempo prudencial desde que se 
  // ha caido la red para volver a comprobar la camara
  if ( millisCamara - millisCaidaElectrica > TIEMPO_ESPERA_CAIDA_ELECTRICA_COMPROBAR_CAMARA*1000 && comprobrar_camara == false)
  {
    comprobrar_camara = true;
  }
  

  if (digitalRead(TESTIGO_SECADO) == HIGH)
  {
    auxCamara = true;
  }
  else
  {
    auxCamara = false;
  }
  // Si esta la bandera de comprobar la camara comprobamos su estado
  if ( comprobrar_camara == true )
  {
    if ( auxCamara !=  camara)
    {
      camara = auxCamara;
      alerta = GenerarTextoAlertaDisplay(camara, TESTIGO_SECADO);
      Serial.println(alerta);
      if (modo_mantinimiento == false)
      {
        // Mostramos en el display que enviamos un sms
        MostrarInfo(alerta, ENVIANDO_SMS);
        EnviarSmsAlertaCamara(camara);
        MostrarInfo(alerta, ENVIADO_SMS);
        millisAntes = millis() - 18000; 
        MostrarSensores(true);
      }
    }
  }

  
  //////////////////////////////////////////////////////////////
  //                                                          //
  //                Inicio parte del teclado                  //
  //                                                          //
  //////////////////////////////////////////////////////////////  
  valor = LeerTeclado();
  if ( valor >=0 )
  {
    millisAntes = millis();
    limpiarPantalla = true;

    int aux = modo_teclado; // Recojo el modo antes de entrar en los 
    // al entrar en la funciones puede cambiar 
    ComprobarTecla(valor, modo_teclado);
    if ( aux == MENU && valor == TECLA_SELECCIONAR)
    {												
      ComprobarFuncion();
    }


  } 


  //////////////////////////////////////////////////////////////
  //                                                          //
  //                Inicio sensores en pantalla               //
  //                                                          //
  ////////////////////////////////////////////////////////////// 
  // Parte mostrar sensores por pantalla
  unsigned long millisAhora = millis();
  //~ Serial.print("millisAhora: ");
  //~ Serial.print(millisAhora);
  //~ Serial.print("\n");
  //~ Serial.print("millisAntes: ");
  //~ Serial.print(millisAntes);
  //~ Serial.print("\n");

  // Comprobamos que han pasado mas de n segundos sin pulsar nada
  if (millisAhora - millisAntes > TIEMPO_ESPERA*1000)
  {
    MostrarSensores(limpiarPantalla);
  }

  //////////////////////////////////////////////////////////////
  //                                                          //
  //                Inicio lectura modem gsm                  //
  //                                                          //
  ////////////////////////////////////////////////////////////// 

  String texto;
  if(Serial1.available() > 0)
  {
    //Serial.println("Estoy en el bucle");
    // Comprobamos si es nueva linea		
    if((caracter = Serial1.read()) == 10) {  
      //Serial.println("Estoy en el bucle 1");
      unsigned long millisSerial = millis(); 
      while( Serial1.available() > 0) {
        unsigned long millisSerial1 = millis();
        //Serial.println("Estoy en el bucle 2");
        caracter = Serial1.read();
        // Comprobamos si es nueva linea o retorno de carro
        if((caracter == 10)||(caracter == 13)) 
        { 
          //Serial.println("Estoy en el bucle 4");
          break;                      
        }
        //Serial.println("Estoy en el bucle 5");
        texto.concat(caracter);

      }
    }
    ComprobarLineaModem(texto);
  }

}
