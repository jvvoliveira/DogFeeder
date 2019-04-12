#include "PCF8563.h"
#include "Arduino.h"
#include "Wire.h"
#define PCF8563address 0x51

byte PCF8563::bcdToDec(byte valor){
  return ((valor / 16) * 10 + valor % 16);
}
byte PCF8563::decToBcd(byte valor){
  return (valor / 10 * 16 + valor % 10);
}
void PCF8563::setHora(int segundo, int minuto, int hora, int diaSemana, int diaMes, int mes, int ano){
  //módulo clock durante transmissão para a contagem (precisa ser algo rápido)
  Wire.beginTransmission(PCF8563address);//inicia transmissão com o clock
  Wire.write(0x02); 
  Wire.write(decToBcd(segundo));
  Wire.write(decToBcd(minuto));
  Wire.write(decToBcd(hora));
  Wire.write(decToBcd(diaSemana));
  Wire.write(decToBcd(diaMes));
  Wire.write(decToBcd(mes));
  Wire.write(decToBcd(ano));
  Wire.endTransmission();//finaliza transmissão
}
void PCF8563::atualizarHora(){
  Wire.beginTransmission(PCF8563address);
  Wire.write(0x02);
  Wire.endTransmission();//devolve inteiro que identifica se houve erro
  Wire.requestFrom(PCF8563address, 7);
  
  this->segundo =   bcdToDec(Wire.read() & B01111111); // remove VL error bit
  this->minuto =    bcdToDec(Wire.read() & B01111111); // remove unwanted bits from MSB
  this->hora =      bcdToDec(Wire.read() & B00111111); 
  this->diaMes =    bcdToDec(Wire.read() & B00111111);
  this->diaSemana = bcdToDec(Wire.read() & B00000111);  
  this->mes =       bcdToDec(Wire.read() & B00011111);  // remove century bit, 1999 is over
  this->ano =       bcdToDec(Wire.read());
}
int PCF8563::getSegundo(){
  return this->segundo;
}
int PCF8563::getMinuto(){
  return this->minuto;
}
int PCF8563::getHora(){
  return this->hora;
}
int PCF8563::getDiaSemana(){
  return this->diaSemana;
}
int PCF8563::getDiaMes(){
  return this->diaMes;
}
int PCF8563::getMes(){
  return this->mes;
}
int PCF8563::getAno(){
  return this->ano;
}
