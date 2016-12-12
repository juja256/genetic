#ifndef GENETIC_H
#define GENETIC_H

typedef unsigned char u8;

extern double (*fitness_func_default)(u8);

class EightAlleleGenomeCreaturePopulation;

class Creature {
public:
    virtual ~Creature() {}
    virtual double getFitness()=0;
};

class EightAlleleGenomeCreature : public Creature {
    friend class EightAlleleGenomeCreaturePopulation;
    double (*fitness_func)(u8);
    u8 genome;
public:
    EightAlleleGenomeCreature();
    EightAlleleGenomeCreature(u8 a, double (*fitness_func)(u8)=fitness_func_default);
    double getFitness();
    u8 getGenome();
};

class EightAlleleGenomeCreaturePopulation {
    int size;
    EightAlleleGenomeCreature* population;
    double* fitnesses_cache;
public:
    EightAlleleGenomeCreaturePopulation(int size);
    ~EightAlleleGenomeCreaturePopulation();

    EightAlleleGenomeCreature breed(int p1, int p2);
    EightAlleleGenomeCreature* getCurrentPopulation();
    void processEpoch2();
    void processEpoch();
    void run(int epochs);
    int getIndex(double n);
    double* getNormalFitnesses();
    EightAlleleGenomeCreature getMaxFitnessCreature();
};

#endif