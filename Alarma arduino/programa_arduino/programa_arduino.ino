#include <LiquidCrystal_I2C.h> // Librerias para la lcd
#include<wire.h> 
LiquidCrystal_I2C lcd(0x27,16,2); //Pin SDA del lcd debe ir en A4 y pin SCL debe ir en A5

//Prototipo funciones
void InicioLcd(); //Inicia la pantalla LCD
void TonoAlarma(int); //Reproduce el sonido de la alarma, debes indicarle el pin del zumbador
int MedirDistancia(int); //Mide la distancia desde el sensor ultrasonido, debes indicarle el pin de Echo

void setup()
{
  InicioLcd();
}

void loop() 
{


}


//Funciones
void InicioLcd()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   SISTEMAS    "); 
  lcd.setCursor (0,1);
  lcd.print("KEEP'N YOU SAFE");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   BIENVENIDO  ");
  delay(3000);
  lcd.clear();
}

void TonoAlarma(int zumb)
{
  int i;
  float sinVal;
  int toneVal;
  
   for(i=0;i<180;i++)
  {
    sinVal=(sin(i*(3.1412/180))); //Señal senoidal para el zumbador. Solo valores positivos
    toneVal=2000+(int)(sinVal*1000);
    tone(zumb,toneVal); //Tono zumbador
    delay(2);
  }
}

int MedirDistancia(int ech)
{
  long duracion;
  long distancia;

  duracion=((pulseIn(ech,HIGH))/2); //La señal sonora va y vuelve, por eso se divide por 2 
  distancia=duracion/29.2; //Velocidad del sonido= 1cm/29.2 microsegundos

  return distancia;
}
