#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
//Biblioteca comunicación serial arduino
#include"SerialPort.h"

#define MAX_DATA_LENGTH 255

//Funciones para comunicacion serial
	void autoConnect(SerialPort *arduino, char*);
	void Crear_Conexion(SerialPort *PuertoSerie, char *portName);
	void CerrarConexion(SerialPort * PuertoSerie);
	int readSerialPort(SerialPort * PuertoSerie, char *buffer, unsigned int buf_size);
	int writeSerialPort(SerialPort *PuertoSerie, char *buffer, unsigned int buf_size);

// Funciones prototipo


void main()
{
	printf("			SISTEMA DE ALARMAS KEEP'N YOU SAFE\n\n");

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
	autoConnect(arduino, incomingData);

}

void autoConnect(SerialPort *arduino, char *incomingData)
{
	char sendData = 0;

	// Espera la conexión con Arduino
	while (!isConnected(arduino))
	{
		Sleep(100);
		Crear_Conexion(arduino, arduino->portName);
	}
	//Comprueba si arduino está connectado
	if (isConnected(arduino))
	{	
		printf("Conectado con Arduino en el puerto %s\n", arduino->portName);
	}
	
	// APLICACIÓN
	while (isConnected(arduino))
	{
	
	}

	if (!isConnected(arduino))
	{
		system("CLS");
		printf("Se ha perdido la conexion con Arduino\n");
		system("PAUSE");
	}
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