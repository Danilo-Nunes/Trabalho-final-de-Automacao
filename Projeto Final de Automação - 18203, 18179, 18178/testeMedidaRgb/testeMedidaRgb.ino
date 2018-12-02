#include <SoftwareSerial.h>
#include <Stepper.h>
#include <Servo.h>

#define ldr A0
#define rx 3
#define tx 2
#define pServo 6
#define graus 10

Servo servo;
Stepper motor(360/graus, 8, 10, 9, 11);
SoftwareSerial bt(rx, tx);

//Matriz de floats para armazenar o valor das cores e a angulacao correspondente
float rgb[] = {0,0,0};
float matrizBranco[] = {0,0,0}; 
float matrizPreto[] = {0,0,0};
int angulacoes[] = {0, 0, 0, 0, 0, 0};

//Variável para armazenar a média das leitura e da cor do m&m atual
int mediaLeituras;
int corAtual;
int estado;
boolean jaCalibrou = false;
int rgbLed[] = {12, 13, 7};

void setup() {
  for (int i = 0; i < 3; i++)
    pinMode(rgbLed[i], OUTPUT);
  pinMode(ldr, INPUT);
  motor.setSpeed(50);
  servo.attach(pServo);
  Serial.begin(9600);
  servo.write(120);
  bt.begin(38400);
}

void loop() {
  if (bt.available() > 0)
  {
    estado = bt.readString().toInt();
  }
  if (estado == 1)
  {
    lerValores();
  }
  if (estado == 2)
  {
    while((bt.available() > 0)&&(estado == 2))
      estado = bt.readString().toInt();
  }
  if (estado == 3)
  {
    return;
  }
}

void mediaSensor(int n)
{
  int val;
  int soma;
  for (int i = 0; i < n; i++)
  {
    val = analogRead(ldr);
    soma += val;
    delay(10);
  }
  mediaLeituras = soma/n;
}
void lerValores()
{
  if (jaCalibrou)
  {
    //Calibrando o branco!   
    Serial.println("Calibrando o branco");   
    delay(5000);   
    for(int i = 0; i < 3; i++)
    {
      digitalWrite(rgbLed[i],HIGH);
      delay(100);     
      mediaSensor(5);               
      matrizBranco[i] = mediaLeituras;     
      digitalWrite(rgbLed[i],LOW);     
      delay(100);   
    }   
    
    //Calibrando o preto!   
    Serial.println("Calibrando o preto");   
    delay(5000);   
    for (int i = 0; i < 3; i++)
    {     
      digitalWrite(rgbLed[i],HIGH);     
      delay(100);     
      mediaSensor(5);     
      matrizPreto[i] = mediaLeituras;     
      digitalWrite(rgbLed[i],LOW);     
      delay(100);   
    }   
    
    //Avisa que a calibragem foi feita   
    Serial.println("Sensor Calibrado");
    delay(3000);
    jaCalibrou = true;
  }
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(rgbLed[i], HIGH);
    delay(100);
    mediaSensor(5);
    rgb[i] = mediaLeituras;
    float diffCinza = matrizBranco[i] - matrizPreto[i];
    rgb[i] = (rgb[i] - matrizPreto[i])/(diffCinza)*255;
    digitalWrite(rgbLed[i], LOW);
    delay(100);
  }
  //rgb[0] - red
  //rgb[1] - green
  //rgb[2] - blue
  if (rgb[0] > rgb[1] && rgb[0] > rgb[2]) //se o vermelhor for o maior de todos, a cor do m&m pode ser: vermelho, laranja, amarelo ou marrom
  {
    if (rgb[0] < 100) //se o vermelho for muito baixo, o m&m é marrom
    {
      //marrom
      corAtual = 4;
    }
    else if (rgb[0] - rgb[1] < 50) //se a diferenca entre o vermelho e o verde for pequena, é amarelo
    {
      //amarelo
      corAtual = 2;
    }
    else if (rgb[0] - rgb[1]*2 < 50) //se o verde for mais ou menos metade do vermelho, é laranja
    {
      //laranja
      corAtual = 3;
    }
    else //se o verde for muito pequeno, é vermelho
    {
      //vermelho
      corAtual = 6;
    }
  }
  else if (rgb[1] > rgb[0] && rgb[1] > rgb[2]) // se o verde for o maior, o m&m é verde
  {
    //verde
    corAtual = 5;
  }
  else //se o azul for o maior, o m&m é azul
  {
    //azul
    corAtual = 1;
  }
  motor.step(angulacoes[corAtual-1]); //gira o motor de passo com a angulacao desejada de acordo com a cor do m&m
  delay(1000);
  //faz a rampa cair
  //rampa sobe de novo
  motor.step(360-angulacoes[corAtual-1]); //motor de passo volta a posicao inicial
  delay(500);
  String aux = corAtual + "";
  bt.println(aux);
  bt.flush();
}
//void setupBlueToothConnection()
//{
//  bt.begin(9600);
//  bt.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
//  bt.print("\r\n+STNA=HC-05\r\n"); //set the bluetooth name as "SeeedBTSlave"
//  bt.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
//  bt.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
//  delay(2000); // This delay is required.
//  bt.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
//  Serial.println("The slave bluetooth is inquirable!");
//  delay(2000); // This delay is required.
//  bt.flush();
//}
