#include "genetic.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

static double sqr (double a) {return a*a;}

static bool Ber() {
    return rand() % 2;
}

static void print_bin(u8 a) {
    for (int i=7; i>=0; i--)
        std::cout << ((a >> i) & 1);
    std::cout << "\n";
}

EightAlleleGenomeCreature::EightAlleleGenomeCreature() {
    this->genome = rand() % 256;
    this->fitness_func = fitness_func_default;
}

EightAlleleGenomeCreature::EightAlleleGenomeCreature(u8 a, double (*fitness_func_)(u8)): genome(a), fitness_func(fitness_func_) {

}

double EightAlleleGenomeCreature::getFitness() {
    return (this->fitness_func)(this->genome);
}

u8 EightAlleleGenomeCreature::getGenome() {
    return this->genome;
}

EightAlleleGenomeCreaturePopulation::EightAlleleGenomeCreaturePopulation(int size_): size(size_) {
    srand(time(NULL));
    this->population = new EightAlleleGenomeCreature[size];
    this->fitnesses_cache = new double[size];
}

EightAlleleGenomeCreaturePopulation::~EightAlleleGenomeCreaturePopulation() {
    delete[] this->population;
    delete[] this->fitnesses_cache;
}

EightAlleleGenomeCreature EightAlleleGenomeCreaturePopulation::breed(int p1, int p2) {
    int d1 = rand() % 9;
    int d2 = rand() % 9;
    EightAlleleGenomeCreature child;
    child.genome = 0;
    if (d1 > d2) { d1 = d1 + d2; d2 = d1 - d2; d1 = d1 - d2; }
    u8 m1 = (u8)0xFF >> (8-d1);
    u8 m3 = (u8)0xFF << d2;
    u8 m2 = (m1^m3)^0xFF;
     
    if (m1) child.genome = (Ber()) ? (population[p1].genome & m1)  : (population[p2].genome & m1);
    if (m2) child.genome ^= (Ber()) ? (population[p1].genome & m2) : (population[p2].genome & m2);
    if (m3) child.genome ^= (Ber()) ? (population[p1].genome & m3) : (population[p2].genome & m3);
    
    //child.genome = (Ber()) ? (population[p1].genome & m1) ^ (population[p2].genome & m2) ^ (population[p1].genome & m3) :
    //    (population[p2].genome & m1) ^ (population[p1].genome & m2) ^ (population[p2].genome & m3);

    /* mutation probability - 0.02 */
    bool mutation = ((rand() % 50) == 0);
    if (mutation) {
        int i = rand() % 8;
        child.genome ^= (1 << i);
    }
    return child;
}

EightAlleleGenomeCreature* EightAlleleGenomeCreaturePopulation::getCurrentPopulation() {
    return population;
}

double* EightAlleleGenomeCreaturePopulation::getNormalFitnesses() {
    double sum=0;
    for (int i=0; i<this->size; i++) {
        sum += population[i].getFitness();
    }
    for (int i=0; i<this->size; i++) {
        fitnesses_cache[i] = (population[i].getFitness())/sum;
    }
    return fitnesses_cache;
}

int EightAlleleGenomeCreaturePopulation::getIndex(double n) {
    double prev = 0, next = 0;
    for (int i=0; i<size; i++) {
        prev = next;
        next += fitnesses_cache[i];
        if ((prev < n) && (n <= next)) return i;
    }
}

void EightAlleleGenomeCreaturePopulation::processEpoch() {
    EightAlleleGenomeCreature* new_population = new EightAlleleGenomeCreature[this->size];
    this->getNormalFitnesses();
    for (int i=0; i<this->size; i++) {
        int i1=0, i2=0;
        while (i1 == i2) {
            i1 = this->getIndex(((double) rand() / (RAND_MAX)));
            i2 = this->getIndex(((double) rand() / (RAND_MAX)));
        }
        //std::cout << i1 << " "<< i2 << "\n";
        new_population[i] = this->breed(i1, i2);
    }
    delete[] this->population;
    this->population = new_population;
}

void EightAlleleGenomeCreaturePopulation::processEpoch2() {
    EightAlleleGenomeCreature* new_population = new EightAlleleGenomeCreature[this->size];
    this->getNormalFitnesses();
    for (int i=0; i<size; i++) {
        new_population[i] = this->population[this->getIndex(((double) rand() / (RAND_MAX)))];
    }
    for (int i=0; i<this->size; i+=2) {
        int d1 = rand() % 9;
        int d2 = rand() % 9;
        if (d1 > d2) { d1 = d1 + d2; d2 = d1 - d2; d1 = d1 - d2; }
        u8 m1 = (u8)0xFF >> (8-d1);
        u8 m3 = (u8)0xFF << d2;
        u8 m2 = (m1^m3)^0xFF;
    
        new_population[i] = (new_population[i].genome & m1) ^ (new_population[i+1].genome & m2) ^ (new_population[i].genome & m3);
        new_population[i+1] = (new_population[i+1].genome & m1) ^ (new_population[i].genome & m2) ^ (new_population[i+1].genome & m3);
        /* mutation probability - 0.01 */
        bool mutation = ((rand() % 100) == 0);
        if (mutation) {
            int i = rand() % 8;
            new_population[i].genome ^= (1 << i);
        }
        mutation = ((rand() % 100) == 0);
        if (mutation) {
            int i = rand() % 8;
            new_population[i+1].genome ^= (1 << i);
        }
    }
    delete[] this->population;
    this->population = new_population;
}

void EightAlleleGenomeCreaturePopulation::run(int epochs) {
    for (int i=0; i<epochs; i++) {
        this->processEpoch2();
        for (int j=0; j<size; j++)
            std::cout << this->population[j].getFitness() << ":" << (int)(this->population[j].genome) << " ";
        std::cout << "\n";
    }
}

EightAlleleGenomeCreature EightAlleleGenomeCreaturePopulation::getMaxFitnessCreature() {
    double max = -(double)(RAND_MAX), fitness;
    int index;
    for (int i=0; i<this->size; i++) {
        fitness = this->population[i].getFitness();
        if (fitness > max) { 
            max = fitness; 
            index = i;
        }
    }
    return this->population[index];
}