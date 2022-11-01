/*
 * Runnable.cpp
 * Aggancia automaticamente in coda le classi "Runnable" durante la creazione
 */


#include "Runnable.h"


Runnable::Runnable() {  // Costruttore richiamato automaticamente alla creazione degli oggetti derivati
  //Serial.println("Runnable++");  // Nn può funzionare xkè richiamato prima dell'avvio del programma che iniz la Serial
  nextRunnable = headRunnable;
  headRunnable = this;
}


void Runnable::setupAll() {
  for (Runnable *r = headRunnable; r; r = r->nextRunnable)
    r->setup();
}


void Runnable::loopAll() {
  for (Runnable *r = headRunnable; r; r = r->nextRunnable)
    r->loop();
}


void Runnable::printAll() {
  Serial.print("Runnable.printAll:");
  for (Runnable *r = headRunnable; r; r = r->nextRunnable)
      Serial.print("x");
  Serial.println("");
}


//Allocca var Statiche:
Runnable *Runnable::headRunnable = NULL;  // Testa della coda di esecuzione dei "Runnable"

