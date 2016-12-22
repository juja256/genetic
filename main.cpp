#include "genetic.h"
#include <iostream>
#include <math.h>

double f(u8 a) {
    double x = (double)a;
    return (-(x*x - 6*x + 19)/256 + 250);
}

double f2(u8 a) {
    double x = (double)a;
    return (1/sqrt((2*M_PI*100))) * exp(-((x - 240)*(x - 240))/(2*100) );
}

double f3(u8 a) {
    double x= (double)a;
    return sin(M_PI*x/256);
}

double (*fitness_func_default)(u8) = &f;

int main() {
    EightAlleleGenomeCreaturePopulation life(70);
    EightAlleleGenomeCreature* creatures = life.getCurrentPopulation();
    std::cout << "Initial population:\n";
    for (int i=0; i<70; i++) {
        std::cout << (int)creatures[i].getGenome() << ":" << creatures[i].getFitness() << " ";
    }
    std::cout << "\n";
    life.run(100);
    std::cout << "Creature with the best fitness has genome: " << (int)(life.getMaxFitnessCreature().getGenome());
    return 0;
}
