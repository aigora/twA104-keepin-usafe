#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>
//Biblioteca comunicación serial arduino
#include"SerialPort.h"

#define length 16 //Longitud máxima permitida en la clave de seguridad de la alarma
#define MAX_DATA_LENGTH 255
#define ON 1
#define OFF 0
#define DETECCION 2

//Funciones para comunicacion serial
void autoConnect(SerialPort *arduino, char*, char*);
void Crear_Conexion(SerialPort *PuertoSerie, char *portName);
void CerrarConexion(SerialPort * PuertoSerie);
int readSerialPort(SerialPort * PuertoSerie, char *buffer, unsigned int buf_size);
int writeSerialPort(SerialPort *PuertoSerie, char *buffer, unsigned int buf_size);

// Funciones prototipo
char* Password(); //Función para introducir contraseña con asignación dinámica
char* DefinePass(int flag); //Función para definir una contraseña para la alarma. Si se quiere cambiar contraseña el flag debe ser 1, si no se pone 0
void registro();//Funcion para ver cuando se activo la alarma
void fecha();//Funcion para poner cuando se activa la alarma en un txt
void Alarm(int flag, SerialPort *arduino, char* pass,char* pass_aux,int* act); //Funcion para activar o desactivar la alarma

void main()
{
	char *pass; //Contraseña de la alarma

	printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");
	pass = DefinePass(0); //indicamos la contraseña de la alarma

//CONEXION CON ARDUINO
	//Arduino SerialPort object
	SerialPort *arduino;
	// Puerto serie en el que está Arduino
	char* portName = "COM5";
	// Buffer para datos procedentes de Arduino
	char incomingData[MAX_DATA_LENGTH];

	// Crear estructura de datos del puerto serie
	arduino = (SerialPort *)malloc(sizeof(SerialPort));
	// Apertura del puerto serie
	Crear_Conexion(arduino, portName);
	autoConnect(arduino, incomingData, pass); //Dentro de esta funcion escribiremos la aplicacion
}


void autoConnect(SerialPort *arduino, char *incomingData, char *pass)
{
	char sendData = 0;
	int act = 0; //Flag que se activa cuando la alarma esta activada

	// Espera la conexión con Arduino
	while (!isConnected(arduino))
	{
		Sleep(100);
		Crear_Conexion(arduino, arduino->portName);
	}
	//Comprueba si arduino está connectado
	if (isConnected(arduino))
	{
		system("CLS");
		printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");
		printf("Conectado con Arduino en el puerto %s\n\n", arduino->portName);
		Sleep(7000);
	}

	// APLICACIÓN!! (Se ejecuta mientras el arduino esta conectado)
	while (isConnected(arduino))
	{
		char opc;
		char *pass_aux = NULL;

		readSerialPort(arduino, incomingData, MAX_DATA_LENGTH); //Lee puerto serie
		if (incomingData[0]=='s') //Si el arduino ha detectado presencia
		{
			Alarm(DETECCION, arduino, pass, pass_aux, &act);
			continue;
		}

		system("CLS");
		printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");
		printf("Conectado con Arduino en el puerto %s\n\n", arduino->portName);

		printf("Que desea hacer?\n\n1.Activar alarma\n2.Desactivar alarma\n3.Ver historial de detecciones\n4.Cambiar clave de seguridad\n\n5.Salir\n\n");
		opc = _getch();

		readSerialPort(arduino, incomingData, MAX_DATA_LENGTH); //Lee puerto serie
		if (incomingData[0] == 's') //Si el arduino ha detectado presencia
		{
			Alarm(DETECCION, arduino, pass, pass_aux, &act);
			continue;
		}

		switch (opc)
		{
			case '1':
			{
				if (act != 1)
				{
					Alarm(ON, arduino, pass,pass_aux, &act);
				}
				break;
		}

			case '2':
			{
				if (act != 0)
				{
					Alarm(OFF, arduino, pass,pass_aux, &act);
				}
			break;
			}

			case '3':
			{
				registro();
				printf("\n\nPulse una tecla para continuar");
				_getch();
				break;
			}

			case '4':
			{
				printf("\nIntroduce la antigua clave de seguridad: ");
				pass_aux = Password();
				if (strcmp(pass, pass_aux) != 0)
				{
					printf("Clave incorrecta!\n\n");
					getch();
					break;
				}
				else
				{
					pass = DefinePass(1);
					break;
				}
			}

			case '5':
			{
				printf("Esta seguro de que que quiere salir? (s/n) ");
				do
				{
					opc = _getch();
				} while (opc != 's'&&opc != 'n');
					
				if (opc == 's')
				{
					opc = 0;
					if (act == 1)
					{
						printf("\nDesactivar alarma?(s/n)\n");
						do
						{
							opc = _getch();
						} while (opc != 's'&&opc != 'n');

						if (opc == 's')
						{
							Alarm(OFF, arduino, pass,pass_aux,&act);
						}
					}
					printf("\nHasta luego!");
					Sleep(700);
					exit(0);
				}
				else if (opc == 'n')
					break;
			}
			default:
				break;
		}
	}

	if (!isConnected(arduino))
	{
		printf("\nSe ha perdido la conexion con Arduino\n");
		system("PAUSE");
	}
}

void Alarm(int flag, SerialPort *arduino, char *pass,char *pass_aux, int *act)
{
	char sendData;
	
	if (flag != DETECCION)
	{
		printf("Introduzca la clave de seguridad: ");
		pass_aux = Password();
	}

		if (flag == ON)
		{
			if (strcmp(pass_aux, pass) == 0)
			{
				printf("Clave correcta\nAlarma activa\n\n");

				sendData = 'a'; //Activa arduino
				writeSerialPort(arduino, &sendData, sizeof(char));
				*act = 1; //Flag para indicar que la alarma esta activada
				printf("Pulse una tecla para continuar");
				_getch();
			}
			else
			{
				printf("Clave incorrecta\n\n");
				printf("Pulse una tecla para continuar");
				_getch();
			}
		}
	else if (flag == OFF)
	{
		if (strcmp(pass_aux, pass) == 0)
		{
			printf("Clave correcta\nAlarma desactivada\n\n");

			sendData = 'o'; //Desactiva arduino
			writeSerialPort(arduino, &sendData, sizeof(char));
			*act = 0; //Flag para indicar que la alarma se ha desactivado
			printf("Pulse una tecla para continuar");
			_getch();
		}
		else
		{
			printf("Clave incorrecta\n\n");
			printf("Pulse una tecla para continuar");
			_getch();
		}
	}

	else if (flag == DETECCION)
		{
			do
			{
				system("CLS");
				printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");
				printf("PRESENCIA DETECTADA!\n\nIntroduzca clave para desactivar: ");
				pass_aux = Password();
				if (strcmp(pass, pass_aux) != 0)
				{
					printf("Clave incorrecta!!\n");
					printf("Pulse una tecla para continuar");
					_getch();
				}
				else
				{
					printf("Clave correcta\nALARMA DESACTIVADA!\n\n");
					sendData = 'o'; //Desactiva el arduino
					writeSerialPort(arduino, &sendData, sizeof(char));
					*act = 0; //Flag para indicar que la alarma se ha desactivado
					printf("Pulse una tecla para continuar");
					_getch();
				}
			} while (strcmp(pass, pass_aux) != 0);
		}
}

char* DefinePass(int flag)
{
	FILE *filepass;
	char *pass1, *pass2,*pass_aux;
	int i = 0;
	errno_t err_pass;

	//Asignacion de memoria para la contraseña
	pass1 = (char*)malloc(sizeof(char)*(length + 1));
	*pass1 = NULL;
	pass_aux = (char*)malloc(sizeof(char)*(length + 1));
	*pass_aux = NULL;

	//Abrimos y leemos archivo de texto donde esta la contraseña
	err_pass = fopen_s(&filepass, "password.txt", "r");
	if (err_pass != NULL)
	{
		printf("El archivo no se ha abierto corretamente\n");
		getchar();
		fclose(filepass);
		exit(1);
	}

	//Si el flag esta activado, se cambia la contraseña que esta ya puesta
	if (flag == 1)
	{
		//Primero escribimos la clave que ya hay en un pass_auxiliar
		while (feof(filepass) == NULL)
		{
			fscanf_s(filepass, "%s", (pass_aux), _msize(pass_aux));
		}
		fclose(filepass);
		err_pass = fopen_s(&filepass, "password.txt", "w");
		fprintf(filepass, "\0");
		fclose(filepass);
		err_pass = fopen_s(&filepass, "password.txt", "r");
	}

	//Introducimos en pass1 la contraseña que haya definida en el archivo
	while (feof(filepass) == NULL)
	{
		fscanf_s(filepass, "%s", (pass1), _msize(pass1));
	}

	//Reasignamos memoria para optimizar
	pass1 = (char*)realloc(pass1, strlen(pass1) + 1);
	fclose(filepass);

	//Si no habia ninguna contraseña previamente definida, primero la creamos
	if (*pass1 == NULL)
	{
		do
		{
			printf("Defina una clave de hasta %d caracteres: ", length);
			pass1 = Password();
			printf("Introducela de nuevo: ");
			pass2 = Password();

			if (strcmp(pass1, pass2) == 0)
				printf("Clave definida correctamente\n");
			else
			{
				printf("Las claves no coinciden\n");
				printf("Pulse una tecla para continuar");
				_getch();
				system("CLS");
				printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");
			}

			//Si queriamos cambiar la clave y no coincide, ponemos la que habiamos grabado en el pass_auxiliar
			if (flag == 1 && strcmp(pass1, pass2) != 0)
			{
				strcpy(pass1, pass_aux);
				break;
			}
		} while (strcmp(pass1, pass2) != 0);

		err_pass = fopen_s(&filepass, "password.txt", "w");
		if (err_pass != NULL)
		{
			printf("El archivo no se ha abierto corretamente\n");
			printf("Pulse una tecla para continuar");
			_getch();
			fclose(filepass);
			exit(1);
		}

		fprintf(filepass, "%s", pass1);
		fclose(filepass);
		if (strcmp(pass1, pass2) == 0)
		{
			printf("Pulse una tecla para continuar");
			_getch();
			system("CLS");
		}
		return pass1;
	}
	//Si ya existia una contraseña creada,  la funcion la devuelve
	else
		return pass1;
}

char* Password()
{
	char *pass, c = 0;
	int i;
	//Asignacion de memoria para la contraseña
	pass = (char*)malloc((length)+1);
	if (pass == NULL)
	{
		printf("No hay memoria disponible\n");
		system("PAUSE");
		exit(1);
	}

	//Compobacion de que como mucho tiene los caracteres que hemos indicado. Cada vez que escribes un caracter, imprime *
	do
	{
		i = 0;

		while (c != 13) //Si no presionamos intro
		{
			if (i < (length + 1)) //Si la contraseña cabe en la cadena, se va introduciendo
			{
				c = _getch();

				if (i == 0 && c == 8)
					continue;

				if (c > 31 && c < 126)
				{
					pass[i] = c;
					printf("*");
				}

				else if (c == 8) //Si pulsa retroceso
				{
					printf("\b \b"); //Mueve cursor y borra anterior caracter
					c = NULL;
					i = i - 2;
				}
				else if (c != 13) //Eliminamos teclas especiales
				{
					_getch();
					continue;
				}
			}
			else //Si no entra siemplemente se imprimen los *, luego te mandara repetir contraseña
			{
				c = _getch();
				if (c != 8 && c != 13)
				{
					printf("*");
				}

				if (c == 8)
				{
					printf("\b \b");
					c = NULL;
					i = i - 2;
				}
				else if (c != 13)
				{
					_getch();
					continue;
				}
			}
			if (c == 13) //Si pulsa intro
			{
				if (i < ((length)+1))
					pass[i] = '\0';
				else
				{
					pass[(length)+1] = '\0';
				}
				i--;
				printf("\n");
			}
			i++;
		}
		if (i > length)
			printf("Error de longitud, introducela de nuevo: ");
		if (i == 0)
			printf("La clave no puede estar en blanco, introducela de nuevo: ");
		c = 0;
	} while (i > length || i < 1);

	//Reasignacion de memoria para optimizar
	pass = (char*)realloc(pass, i * sizeof(char) + 1);
	return pass; //Devuelve el puntero de la contraseña
}

void fecha() {
	time_t current_time;
	FILE *filetime;
	int i = 0;
	errno_t err1;

	err1 = fopen_s(&filetime, "history.txt", "a");
	if (err1 != NULL)
	{
		printf("El archivo no se ha abierto corretamente\n");
		getchar();
		fclose(filetime);
		exit(1);
	}
	current_time = time(NULL);
	fprintf(filetime, "La alarma se activo: %s", ctime(&current_time));
	printf("La alarma se activo: %s", ctime(&current_time));
	fclose(filetime);
}
void registro() {
	FILE *filetime;
	errno_t err1;
	char fecha[20];
	int num_pal, i;
	num_pal = 9;

	err1 = fopen_s(&filetime, "history.txt", "r");
	if (err1 != NULL)
	{
		printf("El archivo no se ha abierto corretamente\n");
		getchar();
		fclose(filetime);
		exit(1);
	}
	while (feof(filetime) == NULL)
	{
		for (i = 1; i < num_pal; i++)
		{
			fscanf_s(filetime, "%s ", fecha, 20);
			printf("%s ", fecha);
		}

		fscanf_s(filetime, "%s ", fecha, 20);
		printf("%s\n", fecha);
	}
	fclose(filetime);
}


//Funciones para la comunicacion serial
void Crear_Conexion(SerialPort *PuertoSerie, char *portName)
{

	PuertoSerie->connected = 0;
	PuertoSerie->portName = portName;
	PuertoSerie->handler = CreateFileA((portName),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (PuertoSerie->handler == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("ERROR: Handle was not attached. Reason: %s not available\n", portName);
		}
		else
		{
			printf("ERROR!!!");
		}
	}
	else
	{
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(PuertoSerie->handler, &dcbSerialParameters))
		{
			printf("failed to get current serial parameters");
		}
		else
		{
			dcbSerialParameters.BaudRate = CBR_9600;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(PuertoSerie->handler, &dcbSerialParameters))
			{
				printf("ALERT: could not set Serial port parameters\n");
			}
			else
			{
				PuertoSerie->connected = 1;
				PurgeComm(PuertoSerie->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
	return;
}

void CerrarConexion(SerialPort * PuertoSerie)
{
	if (PuertoSerie->connected)
	{
		PuertoSerie->connected = 0;
		CloseHandle(PuertoSerie->handler);
	}
}

int isConnected(SerialPort *PuertoSerie)
{
	if (!ClearCommError(PuertoSerie->handler, &PuertoSerie->errors, &PuertoSerie->status))
		PuertoSerie->connected = 0;
	return PuertoSerie->connected;
}

int readSerialPort(SerialPort * PuertoSerie, char *buffer, unsigned int buf_size)
{
	DWORD bytesRead;
	unsigned int toRead = 0;

	ClearCommError(PuertoSerie->handler, &PuertoSerie->errors, &PuertoSerie->status);

	if (PuertoSerie->status.cbInQue > 0)
	{
		if (PuertoSerie->status.cbInQue > buf_size)
		{
			toRead = buf_size;
		}
		else toRead = PuertoSerie->status.cbInQue;
	}

	memset(buffer, 0, buf_size);

	if (ReadFile(PuertoSerie->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

	return 0;
}

int writeSerialPort(SerialPort *PuertoSerie, char *buffer, unsigned int buf_size)
{
	DWORD bytesSend;

	if (!WriteFile(PuertoSerie->handler, (void*)buffer, buf_size, &bytesSend, 0))
	{
		ClearCommError(PuertoSerie->handler, &PuertoSerie->errors, &PuertoSerie->status);
		return 0;
	}
	else return 1;
}
