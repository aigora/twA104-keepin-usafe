
void TonoAlarma(int); //Reproduce el sonido de la alarma, debes indicarle el pin del zumbador

void setup()
{

}

void loop() 
{


}

void TonoAlarma(int Zumbador)
{
  int i;
  float sinVal;
  int toneVal;
  
   for(i=0;i<180;i++)
  {
    sinVal=(sin(i*(3.1412/180)));
    toneVal=2000+(int)(sinVal*1000);
    tone(Zumbador,toneVal);
    delay(2);
  }
}
