#ifndef refeicao_h
#define refeicao_h
#include "Arduino.h"

class Refeicao{
	private:
		char hora[5];
		int porcao = 0;
		bool validaEntrada(char *entrada);
	public:
		//Refeicao();
		//~Refeicao();
		bool setRefeicao(char *entrada);
    String getHorario();
    int getPorcao();
};

#endif
