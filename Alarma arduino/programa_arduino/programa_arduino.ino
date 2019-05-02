#include <LiquidCrystal_I2C.h> // Librerias para la lcd
#include<wire.h> 
#define distancialimite 10 //Introducir distancia a la que está el tope, si es menor, la alarma se activa
LiquidCrystal_I2C lcd(0x27,16,2); //Pin SDA del lcd debe ir en A4 y pin SCL debe ir en A5


//Prototipo funciones
void InicioLcd(); //Inicia la pantalla LCD
void SonarAlarma(int); //Reproduce el sonido de la alarma, debes indicarle el pin del zumbador
int MedirDistancia(int); //Mide la distancia desde el sensor ultrasonido, debes indicarle el pin de Echo

int Echo=13;
int Trig=12;
int Zumbador=3;
int Led[]={7,8}; //Led[0]=LedRojo Led[1]=LedVerde

void setup()
{
  int i;
  
  Serial.begin(9600);
  pinMode(Zumbador,OUTPUT);
  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);
   for(i=0;i<2;i++)
  {
    pinMode(Led[i],OUTPUT);
  }
  InicioLcd();

  //Juego de luces al iniciar
  for(i=0;i<2;i++)
  {
    digitalWrite(Led[i],HIGH);
    delay(50);
    digitalWrite(Led[i],LOW);
    delay(50);
  }
  for(i=1;i>=0;i--)
  {
    digitalWrite(Led[i],HIGH);
    delay(50);
    digitalWrite(Led[i],LOW);
    delay(50);
  }
}

void loop() 
{
  long distancia;
  char active=0;
  
//ALARMA DESACTIVADA
  noTone(Zumbador);
  lcd.setCursor(0,0);
  lcd.print("     ALARMA    ");
  lcd.setCursor(0,1);
  lcd.print("  DESACTIVADA    ");
  digitalWrite(Led[1],HIGH);
  active=Serial.read();
  
//ALARMA ACTIVA
  if(active=='a')
  { 
   //Primero borramos lcd y apagamos LedVerde
   lcd.clear();
   digitalWrite(Led[1],LOW);
   delay(500);
  
   while(active!='o')
   {
     //Cambiamos Led
     digitalWrite(Led[0],HIGH);
     
     //Cambiamos lcd
     lcd.setCursor(0,0);
     lcd.print("     ALARMA    ");
     lcd.setCursor(0,1);
     lcd.print("    ACTIVADA    ");
    
     //Activamos sensor ultrasonido   
     digitalWrite(Trig,LOW);
     delayMicroseconds(4);
     digitalWrite(Trig,HIGH);
     delayMicroseconds(10);
     digitalWrite(Trig,LOW);  
    
     //Medir distancia
     distancia=MedirDistancia(Echo);
     Serial.println(distancia);
     delay(50);

     //Deteccion de paso
     if(distancia<distancialimite)
    {
      SonarAlarma(Zumbador);
    }
    else
    {
     noTone(Zumbador);
    }
    active=Serial.read();
  }

//Si hemos apagado la alarma nosotros, apagar LedRojo y borrar lcd
  if(active=='o')
  {
   lcd.clear();
   digitalWrite(Led[0],LOW);
   delay(500);
  }
 }
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
  delay(2000);
  lcd.clear();
}

void SonarAlarma(int zumb)
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
