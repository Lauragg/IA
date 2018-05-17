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

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

class Nodo{
	static const int MaxProfundidad=5;
	int profundidad;
	vector<Nodo> hijos;
	Move movimiento;
	GameState estado;
	Player jugador,rival;
	friend class CompetitiBot;

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
		int resultadoActual=estado.getScore(jugador)-estado.getScore(rival);
		if(estado.isFinalState()){
			//Puntúa más un resultado real que un resultado calculado.
			return resultadoActual*100;
		}
		return 0;

	}


public:
	Nodo(){}
	Nodo(const Nodo & nodo) : profundidad(nodo.profundidad), hijos(nodo.hijos),
		movimiento(nodo.movimiento), estado(nodo.estado){}
	Nodo(int pronfundo, Move movi, GameState state, Player player): profundidad(pronfundo),
		movimiento(movi), estado(state), jugador(player){
			if(jugador==J1)
				rival=J2;
			else
				rival=J1;

		}

	void set(int profundo, Move movi, GameState state, Player player){
		profundidad=profundo;
		movimiento=movi;
		estado=state;
		if(jugador==J1)
			rival=J2;
		else
			rival=J1;
	}

	Move getMovimiento(){return movimiento;}

	int alphaBeta(int alpha, int beta){
		if(estado.isFinalState() || profundidad==MaxProfundidad){
			return heuristica();
		}
		if(hijos.empty()){
			crearHijos();
		}
		if(jugador==estado.getCurrentPlayer()){//Si es mi turno.
			int i=1;
			for(vector<Nodo>::iterator iter=hijos.begin(); iter!=hijos.end(); iter++){
				alpha=std::max(alpha,iter->alphaBeta(alpha,beta));
				if(beta<=alpha){
					hijos.resize(i); //La poda se convierte en cambiar el tamaño del vector.
					//¡Ojo! No tengo claro cómo se comporta el end del iterador en este caso.
					//Si falla, una posible solución es añadir como condición de salida i<=hijos.size()
					//¿Por qué lo hago así y no con vector? Estoy sacrificando el tamaño de
					//memoria que ocupa un entero por conseguir mayor eficiencia de acceso.
				}
				i++;
			}
			return alpha;

		}else{
			int i=1;
			for(vector<Nodo>::iterator iter=hijos.begin(); iter!=hijos.end(); iter++){
				beta=std::min(beta,iter->alphaBeta(alpha,beta));
				if(beta<=alpha){
					hijos.resize(i);
				}
				i++;
			}
			return beta;
		}
	}

	int movimientoHijo(){
		int elegido;
					cerr<<"4-1"<<endl;
		int puntos=-100000;
		if(hijos.empty())
					cerr<<"4-2"<<endl;
			crearHijos();
		for(int i=0;i<hijos.size();i++){
			int aux=hijos[i].alphaBeta(-100000,100000);
						//cerr<<aux << puntos<<endl;
			if(aux>puntos){
				elegido=i;
				puntos=aux;
			}
		}
					cerr<<"4-3 "<<elegido <<" " <<hijos[elegido].getMovimiento()<<endl;
					return elegido;
	}


};

class CompetitiBot:Bot {
	friend class Nodo;
private:
	Nodo actual;
	bool inicializado;
	void cambiarActual(vector<Move>::const_iterator & iter);

public:
	CompetitiBot();
	~CompetitiBot();

	void initialize();
	void initialize(const GameState & state);
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
};

#endif /* MANUPCBOT_H_ */
