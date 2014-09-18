#include "experiment.h"


void xor_evaluate(Genotype *organism)
{
    int in[4][2]={{0,0},
               {0,1},
               {1,0},
               {1,1}};
    int out[4];
    vector<float> out_vec;
    int i;

    for (i=0;i<4;i++)
    {
        organism->clear_node_value();
        organism->activate(in[i]);
        out_vec = organism->output_value();
        out[i] = out_vec.at(0);
    }
    float errorsum = (fabs(out[0])+fabs(1.0-out[1])+fabs(1.0-out[2])+fabs(out[3]));
    organism->set_fitness(pow((4.0-errorsum),2));

}
