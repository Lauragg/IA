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

void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}


bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	list<celda> abierto;
	list<celda> cerrado;
	bool finalizado=false;
	celda actual, dest=celda(NULL,destino.fila,destino.columna,destino.orientacion,destino.fila,destino.columna),
				orig=celda(NULL,origen.fila,origen.columna,origen.orientacion,destino.fila,destino.columna);

	//Paso 0
	plan.clear();
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
				}
				else if(iter->accesible(mapaResultado) && !celdaCerrada(*iter,cerrado)){
						//Comprobamos si iter pertenece a la lista de abiertos.
						bool pertenece=false;
						list<celda>::iterator iterAb=abierto.begin();
						while(!pertenece && iterAb!=abierto.end()){
							if((*iter)==(*iterAb)){
								pertenece=true;
								//Si la g es mejor por el nuevo camino, entonces sustituimos.
								if(iter->menorOrigen(*iterAb) && !(*iter->getPadre()==*iterAb->getPadre())){
									iterAb=abierto.erase(iterAb);
									iterAb=abierto.insert(iterAb,*iter);
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
	}
	calcularPlan(orig,actual,plan);
	PintaPlan(plan);
	VisualizaPlan(origen,plan);

	return true;
}

Action ComportamientoJugador::think(Sensores sensores) {
	//Si me mandan una nueva posición.
	if(sensores.mensajeC!=-1){
		fil=sensores.mensajeF;
		col=sensores.mensajeC;
		if(sensores.reset){
			plan.clear();
			hayPlan=false;
			accion=actIDLE;
			brujula=0;
		}
	}
	if(accion==actTURN_R) brujula=(brujula+1)%4;
	else if(accion==actTURN_L) brujula=(brujula+3)%4;

	//Comportamiento Reactivo.
	if(fil== 99 || col == 99){
			bool hayK=false;
			for(int i=1;i<=15;i++)
				if(sensores.terreno[i]=='K')
					hayK=true;


			if(plan.empty() || hayK)
				reactivo(sensores,plan);

			accion=plan.front();

			if(accion==actFORWARD && sensores.terreno[2]!='T' && sensores.terreno[2]!='K' && sensores.terreno[2]!='S'){
				if(contador==2){
					giro=!giro;
				}else if(contador==3){
					contador=0;
				}
				reactivo(sensores,plan);
				accion=plan.front();
				plan.pop_front();
				++contador;
			}else if(accion==actFORWARD && sensores.superficie[2]=='a')
				accion=actIDLE;
			else plan.pop_front();

	}else{//Comportamiento Deliberativo.
		//Si no me han dado una nueva posicion, actualizo la actual de acuerdo al último movimiento.
		 if(!sensores.colision && hayPlan){
			 if(accion==actFORWARD && sensores.terreno[0]!='K'){ //Porque si estamos en una casilla K nuestra posicion ya ha sido actualizada.
				 switch (brujula) {
					 case 0: fil--; break;
					 case 1: col++; break;
					 case 2: fil++; break;
					 case 3: col--; break;
				}
			}
		}

		actualizarMapa(sensores);

		//Si cambian mi destino.
		if(destino.fila!=sensores.destinoF || destino.columna!=sensores.destinoC || !hayPlan){
			destino.fila=sensores.destinoF;
			destino.columna=sensores.destinoC;
			cout << "Origen " << fil << " " << col << " " << brujula <<endl;
			cout << "Destino " << destino.fila<< " " << destino.columna<<endl;
			if(plan.empty())cout << "Vacío"<<endl;
			hayPlan=pathFinding(estado(fil,col,brujula),destino,plan);
		}

		accion=actIDLE; //Esto perdura sólo en caso de que el plan este vacío. El plan estará vacío sólo si hemos llegao a la casilla objetivo.
		if(!plan.empty()){
			accion=plan.front();
			if(accion==actFORWARD && sensores.terreno[2]!='T' && sensores.terreno[2]!='K' && sensores.terreno[2]!='S'){
				//Si el plan es inválido, recalculamos uno válido y hacemos el primer movimiento que este nos indique.
				hayPlan=pathFinding(estado(fil,col,brujula),destino,plan);
				accion=plan.front();
				plan.pop_front();
			}else if(accion==actFORWARD && sensores.superficie[2]=='a')
			//Si tenemos delante un aldeano y queremos avanzar, esperamos.
				accion=actIDLE;
			else
			//Si el plan es correcto y podemos avanzar, proseguimos con el plan.
				plan.pop_front();
		}
	}


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

void ComportamientoJugador::actualizarMapa(Sensores sensores){
	mapaResultado[fil][col]=sensores.terreno[0];
	if(brujula==0){
		int k=0;
		for(int i=1; i<=3; i++){
			int n=1+2*i;
			for(int j=1; j<=n; j++){
				k++;
				mapaResultado[fil-i][col+j-n+i]=sensores.terreno[k];
			}
		}
	}else if(brujula==1){
		int k=0;
		for(int i=1; i<=3; i++){
			int n=1+2*i;
			for(int j=1; j<=n; j++){
				k++;
				mapaResultado[fil+j-n+i][col+i]=sensores.terreno[k];
			}
		}
	}else if(brujula==2){
		int k=0;
		for(int i=1; i<=3; i++){
			int n=1+2*i;
			for(int j=n; j>=1; j--){
				k++;
				mapaResultado[fil+i][col+j-n+i]=sensores.terreno[k];
			}
		}
	}else{
		int k=0;
		for(int i=1; i<=3; i++){
			int n=1+2*i;
			for(int j=n; j>=1; j--){
				k++;
				mapaResultado[fil+j-n+i][col-i]=sensores.terreno[k];
			}
		}
	}
}

void ComportamientoJugador::calcularPlan(const celda & origen, const celda & destino, list<Action> &plan){
	list<celda> ruta;
	celda actual=destino;
	int pos=99, giros;
	//Inicializamos ruta, insertándole todas las celdas en orden de lectura.
	bool nulo=false;
	bool doble=false;
	int contador=0;
	while(!nulo&&!doble){
		celda antigua=actual;
		ruta.push_front(actual);
		if(actual.getPadre()==NULL)
			nulo=true;
		else actual=*(antigua.getPadre());
		//cout << contador<<endl;
		contador++;
		if(actual==antigua)
			cout << "Iguales"<<endl;
		if(celdaCerrada(actual,ruta))
			doble=true;
	}


	//Comienzo la búsqueda.
	list<celda>:: iterator it=ruta.begin();
		//cout << "He llegado 2-3"<<endl;
	while(it!=ruta.end()){
		//Vamos a recorrer las celdas adyacentes a la celda actual.
		actual=*it;
		list<celda> adyacentes=actual.getAdyacentes();
		list<celda>::iterator iter=adyacentes.begin();
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
		if(!((*it)==destino))
			plan.push_back(actFORWARD);
		it->setOrientacion(pos);
	}
}

bool ComportamientoJugador::reactivo(Sensores sensores, list<Action> & plan){
	int k=0; //Inicializamos a 0 porque si fuese 0 estaríamos sobre una casilla K y no entrearíamos aquí.
	int pos=1;
	bool bloqueado[3]={true,true,true};
	plan.clear();
	for(int i=1;i<=3;i++){
		int n=1+2*i;
		for(int j=1;j<=n;j++){
			if(sensores.terreno[pos]=='K'){
				bloqueado[i-1]=false;
				k=pos;
			}else if(sensores.terreno[pos]=='T' || sensores.terreno[pos]=='S'){
				bloqueado[i-1]=false;
			}
			pos++;
		}
	}
	//Caso particular importante:
	if(sensores.terreno[2]!='K' && sensores.terreno[2]!='T' && sensores.terreno[2]!='S')
		bloqueado[0]=true;

	if(sensores.terreno[6]!='T' && sensores.terreno[6]!='S' &&
			sensores.terreno[7]!='T' && sensores.terreno[7]!='S'&&
			sensores.terreno[5]!='T' && sensores.terreno[5]!='S')
			bloqueado[1]=true;

	if(k==0){
		if(bloqueado[0]){
			girar(plan);
		}else if(sensores.terreno[6]!='T' && sensores.terreno[6]!='S'){
			bool bloqueo1=sensores.terreno[1]!='T' && sensores.terreno[1]!='S';
			bool bloqueo3=sensores.terreno[3]!='T' && sensores.terreno[3]!='S';
			if(bloqueo1 && bloqueo3){ //Todo bloqueado, así que doy la media vuelta.
					girar(plan);
				//cout << "Media vuelta"<<endl;
			}else if(bloqueo1){//Si 1 está bloqueado, giro a la derecha.
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				//cout << "Giro verdadero "<<endl;
			}else if(bloqueo3){//Si 3 está bloqueado, giro a la izquierda.
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				//cout << "Giro falso"<<endl;
			}else
				plan.push_back(actFORWARD);

		}else if(bloqueado[1] || bloqueado[2]){
			girar(plan);
		}else{
			plan.push_back(actFORWARD);
		}//A partir de aquí miramos en caso de que no haya casilla k;
	}else if(bloqueado[0] || (bloqueado[1] && k>3)){
		plan.push_back(actTURN_R);
	}else{
		miniBusqueda(k,sensores,plan);
	}

	//En caso de que el plan esté vacío, metemos unas actuaciones prefinidas.
	if(plan.empty()){
		if(!bloqueado[0])
			plan.push_back(actFORWARD);
		else
			plan.push_back(actIDLE);
	}

	//PintaPlan(plan);
	return true;

}

void ComportamientoJugador::miniBusqueda(int k,Sensores sensores, list<Action> &ruta){
	ruta.push_back(actFORWARD);
	if(k==1 || k==3){
			if(k==1) ruta.push_back(actTURN_L);
			else ruta.push_back(actTURN_R);
			ruta.push_back(actFORWARD);
	}else if(k==4){
		if(sensores.terreno[6]=='T' || sensores.terreno[6]=='S'){
			if(sensores.terreno[5]=='T' || sensores.terreno[5]=='S'){
				ruta.push_back(actFORWARD);ruta.push_back(actTURN_L);
				ruta.push_back(actFORWARD);ruta.push_back(actFORWARD);
			}else if((sensores.terreno[12]=='T' || sensores.terreno[12]=='S') &&
				(sensores.terreno[11]=='T' || sensores.terreno[11]=='S') &&
				(sensores.terreno[10]=='T' || sensores.terreno[10]=='S')){
				ruta.push_back(actFORWARD);ruta.push_back(actFORWARD);
				ruta.push_back(actTURN_L);ruta.push_back(actFORWARD);
				ruta.push_back(actFORWARD);ruta.push_back(actTURN_L);
				ruta.push_back(actFORWARD);
			}
		}
	}else if(k==8){
			if(sensores.terreno[6]=='T' || sensores.terreno[6]=='S'){
				if(sensores.terreno[7]=='T' || sensores.terreno[7]=='S'){
					ruta.push_back(actFORWARD);ruta.push_back(actTURN_R);
					ruta.push_back(actFORWARD);ruta.push_back(actFORWARD);
				}else if((sensores.terreno[12]=='T' || sensores.terreno[12]=='S') &&
					(sensores.terreno[13]=='T' || sensores.terreno[13]=='S') &&
					(sensores.terreno[14]=='T' || sensores.terreno[14]=='S')){
					ruta.push_back(actFORWARD);ruta.push_back(actFORWARD);
					ruta.push_back(actTURN_R);ruta.push_back(actFORWARD);
					ruta.push_back(actFORWARD);ruta.push_back(actTURN_R);
					ruta.push_back(actFORWARD);
				}
		}
		//A continuar, si sobra tiempo  FIXME::FALTA K==9,15 principalmente
	}

}

	void ComportamientoJugador::girar(list<Action> & plan){
		if(giro){
			plan.push_back(actTURN_R);
			for(int i=0; i<6; i++)
				plan.push_back(actFORWARD);
			plan.push_back(actTURN_R);
		}else{
			plan.push_back(actTURN_L);
			for(int i=0; i<6; i++)
				plan.push_back(actFORWARD);
			plan.push_back(actTURN_L);
		}
		giro=!giro;
	}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
