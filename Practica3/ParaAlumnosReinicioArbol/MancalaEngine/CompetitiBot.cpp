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

}

CompetitiBot::~CompetitiBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void CompetitiBot::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string CompetitiBot::getName() {
	return "CompetitiBot"; // Sustituir por el nombre del bot
}

Move CompetitiBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	Move movimiento= M_NONE;
	Nodo actual(0,M_NONE,state,getPlayer());
	movimiento=actual.movimientoHijo();
	// Implementar aquí la selección de la acción a realizar

	// OJO: Recordatorio. NO USAR cin NI cout.
	// Para salidas por consola (debug) utilizar cerr. Ejemplo:
	// cerr<< "Lo que quiero mostrar"<<endl;


	// OJO: Recordatorio. El nombre del bot y de la clase deben coincidir.
	// En caso contrario, el bot no podrá participar en la competición.
	// Se deberá sustituir el nombre AlumnoBot como nombre de la clase por otro
	// seleccionado por el alumno. Se deberá actualizar también el nombre
	// devuelto por el método getName() acordemente.


	return movimiento;
}
