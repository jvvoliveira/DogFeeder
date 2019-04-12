#include "refeicao.h"
#include "Arduino.h"

//houve ajustes nas posi��es do array com a troca do uso da classe
bool Refeicao::validaEntrada(char *entrada){
  if(!(entrada[0] == 49 || entrada[0] == 50)){  //posi��o 0 s� pode ser 1 ou 2 (Refei��es)
    return false;
  }
  if(!((entrada[2] >= 48 && entrada[2] <= 50) && (entrada[3] >= 48 && entrada[3] <= 57))){  //posi��o 0 s� pode ser 0, 1 ou 2 e posi��o 1 pode ser de 0 at� 9 (Hora)
    return false;
  }
//  if(!(entrada[2] == 50) && (entrada[3] >= 48 && entrada[3] <= 51)){//caso a posi��o 2 seja 2, ent�o s� pode ser 20h, 21h, 22h ou 23h (Hora)
//    return false;
//  }
  if(!((entrada[5] >= 48 && entrada[5] <= 53) && (entrada[6] >= 48 && entrada[6] <= 57))){  //posi��o 5 s� pode ser de 0 at� 5 e posi��o 6 pode ser de 0 at� 9 (Minuto)
    return false;
  }
  if(!(entrada[8] >= 48 && entrada[8] <= 57)){  //posi��o 8 pode ser de 0 at� 9 (Por��es)
    return false;
  }
  return true;  
}

bool Refeicao::setRefeicao(char *entrada){
	//exemplo.: 1;15:00;7
	if(Refeicao::validaEntrada(entrada)){
		this->hora[0] = entrada[2];
      this->hora[1] = entrada[3];
      this->hora[2] = ':';
      this->hora[3] = entrada[5];
      this->hora[4] = entrada[6];
      this->porcao = ((int)entrada[8] - 48); //convers�o da tabela ASCII para inteiro
      return true;
	}
	else{
		return false;	
	}
}
String Refeicao::getHorario(){
  String horario;
  for(int i = 0; i<5; i++){
    horario += this->hora[i];
  }
  return horario;
}
int Refeicao::getPorcao(){
  return porcao;
}
