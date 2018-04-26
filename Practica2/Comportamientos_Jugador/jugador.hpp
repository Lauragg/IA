#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <climits>
#include <cmath>
#include <iostream>

struct estado {
  int fila;
  int columna;
  int orientacion;

  estado(){}
  estado(int fil, int col, int orient) : fila(fil), columna(col), orientacion(orient){}
  estado(const estado & est) : fila(est.fila), columna(est.columna), orientacion(est.orientacion){}
};

/*
  clase celda, utilizada para nuestro algoritmo de búsqueda
*/
class celda{
 private:
  celda * padre;
  list <celda> adyacentes;
  int distanciaOrigen=0;
  int dest_fila,dest_columna;
  estado pos;


  void CalcAdy(){
    if(adyacentes.empty()){
      int pos_fil;
      int pos_col;
      pos_fil=pos.fila-1; pos_col=pos.columna;
      if(pos_fil >= 0 && pos_fil <= 100 && pos_col >=0 && pos_col <= 100)
        adyacentes.emplace_back(this,pos_fil,pos_col,0,dest_fila,dest_columna);

      pos_fil=pos.fila;pos_col=pos.columna+1;
      if(pos_fil >= 0 && pos_fil <= 100 && pos_col >=0 && pos_col <= 100)
        adyacentes.emplace_back(this,pos_fil,pos_col,1,dest_fila,dest_columna);

      pos_fil=pos.fila+1;pos_col=pos.columna;
      if(pos_fil >= 0 && pos_fil <= 100 && pos_col >=0 && pos_col <= 100)
        adyacentes.emplace_back(this,pos_fil,pos_col,2,dest_fila,dest_columna);

      pos_fil=pos.fila;pos_col=pos.columna-1;
      if(pos_fil >= 0 && pos_fil <= 100 && pos_col >=0 && pos_col <= 100)
        adyacentes.emplace_back(this,pos_fil,pos_col,3,dest_fila,dest_columna);
    }

  }

  int CalcDistHeur() const{
      return abs(pos.fila-dest_fila)+abs(pos.columna-dest_columna);
  }

  void CalcDistOrig(){
    if(padre!= NULL)
      distanciaOrigen=padre->distanciaOrigen+10;
  }

  celda(celda * papa){
    padre=papa;
  }

 public:
  celda(){}

/**
  celda(celda * papa, list <celda> adyc, int fils, int cols, int bruj, int dfils, int dcols) :
    padre(papa), adyacentes(adyc), pos(fils,cols,bruj){
      dest_fila=fils;
      dest_colummna=dcols;
    }
*/

  //celda(celda * papa, list <celda> adyc, estado est):
    //padre(papa), adyacentes(adyc), pos(est){}

  celda(const celda & copia) :
    celda(copia.padre,copia.pos.fila,copia.pos.columna,copia.pos.orientacion,copia.dest_fila,copia.dest_columna){
    distanciaOrigen=copia.distanciaOrigen;
  }
  celda(celda * papa, int fils, int cols, int bruj,int dfils, int dcols) :
    padre(papa), pos(fils,cols,bruj){
      dest_fila=fils;
      dest_columna=dcols;
      CalcDistOrig();
    }

//  celda(celda * papa, estado est) :
  //  celda(papa,est.fila,est.columna,est.orientacion){}

  list<celda> getAdyacentes() {
    if(adyacentes.empty())
      CalcAdy();
    return adyacentes;}

  celda * getPadre(){
    return padre;}

  int getOrientacion(){return pos.orientacion;}

  void setOrientacion(int orienta){pos.orientacion=orienta;}

  bool operator ==(const celda & b) const{
    return pos.fila==b.pos.fila && pos.columna==b.pos.columna;
  }

  celda & operator=(const celda & b){
    if(this!= &b){
      padre=b.padre;
      pos.fila=b.pos.fila;
      pos.columna=b.pos.columna;
      pos.orientacion=b.pos.orientacion;
      adyacentes=b.adyacentes;
      distanciaOrigen=b.distanciaOrigen;
      dest_fila=b.dest_fila;
      dest_columna=b.dest_columna;
    }
  }
/*
  celda & operator=(celda * b){
    if(this!= b){
      padre=b->padre;
      pos.fila=b->pos.fila;
      pos.columna=b->pos.columna;
      pos.orientacion=b->pos.orientacion;
      adyacentes=b->adyacentes;
      distanciaOrigen=b->distanciaOrigen;
      dest_fila=b->dest_fila;
      dest_columna=b->dest_columna;
    }
  }
*/
/*
  Este método nos devolverá true si tenemos una menor distancia al Origen o no en sentido estricto.
*/
  bool menorOrigen(const celda & b) const{
    return distanciaOrigen < b.distanciaOrigen;
  }

/*
  Este método nos dirá si nuestra celda es MEJOR o no estrictamente, en el sentido de que una celda es mejor
  cuando MENOR sea valorCelda.
*/
  bool operator <(const celda & b) const{
    return valorCelda() < b.valorCelda();
  }

/*
  El valor de una celda será la suma de distanciaOrigen y distanciaHeuristica.
*/
  int valorCelda()const {return distanciaOrigen+CalcDistHeur();}

  /*
  Nos indica si la celda es accesible o no.
  */
  bool accesible(vector<vector<unsigned char> > mapaResultado){
    char tipo=mapaResultado[pos.fila][pos.columna];
    if(tipo=='T' || tipo=='S' || tipo=='K' || tipo=='?')
      return true;
    else return false;
  }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, brujula;
    estado destino;
    list<Action> plan;
    //list<celda> cerrado;


    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    void PintaPlan(list<Action> plan);
    bool celdaCerrada(const celda & celd, const list<celda> &cerrado);
    void calcularPlan(const celda & origen, const celda & destino, list<Action> &plan);

};

#endif
