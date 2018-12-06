#include <SoftwareSerial.h>
#include <Stepper.h>
#include <Servo.h>

#define ldr A0
#define rx 2
#define tx 3
#define pServo 6
#define graus 50
#define ang 45

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
int corAtual = 0;
char estado = 0;
boolean jaCalibrou = false;
int rgbLed[] = {12, 13, 7};

void setup() {
  for (int i = 0; i < 3; i++)
    pinMode(rgbLed[i], OUTPUT);
  pinMode(ldr, INPUT);
  motor.setSpeed(3000);
  servo.attach(pServo);
  Serial.begin(9600);
  servo.write(ang);
  bt.begin(9600);
}

void loop() {
  if (bt.available())
  {
    estado = bt.read();
  }
  if (estado == '1')
  {
    Serial.println("fsdfds");
    lerValores();
  }
  if (estado == '2')
  {
    //já que está pausado, não acontece nada
  }
  if (estado == '3')
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
  if (!jaCalibrou)
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
  if (rgb[0] > rgb[1] && rgb[0] > rgb[2]) //se o vermelhor for o maior de todos, a cor do m&m pode ser: vermelho, laranja ou amarelo
  {
    corAtual = 1;
  }
  else if ((rgb[1] > rgb[0] && rgb[1] > rgb[2])|| (rgb[2] > rgb[0] && rgb[2] > rgb[1])) // se o verde for o maior, o m&m é verde
  {
    //cor fria
    corAtual = 2;
  }
  Serial.print(rgb[0]);
  Serial.print(" ");
  Serial.print(rgb[1]);
  Serial.print(" ");
  Serial.println(rgb[2]);
  motor.step(270 + (corAtual==1?110:300)); //gira o motor de passo com a angulacao desejada de acordo com a cor do m&m
  delay(1000);
  servo.write(25);
  delay(850);
  servo.write(ang);
  motor.step(1800 - (270 + (150 * corAtual))); //motor de passo volta a posicao inicial
  delay(1500);
  bt.println(corAtual);
  bt.flush();
}
