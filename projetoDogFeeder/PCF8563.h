#ifndef PCF8563_h
#define PCF8563_h
#include "Arduino.h"



class PCF8563{
  private:
    int segundo, minuto, hora, diaSemana, diaMes, mes, ano;
    String dias[7] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado" };
    byte bcdToDec(byte valor);
    byte decToBcd(byte valor);
  public:
    void setHora(int segundo, int minuto, int hora, int diaSemana, int diaMes, int mes, int ano);
    void atualizarHora(); //faz comunicação com o módulo
    int getSegundo();
    int getMinuto();
    int getHora();
    int getDiaSemana();
    int getDiaMes();
    int getMes();
    int getAno();
};

#endif
