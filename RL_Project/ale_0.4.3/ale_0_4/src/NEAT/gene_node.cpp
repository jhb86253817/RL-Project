#include "gene_node.h"

int Gene_node::node_id = 0;

Gene_node::Gene_node(int node_num, int type)
{
    this->node_num = node_num;
    this->type = type;
    this->value = 0.0;
}

Gene_node::Gene_node(int node_num, int type, float value)
{
    this->node_num = node_num;
    this->type = type;
    this->value = value;
}

Gene_node::Gene_node(Gene_node &origin)
{
    this->node_num = origin.get_node_num();
    this->type = origin.get_type();
    this->value = origin.get_value();
}

int Gene_node::get_node_num()
{
    return this->node_num;
}

int Gene_node::get_type()
{
    return this->type;
}


void Gene_node::print_node()
{
    std::cout<<"node_num:"<<this->node_num<<" type:"<<this->type<<" value:"<<this->value<<endl;
}

int Gene_node::next_node_id()
{
    node_id++;
    return node_id;
}

float Gene_node::get_value()
{
    return this->value;
}

void Gene_node::set_value(float new_value)
{
    this->value = new_value;
}

void Gene_node::add_value(float new_value)
{
    this->value += new_value;
}

int Gene_node::get_node_id()
{
    return node_id;
}
