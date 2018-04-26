#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>

void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	list<celda> abierto;
	list<celda> cerrado;
	bool finalizado=false;
	celda actual, dest=celda(NULL,destino.fila,destino.columna,destino.orientacion,destino.fila,destino.columna),
				orig=celda(NULL,origen.fila,origen.columna,origen.orientacion,destino.fila,destino.columna);

	//Paso 0
	abierto.push_front(orig);

	while(!finalizado){
		//Paso 1
		actual=abierto.front();
		cerrado.push_back(actual);
		abierto.pop_front();
		//Paso 2
		if(actual.accesible(mapaResultado)){
			list<celda> adyacentes=actual.getAdyacentes();
			for(list<celda>::iterator iter=adyacentes.begin(); iter!=adyacentes.end() && !finalizado ; ++iter){
				//Vemos si la celda escogida es el destino, en cuyo caso finalizamos.
				if((*iter)==dest){
					actual=*iter;
					finalizado=true;
						cout << "He llegado 2"<<endl;
				}
				else if(iter->accesible(mapaResultado) && !celdaCerrada(*iter,cerrado)){
						//Comprobamos si iter pertenece a la lista de abiertos.
						bool pertenece=false;
						list<celda>::iterator iterAb=abierto.begin();
						while(!pertenece && iterAb!=abierto.end()){
							if((*iter)==(*iterAb)){
								pertenece=true;
								//Si la g es mejor por el nuevo camino, entonces sustituimos.
								if(iter->menorOrigen(*iterAb)){
									*iterAb=*iter;
								}
							}
							++iterAb;
						}
						//Si no pertenece a la lista de abiertos, la añadimos.
						//Se recuerda que esto se ejecuta sólo si es accesible y no pertenece
						// a la lista de cerrados-
						if(!pertenece)
							abierto.push_back(*iter);
				}
			}
		}
		//Paso 3: Reordenamos la lista de abiertos.
		abierto.sort();
	//cout << "Cerrados->" << cerrado.size()<<endl;
	//cout << "Abiertos->" << abierto.size()<<endl;
	}
	cout << "Origen->" << orig.getEstado().fila << " " << orig.getEstado().columna<<endl;
	cout << "Destino->" << actual.getEstado().fila << " " << actual.getEstado().columna<<endl;		
	cout << "He llegado 3"<<endl;
	calcularPlan(orig,actual,plan);
		cout << "He llegado 4"<<endl;

	PintaPlan(plan);

	return true;
}

Action ComportamientoJugador::think(Sensores sensores) {
	Action accion=actIDLE;
	if(sensores.mensajeC!=-1){
		fil=sensores.mensajeF;
		col=sensores.mensajeC;
	}

	if(destino.fila!=sensores.destinoF || destino.columna!=sensores.destinoC){
		destino.fila=sensores.destinoF;
		destino.columna=sensores.destinoC;
		cout << "He llegado 1" <<endl;
	cout << "Origen->" << fil << " " << col<<endl;
	cout << "Destino->" << destino.fila << " " << destino.columna<<endl;
		pathFinding(estado(fil,col,brujula),destino,plan);
	}


	accion=plan.front();
	plan.pop_front();

  return accion;
}



bool ComportamientoJugador::celdaCerrada(const celda & celd, const list<celda> &cerrado){
	bool cerrada=false;
	list<celda>::const_iterator iter=cerrado.begin();
	while(!cerrada && iter!=cerrado.end()){
		if(celd==(*iter)) cerrada=true;
		++iter;
	}

	return cerrada;
}

void ComportamientoJugador::calcularPlan(const celda & origen, const celda & destino, list<Action> &plan){
	list<celda> ruta;
	celda actual=destino;
	int pos, giros;
		cout << "He llegado 2-1"<<endl;
	//Inicializamos ruta, insertándole todas las celdas en orden de lectura.
	bool nulo=false;
	bool doble=false;
	//int contador=0;
	while(!nulo&&!doble){
		ruta.push_front(actual);
		if(actual.getPadre()==NULL)
			nulo=true;		
		else actual=*(actual.getPadre());
		//if(contador%100==0)
		//	cout << contador<<endl;
		//contador++;
		if(celdaCerrada(actual,ruta))
			doble=true;
	}
		cout << "He llegado 2-2"<< nulo << doble <<endl;
	ruta.push_front(origen);


	//Comienzo la búsqueda.
	list<celda>:: iterator it=ruta.begin();
		cout << "He llegado 2-3"<<endl;
	while(it!=ruta.end()){
		//Vamos a recorrer las celdas adyacentes a la celda actual.
		actual=*it;
		list<celda>::iterator iter=actual.getAdyacentes().begin();
		//Con la celda siguiente.
		++it;
		//Son exactamente 4 adyacentes siempre.
		for(int i=0; i<4; i++){
			if((*iter)==(*it)) pos=i;
			++iter;
		}
		//Así sabemos cuántos giros tenemos que dar y en qué dirección.
		giros=actual.getOrientacion()-pos;
		if(giros==1 || giros == -3)
			plan.push_back(actTURN_L);
		else if(giros==0) ;
		else{
			plan.push_back(actTURN_R);
			if(giros==-2 || giros ==2)
				plan.push_back(actTURN_R);
		}
		plan.push_back(actFORWARD);
		it->setOrientacion(pos);
	}
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
