/*
Extractos de códigos para poner en ArduTino
Son sólo ideas que puedes ayudar en el desarrollo
*/

/*
Nombre: ComprobarFuncion
Fichero: Controlador.pde (717)
El objetivo de la esta función es simplemenete comprobar que id nos
pasa el menú y el función del mismo llamar a la función correcta
*/

void ComprobarFuncion()
{
	// Quitamos las opciones del display
	lcd.noCursos();
	
	// Ahora comprobamos el id del menú en el que estamos
	// Nota: Esto se tendría que modificar, la variable id debería ser
	// privada y acceder a ella mediante un método.
	// int opcion = Root.getID(); podría ser
	int opcion = Root.id;
	
	if (opcion == 1)
	{
		GenerarMenuNumerosAdmitidos();
	}
	else if (opcion == 2)
	{
		EditarFecha();
	}
	else if (opcion == 3)
	{
		ConfigurarModem();
		MostrarInfo(CONFIGURAR, MODEM);	
	}
	else if (opcion ==4)
	{
		MostrarInfo(NOMBRE_APLICACION, VERSION_APLICACION);
	}
	else
	{
		// El id es tiene 3 digitos 1[telefono_id][funcion_id]
		int aux = opcion - 100;
		int telefono_id = aux / 10 -1; // El id empieza por 0
		int funcion_id = aux % 10;
		
		// Ahora sólo tengo que saber que funcion debo llamar, hay 5 niveles definidos
		if (funcion_id == 1)
		{
			// Envio sms
			CambiarMenusActivacion(telefono_id, ENVIAR_SMS);
		}
		else if (funcion_id ==2)
		{
			// Permitir llamadas
			CambiarMenusActivacion(telefono_id, PERMITIR_LLAMADAS);
		}
		else if (funcion_id == 3)
		{
			// Editar numero
			EditarNumero(telefono_id);
		}
		else if (funcion_id == 4)
		{
			//Borrar Numero
			BorrarNumero(telefono_id);
		}
		else if (funcion_id ==5)
		{
			// Enviar sms de pruebas
			EnviarSmsDirecto(telefono_id); 
		}
	}
	// Si elegimos editar telefono no queremos mostar ningun menu
	// por eso revisamos aqui que no hemos cambiado de modo de teclado
	
	if (modo_teclado == MENU)
	{
		GenerarMenuNumerosAdmitidos();
		Root.display();
	}	
	
}