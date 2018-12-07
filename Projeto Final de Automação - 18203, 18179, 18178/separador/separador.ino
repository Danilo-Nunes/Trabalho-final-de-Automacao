#include <SoftwareSerial.h>
#include <Stepper.h>
#include <Servo.h>
//constantes para as entradas
#define ldr A0
#define rx 2
#define tx 3
#define pinoServo 6
#define graus 50
#define ang 55

Servo servo;
Stepper motor(360/graus, 8, 10, 9, 11);
SoftwareSerial bt(rx, tx);

//Matriz de floats para armazenar o valor das cores e a angulacao correspondente
float rgb[] = {0,0,0};
float matrizBranco[] = {364,346,174};
float matrizPreto[] = {150,155,96};
int mov = 0;
int retorno = 0;

//Variável para armazenar a média das leitura e da cor do m&m atual
int mediaLeituras;
int corAtual = 0;
String estado = "";
boolean jaCalibrou = false;
int rgbLed[] = {12, 13, 7};
boolean enviar = false;

void setup() {
  for (int i = 0; i < 3; i++)
    pinMode(rgbLed[i], OUTPUT);
  pinMode(ldr, INPUT);
  motor.setSpeed(500);
  servo.attach(pinoServo);
  Serial.begin(9600);
  servo.write(ang);
  bt.begin(9600);
}

void loop() {
  if (bt.available())
  {
    estado = bt.readString();
  }
  if (estado == "1")
  {
    lerValores();
  }
  else
  {
    //já que está pausado ou parado, não acontece nada
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
    delay(10000);   
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
    Serial.print("Branco: ");
    Serial.print(matrizBranco[0]);
    Serial.print(" ");
    Serial.print(matrizBranco[1]);
    Serial.print(" ");
    Serial.println(matrizBranco[2]);
    Serial.print("preto: ");
    Serial.print(matrizPreto[0]);
    Serial.print(" ");
    Serial.print(matrizPreto[1]);
    Serial.print(" ");
    Serial.println(matrizPreto[2]);
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
    //cor quente
    corAtual = 1;
  }
  else // se o verde ou azul for o maior, o m&m tem uma cor fria
  {
    //cor fria
    corAtual = 2;
  }
  Serial.print(rgb[0]);
  Serial.print(" ");
  Serial.print(rgb[1]);
  Serial.print(" ");
  Serial.println(rgb[2]);
  if (enviar)
  {
    bt.println(corAtual);
    bt.flush();
  }
  mov = 300 + (corAtual==1?130:300);
  motor.step(mov); //gira o motor de passo com a angulacao desejada de acordo com a cor do m&m
  //delay(1000);
  servo.write(15);
  delay(850);
  servo.write(ang);
  retorno = 1790 - mov;
  motor.step(retorno); //motor de passo volta a posicao inicial
  //delay(1500);
  mov = 0;
  retorno = 0;
  enviar = true;
}
