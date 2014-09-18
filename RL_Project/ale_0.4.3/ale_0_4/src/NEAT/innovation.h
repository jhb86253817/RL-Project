#ifndef INNOVATION_H
#define INNOVATION_H

#include <vector>
#include "gene_connection.h"

using namespace std;

class Innovation
{
public:
    Innovation();
    static void add_innovation(Gene_connection *new_innovation);
    static int get_innovation_num();
    static void print_innovations();
    static int check_if_innovation(int in,int out);

private:
    static int innovation_num;
    static vector<Gene_connection*> innovations;
};

#endif // INNOVATION_H
