#include <SoftwareSerial.h>
#include <Servo.h>
#include "refeicao.h"
#include "PCF8563.h"
#include "Wire.h"
#include <LiquidCrystal.h>

SoftwareSerial bluetooth(12, 13); //TX e RX (HC-05 se coincidem)

Servo servoMotor;
int pinServo = 11;

char entrada[9];//ex.: 1;15:30;7
bool novaEntrada; //validador para parar repetições
int i, comer;
bool salvo;
Refeicao ref1, ref2; //objetos que utilizam classe importada Refeicao 

PCF8563 relogio; //objeto que utiliza classe importada PCF8563
LiquidCrystal lcd(2,3,6,7,9,10); //lcd apenas pra escrita com 4bits por vez

int bip = 4;
int exibeRef; //contador para exibir horários salvos

void setup() {
  Serial.begin(9600); //inicia console em 9600
  bluetooth.begin(9600); //inicia serial emulada em 9600
  servoMotor.attach(pinServo);
  servoMotor.write(45); //porta fechada
  Wire.begin(); //inicialização necessária para uso do módulo PCF8563
  i = 0; //serve para controlar posição do array
  lcd.begin(16,2); //tamanho da tela (2 linhas)
  pinMode(bip, OUTPUT);
  exibeRef = 0; //contagem para exibir horários salvos a cada 1min
}

void loop() {
  while (bluetooth.available()) { //dados são recebidos byte a byte (cada caracter por vez)
    delay(3); //parada para garantir que todos os caracteres são recebidos
    entrada[i] = bluetooth.read();
    novaEntrada = true; //indicador para validar a análise da string recebida
    i++;
  }
  
  relogio.atualizarHora();//necessária constante leitura no módulo para atualizar objeto
  lcd.setCursor(0, 0); //posiciona cursor no começo da tela
  lcd.print(receberHorarioFormatado(relogio.getHora(), relogio.getMinuto()));
  lcd.print(":");
  lcd.print(relogio.getSegundo());
  i = 0; //saindo do loop a próxima entrada vai sobrescrever a anterior, posição do array volta ao início
  
  if(novaEntrada){
    novaEntrada = false;
    salvo = false; 
    if(entrada[0] == 49){//if = 1
      salvo = ref1.setRefeicao(entrada); //método retorna bool, assim valida se entrada foi aceita ou não
      if(salvo){
        lcd.setCursor(0,1);
        lcd.print("Ref1 "+ref1.getHorario()+"->"+ref1.getPorcao());
      }
    }
    else if(entrada[0] == 50){//if = 2
      salvo = ref2.setRefeicao(entrada); //método retorna bool, assim valida se entrada foi aceita ou não
      if(salvo){
        lcd.setCursor(0,1);
        lcd.print("Ref2 "+ref2.getHorario()+"->"+ref2.getPorcao());
      }
    }
    else if(comidaAgora(entrada)){ //if = comida;7 -> comando;porção
      lcd.setCursor(0,1);
      lcd.print("Liberar comida!");
      liberarComida(((int)entrada[7] - 48));
      tocarAlarme();
    }
    else if(!comidaAgora(entrada)){ //casos de entradas inválidas
      lcd.setCursor(0,1);
      lcd.print("Erro");
    }
  }
  comer = checarHorarioRefeicao(); //recebe número inteiro referente a refeição
  if(comer == 1){ //refeição salva 1
    lcd.setCursor(0,1);
    lcd.print("Hora da ref1");
    liberarComida(ref1.getPorcao());
    tocarAlarme();
  }
  else if(comer == 2){ //refeição salva 2
    lcd.setCursor(0,1);
    lcd.print("Hora da ref2");
    liberarComida(ref2.getPorcao());
    tocarAlarme();
  }
  if(comer == 1 || comer == 2){
     delay(60000); //1 minuto de espera, garante que caso esteja na hora de uma refeição, isso só ocorra 1 vez (liberar comida dura alguns segundos)
  }

  exibeRef++; //contador para mostrar horários salvos a cada 1min
  if(exibeRef == 60){
    lcd.setCursor(0,1);
    lcd.print("Ref1 "+ref1.getHorario()+"->"+ref1.getPorcao());
    delay(3000);
    lcd.setCursor(0,1);
    lcd.print("Ref2 "+ref2.getHorario()+"->"+ref2.getPorcao());
    delay(3000);
    exibeRef = 0;
  }

  delay(1000); //garantir que loop demora no mínimo 1 segundo
  lcd.clear();
}

bool comidaAgora(char *entrada){
  char validacao[7] = {'c','o','m','i','d','a',';'}; //Ex.: comida;7
  for(int i = 0; i<7; i++){
    if(validacao[i] != entrada[i]){
      return false;
    }
  }
  if((entrada[7] >= 48 && entrada[7] <= 57)){
    return true;
  }
  else{
    return false;
  }
}

void liberarComida(int porcao){
  int unidadePorcao = 250; //1 porção (30g)
  servoMotor.write(100); //porta aberta
  delay(unidadePorcao*porcao);
  servoMotor.write(45); //porta fechada
}
String receberHorarioFormatado(int hora, int minuto){
  String horario;
  
  if(hora < 10){
    horario = '0';
    horario += (char)hora%10;
  }
  else{
    horario = (char)hora/10;
    horario += (char)hora%10;
  }
  horario += ':';
  if(minuto < 10){
    horario += '0';
    horario += (char)minuto%10;
  }
  else{
    horario += (char)minuto/10;
    horario += (char)minuto%10;
  }
  return horario;
}
int checarHorarioRefeicao(){
  String horario = receberHorarioFormatado(relogio.getHora(), relogio.getMinuto()); //Ex.: 09:02 (só precisamos validar hora e minuto)
  if(horario.equals(ref1.getHorario())){
    return 1;
  }
  else if(horario.equals(ref2.getHorario())){
    return 2;
  }
  else{
    return 0;
  }
}
void tocarAlarme(){
  digitalWrite(bip, HIGH);
  delay(5000);
  digitalWrite(bip, LOW);
}
