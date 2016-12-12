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

double (*fitness_func_default)(u8) = &f2;

int main() {
    EightAlleleGenomeCreaturePopulation life(20);
    life.run(1000);
    std::cout << "Creature with the best fitness has genome: " << (int)(life.getMaxFitnessCreature().getGenome());
    return 0;
}