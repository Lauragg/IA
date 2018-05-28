/*
 * AlumnoBot.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

class Nodo{
	//static const int MaxProfundidad=8; //Profundidad en función del tiempo
	int profundidad;
	vector<Nodo> hijos;
	Move movimiento;
	GameState estado;
	Player jugador;

	void crearHijos(){
		GameState hijo;
		for(int i=1; i<=6;i++){
			//Si no nos inmolamos con este movimiento.
			if(estado.getSeedsAt(estado.getCurrentPlayer(),(Position) i)>0){
				//Creamos el estado correspondiente a este nodo hijo.
				hijo=estado.simulateMove((Move) i);

				//Si con este movimiento cambiamos el turno, aumenta la profundidad.
				if(hijo.getCurrentPlayer()!=estado.getCurrentPlayer())
					hijos.emplace_back(profundidad+1,(Move) i, hijo, jugador);

				//Si no cambiamos el turno, seguimos en la misma profundidad.
				else
					hijos.emplace_back(profundidad,(Move) i, hijo, jugador);
			}
		}
	}

	int heuristica(){
		//Positivo si yo tengo más en el granero, negativo si tiene más mi rival.
		//0 en caso de tener la misma cantidad de semillas en el granero.
		Player rival;
		if(jugador==J1)
			rival=J2;
		else
			rival=J1;
		int resultadoActual=estado.getScore(jugador)-estado.getScore(rival);
		//cerr << jugador<< " " << estado.getScore(jugador) <<" "<<rival <<" "<<estado.getScore(rival)<< " diferencia "<<resultadoActual<<endl;
		if(estado.isFinalState()){
			//Puntúa más un resultado real que un resultado calculado.
			return resultadoActual*100;

		}
		return resultadoActual;

	}


public:
		static int numeroNodos;
	Nodo(){}
	Nodo(const Nodo & nodo) : profundidad(nodo.profundidad), hijos(nodo.hijos),
		movimiento(nodo.movimiento), estado(nodo.estado), jugador(nodo.jugador){}
	Nodo(int pronfundo, Move movi, GameState state, Player player): profundidad(pronfundo),
		movimiento(movi), estado(state),jugador(player){
						//cerr << profundidad<<" "<<jugador<< " "<< player<<endl;
		}
/*
	void set(int profundo, Move movi, GameState state, Player player){
		profundidad=profundo;
		movimiento=movi;
		estado=state;
		if(jugador==J1)
			rival=J2;
		else
			rival=J1;
	}*/

	Move getMovimiento(){return movimiento;}

	int alphaBeta(int alpha, int beta, clock_t clock_ini){
		numeroNodos++;
		double tiempo=6*(double(clock()-clock_ini)/CLOCKS_PER_SEC);
		if(tiempo>1.8|| estado.isFinalState()){
			//cerr << tiempo<<endl;
			return heuristica();
		}
		if(hijos.empty()){
			crearHijos();
		}
		if(jugador==estado.getCurrentPlayer()){//Si es mi turno.
			int i=1;
			int aux=-100000;
			for(vector<Nodo>::iterator iter=hijos.begin(); iter!=hijos.end(); iter++){
				aux=std::max(aux,iter->alphaBeta(alpha,beta,clock_ini));
				alpha=std::max(alpha,aux);
				if(beta<=alpha){
					hijos.resize(i);
				}
				i++;
			}//cerr << "Max:alpha " <<alpha<<" beta " <<beta<<endl;
			return aux;

		}else{
			int i=1;
			int aux=10000;
			for(vector<Nodo>::iterator iter=hijos.begin(); iter!=hijos.end(); iter++){
				aux=std::min(aux,iter->alphaBeta(alpha,beta,clock_ini));
				beta=std::min(beta,aux);
				if(beta<=alpha){
					hijos.resize(i);
				}
				i++;
			}//cerr << "Min: alpha " <<alpha<<" beta " <<beta<<endl;
			return aux;
		}
	}

	Move movimientoHijo(){
		int elegido;
		int puntos=-100000;
		if(hijos.empty())
			crearHijos();

		clock_t clock_ini=clock();
		for(int i=0;i<hijos.size();i++){
			int aux=hijos[i].alphaBeta(-100000,100000,clock());
			//cerr << i << " "<<aux<<endl;
			if(aux>puntos){
				elegido=i;
				puntos=aux;
			}
		}
					cerr<<numeroNodos <<" "<<(double(clock()-clock_ini)/CLOCKS_PER_SEC) <<endl;
					return hijos[elegido].getMovimiento();
	}


};


class CompetitiBot:Bot {
	friend class Nodo;

public:
	CompetitiBot();
	~CompetitiBot();

	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
};

#endif /* MANUPCBOT_H_ */
