#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>
//Biblioteca comunicaci�n serial arduino
#include"SerialPort.h"

#define length 16 //Longitud m�xima de la contrase�a de la alarma
#define MAX_DATA_LENGTH 255

//Funciones para comunicacion serial
	void autoConnect(SerialPort *arduino, char*,char*);
	void Crear_Conexion(SerialPort *PuertoSerie, char *portName);
	void CerrarConexion(SerialPort * PuertoSerie);
	int readSerialPort(SerialPort * PuertoSerie, char *buffer, unsigned int buf_size);
	int writeSerialPort(SerialPort *PuertoSerie, char *buffer, unsigned int buf_size);

// Funciones prototipo
	char* Password(); //Funci�n para introducir contrase�a con asignaci�n din�mica
	char* DefinePass(int flag); //Funci�n para definir una contrase�a para la alarma. Si se quiere cambiar contrase�a el flag debe ser 1, si no se pone 0

void main()
{
	printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");
	
	char *pass; //Contrase�a de la alarma
	pass = DefinePass(0); //indicamos la contrase�a de la alarma

//CONEXION CON ARDUINO
	//Arduino SerialPort object
	SerialPort *arduino;
	// Puerto serie en el que est� Arduino
	char* portName = "COM5";
	// Buffer para datos procedentes de Arduino
	char incomingData[MAX_DATA_LENGTH];

	// Crear estructura de datos del puerto serie
	arduino = (SerialPort *)malloc(sizeof(SerialPort));
	// Apertura del puerto serie
	Crear_Conexion(arduino, portName);
	autoConnect(arduino, incomingData,pass); //Dentro de esta funcion escribiremos la aplicacion
}


void autoConnect(SerialPort *arduino, char *incomingData, char *pass)
{
	char sendData = 0;

	// Espera la conexi�n con Arduino
	while (!isConnected(arduino))
	{
		Sleep(100);
		Crear_Conexion(arduino, arduino->portName);
	}
	//Comprueba si arduino est� connectado
	/*if (isConnected(arduino))
	{	
	
		
	}*/
	
	// APLICACI�N!! (Se ejecuta mientras el arduino esta conectado)
	while (isConnected(arduino))
	{	
		char opc;
		char *pass_aux;

		system("CLS");
		printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");
		printf("Conectado con Arduino en el puerto %s\n\n", arduino->portName);
		
		printf("Que desea hacer?\n\n1.Activar alarma\n2.Desactivar alarma\n3.Ver historial de detecciones\n4.Cambiar clave de seguridad\n\n5.Apagar\n\n");
		do
		{
			opc=_getch();
		} while (opc < '1' && opc>'5');

		switch (opc)
		{
			case '1':
			{
				
				break;
			}

			case '2':
			{
				
				break;
			}

			case '3':
			{
				
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
					printf("\nEsta seguro de que que quiere salir? (s/n) ");				
				do
				{
					opc = _getch();
				} while (opc != 's'&&opc != 'n');

				if (opc == 's')
				{
					printf("\nHasta luego!");
					Sleep(700);
					exit(0);
				}
				else if (opc == 'n')
					break;
			}
		}
	}

	if (!isConnected(arduino))
	{
		printf("\nSe ha perdido la conexion con Arduino\n");
		system("PAUSE");
	}
}

char* DefinePass(int flag)
{
	FILE *filepass;
	char *pass1,*pass2;
	int i=0;
	errno_t err_pass;

	//Asignacion de memoria para la contrase�a
	pass1 = (char*)malloc(sizeof(char)*(length + 1));
	*pass1 = NULL;

	//Abrimos y leemos archivo de texto donde esta la contrase�a
	err_pass = fopen_s(&filepass, "password.txt", "r");
	if (err_pass != NULL)
	{
		printf("El archivo no se ha abierto corretamente\n");
		getchar();
		fclose(filepass);
		exit(1);
	}

	//Si el flag esta activado, se cambia la contrase�a que esta ya puesta
	if (flag == 1)
	{
		fclose(filepass);
		err_pass = fopen_s(&filepass, "password.txt", "w");
		fprintf(filepass,"\0");
		fclose(filepass);
		err_pass = fopen_s(&filepass, "password.txt", "r");
	}

	//Introducimos en pass1 la contrase�a que haya definida en el archivo
	while (feof(filepass) == NULL)
	{
		fscanf_s(filepass, "%s", (pass1), _msize(pass1));
	}

	//Reasignamos memoria para optimizar
	pass1 = (char*)realloc(pass1, strlen(pass1) + 1);
	fclose(filepass);

	//Si no habia ninguna contrase�a previamente definida, primero la creamos
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
				system("PAUSE");
				system("CLS");
				printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");
			}
		} while (strcmp(pass1, pass2) != 0);

		err_pass = fopen_s(&filepass, "password.txt", "w");
		if (err_pass != NULL)
		{
			printf("El archivo no se ha abierto corretamente\n");
			system("PAUSE");
			fclose(filepass);
			exit(1);
		}

		fprintf(filepass, "%s", pass1);
		fclose(filepass);
		system("PAUSE");
		system("CLS");
		return pass1;
	}
	//Si ya existia,  la funcion devuelve la contrase�a
	else
		return pass1;
}

char* Password()
{
	char *pass, c = 0;
	int i;
	//Asignacion de memoria para la contrase�a
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
			if (i < (length + 1)) //Si la contrase�a cabe en la cadena, se va introduciendo
			{
				c = _getch();

				if (i == 0 && c == 8)
					continue;

				if (c > 31 && c < 126)
				{
					pass[i] = c;
					printf("*");
				}

				else if (c == 8) //Pulsa retroceso
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
			else //Si no entra siemplemente se imprimen los *, luego te mandara repetir contrase�a
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
			if (c == 13) //Pulsa intro
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
	return pass; //Devuelve el puntero de la contrase�a
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

int isConnected(SerialPort *PuertoSerie)
{
	if (!ClearCommError(PuertoSerie->handler, &PuertoSerie->errors, &PuertoSerie->status))
		PuertoSerie->connected = 0;
	return PuertoSerie->connected;
}