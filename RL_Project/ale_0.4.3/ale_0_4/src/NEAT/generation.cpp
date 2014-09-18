#include "generation.h"

Generation::Generation(int population)
{
    this->population = population;
    this->all_species = new vector<Species*>();
    int i;
    Genotype *new_organism;
    Species *new_species = new Species();
    for (i=0;i<population;i++)
    {
        new_organism = new Genotype(100,18,2);//need better definition
        new_species->add_organism(new_organism);
    }
    this->all_species->push_back(new_species);
}

int Generation::get_species_num()
{
    return this->all_species->size();
}

void Generation::print_generation()
{
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        cout<<"species "<<i+1<<endl;
        this->all_species->at(i)->print_species();
    }
}

void Generation::evaluate()
{
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        this->all_species->at(i)->evaluate();
    }
}

void Generation::evolve()
{
    this->evaluate();


}

void Generation::adjust_fitness()
{
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        this->all_species->at(i)->adjust_fitness();
    }
}

void Generation::sepciate(float threshold)
{
    vector<Genotype*> *store = new vector<Genotype*>();
    Species *new_species;
    int i,j;
    float temp;
    for (i=0;i<this->all_species->size();i++)
    {
        this->all_species->at(i)->set_representative(*store);
    }
    //put genotypes into different species
    for (i=0;i<store->size();i++)
    {
        for (j=0;j<this->all_species->size();j++)
        {
            //satisify the threshold
            temp = this->all_species->at(j)->compatible_value(store->at(i));
//            cout<<"temp:"<<temp<<"  i:"<<i<<" j:"<<j<<endl;
//            cout<<"species size:"<<this->all_species.size()<<endl;
            if (temp<threshold)
            {
                this->all_species->at(j)->add_organism(store->at(i));
                break;
            }
        }
        //no existing species satisify, build a new one
        if (j==this->all_species->size())
        {
            new_species = new Species();
            new_species->add_organism(store->at(i));
            new_species->set_representative(store->at(i));
            this->all_species->push_back(new_species);
        }
    }
}

void Generation::sort_by_fitness()
{
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        this->all_species->at(i)->sort_by_fitness();
    }
}

void Generation::calculate_spawn_amount()
{
    float sum = 0, average;
    int size = 0;
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        sum += this->all_species->at(i)->get_adjusted_fitness();
        size += this->all_species->at(i)->get_species_size();
    }
    average = sum / size;
    for (i=0;i<this->all_species->size();i++)
    {
        this->all_species->at(i)->calculate_spawn_amount(average);
    }
    int check = 0;
    for (i=0;i<this->all_species->size()-1;i++)
    {
        check += this->all_species->at(i)->get_spawn_amount();
    }

    this->all_species->at(this->all_species->size()-1)->set_spawn_amount(this->population-check);

}

void Generation::spawn()
{
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        this->all_species->at(i)->spawn();
    }
}

Generation::~Generation()
{
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        delete this->all_species->at(i);
    }
    this->all_species->clear();
    delete this->all_species;
    this->all_species = NULL;
}

int Generation::get_best_fitness()
{
    int best = -1;
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        if (this->all_species->at(i)->get_best_fitness()>best)
            best = this->all_species->at(i)->get_best_fitness();
    }
    return best;
}

void Generation::print_one_of_best()
{
    int best_fitness = this->get_best_fitness();
//    cout<<"There are "<<this->all_species->size()<<" species"<<endl;
//    cout<<"one of best:"<<endl;
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        if (this->all_species->at(i)->get_best_fitness()==best_fitness)
        {
            this->all_species->at(i)->print_best();
        }
    }
}

void Generation::get_all_genotypes(vector<Genotype*> &all)
{
    int i;
    for (i=0;i<this->all_species->size();i++)
    {
        this->all_species->at(i)->get_all_genotypes(all);
    }
}

