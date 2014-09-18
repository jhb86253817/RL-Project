#ifndef GENERATION_H
#define GENERATION_H

#include "species.h"
#include <vector>
#include "experiment.h"

using namespace std;

class Generation
{
public:
    Generation(int population);
    ~Generation();
    int get_species_num();
    void print_generation();
    void evaluate();
    void evolve();
    void adjust_fitness();
    void sepciate(float threshold);
    void sort_by_fitness();
    void calculate_spawn_amount();
    void spawn();
    int get_best_fitness();
    void print_one_of_best();
    void get_all_genotypes(vector<Genotype*> &all);


private:
    vector<Species*> *all_species;
    int population;

};

#endif // GENERATION_H
