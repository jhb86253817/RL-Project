#include "innovation.h"

int Innovation::innovation_num = 0;
vector<Gene_connection*> Innovation::innovations;

Innovation::Innovation()
{
}

void Innovation::add_innovation(Gene_connection *new_innovation)
{
    if (new_innovation!=NULL)
    {
        int i;
        for (i=0;i<Innovation::innovations.size();i++)
        {
            if (Innovation::innovations.at(i)->get_in()==new_innovation->get_in() && Innovation::innovations.at(i)->get_out()==new_innovation->get_out())
                break;
        }
        if (i>=Innovation::innovations.size())
        {
            Innovation::innovations.push_back(new Gene_connection(*new_innovation));
            Innovation::innovation_num++;
        }
    }
    else
    {
        cout<<"new_innovation is NULL!"<<endl;
    }


}

int Innovation::get_innovation_num()
{
    return Innovation::innovation_num;
}

void Innovation::print_innovations()
{
    int i;
    for (i=0;i<Innovation::innovations.size();i++)
    {
        Innovation::innovations.at(i)->print_connection();
    }
}

int Innovation::check_if_innovation(int in,int out)
{
    int i;
    for (i=0;i<innovations.size();i++)
    {
        if (in==innovations.at(i)->get_in() && out==innovations.at(i)->get_out())
        {
            return innovations.at(i)->get_innovation_num();
        }
    }

    return 0;
}
