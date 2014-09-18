#ifndef GENE_NODE_H
#define GENE_NODE_H
#include <iostream>

using namespace std;

class Gene_node
{
public:
    Gene_node(int node_num, int type);
    Gene_node(int node_num, int type, float value);
    Gene_node(Gene_node &origin);
    int get_node_num();
    int get_type();
    void print_node();
    float get_value();
    void set_value(float new_value);
    void add_value(float new_value);
    static int next_node_id();
    static int get_node_id();

private:
    int node_num;
    int type;//1:input neuron;2:hidden neuron;3:output neuron
    float value;
    static int node_id;
};

#endif // GENE_NODE_H
