#ifndef SPECIES_H
#define SPECIES_H

#include "genotype.h"
#include <vector>
#include "experiment.h"
#include <algorithm>

using namespace std;

class Species
{
public:
    Species();
    Species(Species &origin);
    ~Species();
    float compatible_value(Genotype* chosen_organism);
    Genotype* crossover();
    Genotype* mutate();
    void add_organism(Genotype* new_organism);
    int get_species_size();
    void print_species();
    void evaluate();
    void set_representative(vector<Genotype*> &store);
    void adjust_fitness();
    void sort_by_fitness();
    void set_representative(Genotype *new_genotype);
    float get_adjusted_fitness();
    void calculate_spawn_amount(float average);
    int get_spawn_amount();
    void set_spawn_amount(int amount);
    void spawn();
    vector<Genotype*>* get_organisms();
    Genotype* get_representative();
    int get_best_fitness();
    void print_best();
    void get_all_genotypes(vector<Genotype*> &all);


private:
    vector<Genotype*> *organisms;
    Genotype* representative;
    int spawn_amount;
};

#endif // SPECIES_H
