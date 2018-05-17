/*
 * CompetitiBot.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 *  Finished on: 17 mayo 2018
 				Author: Laura Gómez Garrido
 */

#include "CompetitiBot.h"

#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

CompetitiBot::CompetitiBot() {
	// Inicializar las variables necesarias para ejecutar la partida
	inicializado=false;

}

CompetitiBot::~CompetitiBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void CompetitiBot::cambiarActual(vector<Move>::const_iterator & iterMov){
	bool cambio=false;
	vector<Nodo>::iterator iter=actual.hijos.begin();
	while (!cambio && iter!=actual.hijos.end()) {
		if(iter->movimiento==*iterMov){
			cambio=true;
			actual=*iter;
		}
		iter++;
	}
}
void CompetitiBot::initialize() {
	// Inicializar el bot antes de jugar una partida
}
void CompetitiBot::initialize(const GameState & state) {
	// Inicializar el bot antes de jugar una partida
	//setPlayer(state.getCurrentPlayer());
	cerr<<"2"<<endl;
	actual.set(0,M_NONE,state,getPlayer());
	cerr<<"3"<<endl;
}

string CompetitiBot::getName() {
	return "CompetitiBot"; // Sustituir por el nombre del bot
}

Move CompetitiBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	Move movimiento= M_NONE;
	cerr<<"0 " << movimiento<<endl;

	//Si es la primera vez que ejecutamos este método, lo inicializamos.
	if(!inicializado){
		cerr<<"1"<<endl;
		initialize(state);
		inicializado=true;
	//Si no es la primera vez, actualizamos de acuerdo a los movimientos del
	//adversario.
	}else if(!adversary.empty()){
			cerr<<"9"<<endl;
		for(vector<Move>::const_iterator iter=adversary.begin(); iter!=adversary.end(); iter++){
			cambiarActual(iter);
		}
	}
	cerr<<"4"<<endl;
	elegido=actual.movimientoHijo();
	cerr<<"5"<<endl;
	actual=*actual[elegido];
	cerr<<"6"<<endl;

	movimiento=actual.getMovimiento();
	// Implementar aquí la selección de la acción a realizar

	// OJO: Recordatorio. NO USAR cin NI cout.
	// Para salidas por consola (debug) utilizar cerr. Ejemplo:
	// cerr<< "Lo que quiero mostrar"<<endl;


	// OJO: Recordatorio. El nombre del bot y de la clase deben coincidir.
	// En caso contrario, el bot no podrá participar en la competición.
	// Se deberá sustituir el nombre AlumnoBot como nombre de la clase por otro
	// seleccionado por el alumno. Se deberá actualizar también el nombre
	// devuelto por el método getName() acordemente.
	cerr<<"10" <<movimiento<<endl;

	return movimiento;
}
