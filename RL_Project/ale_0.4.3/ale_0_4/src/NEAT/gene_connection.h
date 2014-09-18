#ifndef GENE_CONNECTION_H
#define GENE_CONNECTION_H

#include <iostream>

using namespace std;

class Gene_connection
{
public:
public:
    Gene_connection(int in, int out, float weight, bool enabled, int innovation_num);
    Gene_connection(const Gene_connection &origin);
    int get_in() const;
    int get_out() const;
    float get_weight() const;
    bool isEnabled() const;
    int get_innovation_num() const;
    void print_connection();
    void set_enabled(bool new_enabled);
    void set_weight(float new_weight);
private:
    int in;
    int out;
    float weight;
    bool enabled;
    int innovation_num;//unique number of every new added innovation, called historical marking
};

#endif // GENE_CONNECTION_H
