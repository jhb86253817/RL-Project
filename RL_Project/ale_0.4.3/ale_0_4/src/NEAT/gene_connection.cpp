#include "gene_connection.h"


Gene_connection::Gene_connection(int in, int out, float weight, bool enabled, int innovation_num)
{
    this->in = in;
    this->out = out;
    this->weight = weight;
    this->enabled = enabled;
    this->innovation_num = innovation_num;
}

Gene_connection::Gene_connection(const Gene_connection &origin)
{
    this->in = origin.get_in();
    this->out = origin.get_out();
    this->weight = origin.get_weight();
    this->enabled = origin.isEnabled();
    this->innovation_num = origin.get_innovation_num();
}

int Gene_connection::get_in() const
{
    return this->in;
}

int Gene_connection::get_out() const
{
    return this->out;
}

float Gene_connection::get_weight() const
{
    return this->weight;
}

bool Gene_connection::isEnabled() const
{
    return this->enabled;
}

int Gene_connection::get_innovation_num() const
{
    return this->innovation_num;
}

void Gene_connection::print_connection()
{
    std::cout<<"in:"<<this->in<<" out:"<<this->out<<" weight:"<<this->weight<<" enabled:"<<this->enabled<<" innovation_num:"<<this->innovation_num<<endl;
}

void Gene_connection::set_enabled(bool new_enabled)
{
    this->enabled = new_enabled;
}

void Gene_connection::set_weight(float new_weight)
{
    this->weight = new_weight;
}
