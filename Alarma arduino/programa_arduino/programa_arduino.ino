//Prototipo funciones
void TonoAlarma(int); //Reproduce el sonido de la alarma, debes indicarle el pin del zumbador
int MedirDistancia(int); //Mide la distancia desde el sensor ultrasonido, debes indicarle el pin de Echo

void setup()
{

}

void loop() 
{


}

//Funciones

void TonoAlarma(int Zumbador)
{
  int i;
  float sinVal;
  int toneVal;
  
   for(i=0;i<180;i++)
  {
    sinVal=(sin(i*(3.1412/180))); //Señal senoidal para el zumbador. Solo valores positivos
    toneVal=2000+(int)(sinVal*1000);
    tone(Zumbador,toneVal); //Tono zumbador
    delay(2);
  }
}

int MedirDistancia(int Echo)
{
  long duracion;
  long distancia;

  duracion=((pulseIn(Echo,HIGH))/2); //La señal sonora va y vuelve, por eso se divide por 2 
  distancia=duracion/29.2; //Velocidad del sonido= 1cm/29.2 microsegundos

  return distancia;
}
