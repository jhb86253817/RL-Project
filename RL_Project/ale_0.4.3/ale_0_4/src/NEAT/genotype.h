#ifndef GENOTYPE_H
#define GENOTYPE_H

#include "gene_node.h"
#include "gene_connection.h"
#include "innovation.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>

using namespace std;

class Genotype
{
public:
    Genotype(int input_num, int output_num, int weight_range);
    Genotype();
    Genotype(const Genotype &origin);
    ~Genotype();
    void add_gene_node(Gene_node *new_node);
    void add_gene_connection(Gene_connection *new_connection);
    std::vector<Gene_node*> get_gene_nodes() const;
    std::vector<Gene_connection*> get_gene_connections() const;
    void print_genotype();
    void activate(int *inputs);
    int find_node(int node_id);
    float sigmoid(float value_before);
    int output_value();
    void clear_node_value();
    void set_fitness(float new_fitness);
    float get_fitness() const;
    int get_max_innovation_id() const;
    int get_max_node_id();
    bool node_in_connection(int node_id);
    float get_adjusted_fitness() const;
    float get_spawn_amount() const;
    float create_random(int range);
    void perturb(int which,float perturbed_value);
    int link_a_hidden_node();
    void set_adjusted_fitness(float adjusted);
    bool operator <(Genotype &g);
    void calculate_spawn_amount(float average);
    int get_connection_in(int which);
    int get_connection_out(int which);
    float get_connection_weight(int which);
    bool get_connection_enabled(int which);
    int get_connection_size();
    void set_weight(int which, float new_weight);
    void set_connection_enabled(int which,bool enable);
    int get_node_size();
    void write_genotype();
    void read_genotype();

private:
    vector<Gene_node*> *gene_nodes;
    vector<Gene_connection*> *gene_connections;
    float fitness;
    float adjusted_fitness;
    float spawn_amount;
    int max_innovation_id;
};

#endif // GENOTYPE_H
