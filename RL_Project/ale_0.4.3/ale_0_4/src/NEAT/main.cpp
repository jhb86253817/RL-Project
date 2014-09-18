#include <QCoreApplication>
#include <iostream>
#include "gene_node.h"
#include "gene_connection.h"
#include "genotype.h"
#include "experiment.h"
#include "species.h"
#include "generation.h"

using namespace std;

void find_best_solution(int population,int generation);
void test();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    srand (time(NULL));

//    find_best_solution(100,100);
    test();





    return a.exec();
}

void find_best_solution(int population,int generation)//need better definition
{
    Generation g(population);
    int i;
    for (i=0;i<generation;i++)
    {
        g.evaluate();
        g.sort_by_fitness();
        g.sepciate(1.5);
        g.sort_by_fitness();
        g.adjust_fitness();
        g.calculate_spawn_amount();
        g.spawn();
        g.evaluate();
    }
    cout<<"best fitness:"<<g.get_best_fitness()<<endl;
    g.print_one_of_best();

}

void test()
{
    Genotype d;
    d.read_genotype();
    float inputs[8][2]={{0,0},
                     {0,1},
                     {1,0},
                     {1,1},
                     {1,1},
                     {1,0},
                     {0,1},
                     {0,0}};
    vector<float> output;
    for (int i=0;i<8;i++)
    {
        d.clear_node_value();
        d.activate(inputs[i]);
        output = d.output_value();
        cout<<output.at(0)<<endl;
    }

}
