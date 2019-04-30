/*
* Author: Manash Kumar Mandal
* Modified Library introduced in Arduino Playground which does not work
* This works perfectly
* LICENSE: MIT
*/

#include "SerialPort.h"

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
        DCB dcbSerialParameters = {0};

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
    return ;
}

void CerrarConexion(SerialPort * PuertoSerie)
{
    if (PuertoSerie->connected)
	{
        PuertoSerie->connected = 0;
        CloseHandle(PuertoSerie->handler);
    }
}

int readSerialPort(SerialPort * PuertoSerie,char *buffer, unsigned int buf_size)
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

int writeSerialPort(SerialPort *PuertoSerie,char *buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(PuertoSerie->handler, (void*) buffer, buf_size, &bytesSend, 0))
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
