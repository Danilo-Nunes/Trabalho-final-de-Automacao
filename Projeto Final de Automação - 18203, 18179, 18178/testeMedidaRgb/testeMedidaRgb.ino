#define ldr A0

//Matriz de floats para armazenar o valor das cores 
float rgb[] = {   0,0,0}; 
float matrizBranco[] = {   0,0,0}; 
float matrizPreto[] = {   0,0,0}; 

//Variável para armazenar a média das leitura 
int mediaLeituras;
boolean jaCalibrou = false;

int rgbLed[] = {12, 13, 7};
void setup() {
  for (int i = 0; i < 3; i++)
    pinMode(rgbLed[i], OUTPUT);
  pinMode(ldr, INPUT);
  Serial.begin(9600);
}

void loop() {
  /*if (!jaCalibrou)
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
  }*/
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(rgbLed[i], HIGH);
    delay(100);
    mediaSensor(5);
    rgb[i] = mediaLeituras;
    /*float diffCinza = matrizBranco[i] - matrizPreto[i];
    if (i == 2)
      rgb[i] = (rgb[i] - matrizPreto[i])/(diffCinza)*240;
    else
      rgb[i] = (rgb[i] - matrizPreto[i])/(diffCinza)*255;*/
    digitalWrite(rgbLed[i], LOW);
    delay(100);
  }
  String teste = "rgb(";
  teste.concat(rgb[0]);
  teste.concat(", ");
  teste.concat(rgb[1]);
  teste.concat(", ");
  teste.concat(rgb[2]);
  teste.concat(")");
  Serial.println(teste);
  //delay(200);
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

