#include "species.h"

Species::Species()
{
    this->representative = NULL;
    this->spawn_amount = 0;
    this->organisms = new vector<Genotype*>();
}

Species::Species(Species &origin)
{
    int i;
    for (i=0;i<origin.get_organisms()->size();i++)
    {
        this->organisms->push_back(new Genotype(*origin.get_organisms()->at(i)));
    }
    if (origin.get_representative()==NULL)
        this->representative = NULL;
    else
        this->representative = new Genotype(*origin.get_representative());
    this->spawn_amount = origin.get_spawn_amount();
}

float Species::compatible_value(Genotype* chosen_organism)
{
    int bigger_gene;
    if (this->representative->get_gene_connections().size()>chosen_organism->get_gene_connections().size())
    {
        bigger_gene = this->representative->get_gene_connections().size();
    }
    else
    {
        bigger_gene = chosen_organism->get_gene_connections().size();
    }
    int p1 = 0, p2 = 0;
    int id1, id2;
    float weight1, weight2;
    float weight_sum = 0.0;
    int weight_count = 0;
    int disjoint_count = 0;
    int excess_count = 0;
    while(true)
    {
        id1 = this->representative->get_gene_connections().at(p1)->get_innovation_num();
        id2 = chosen_organism->get_gene_connections().at(p2)->get_innovation_num();
        if (id1==id2)
        {
            weight1 = this->representative->get_gene_connections().at(p1)->get_weight();
            weight2 = chosen_organism->get_gene_connections().at(p2)->get_weight();
            weight_sum += fabs(weight1-weight2);
            weight_count++;
            p1++;
            p2++;
        }
        else if (id1>id2)
        {
            disjoint_count++;
            p2++;
        }
        else
        {
            disjoint_count++;
            p1++;
        }
        if (p1>=this->representative->get_gene_connections().size())
        {
            while(p2<chosen_organism->get_gene_connections().size())
            {
                excess_count++;
                p2++;
            }
            break;
        }
        if (p2>=chosen_organism->get_gene_connections().size())
        {
            while(p1<this->representative->get_gene_connections().size())
            {
                excess_count++;
                p1++;
            }
            break;
        }
    }
//    cout<<"weight_sum:"<<weight_sum<<endl;
//    cout<<"weight_count:"<<weight_count<<endl;
//    cout<<"disjoint_count:"<<disjoint_count<<endl;
//    cout<<"excess_count"<<excess_count<<endl;

    //need better definition
    float distance = 1.0*excess_count/bigger_gene+1.0*disjoint_count/bigger_gene+0.4*weight_sum/weight_count;
    return distance;
}

Genotype* Species::crossover()
{
    Genotype *kid = new Genotype();
    Gene_connection *temp_connection;

    int p1 = 0, p2 = 0;
    int id1, id2;
    float weight1, weight2;
    int inherit_which;
    int rand_1 = 0, rand_2 = 0;

    Genotype *gene1, *gene2;
    //pick random parents
    for (int i=0;i<5;i++)
    {
        rand_1 = rand() % this->organisms->size();
        rand_2 = rand() % this->organisms->size();
        if (rand_1!=rand_2)
            break;
    }
    gene1 = this->organisms->at(rand_1);
    gene2 = this->organisms->at(rand_2);

    while(true)
    {
        id1 = gene1->get_gene_connections().at(p1)->get_innovation_num();
        id2 = gene2->get_gene_connections().at(p2)->get_innovation_num();
        if (id1==id2)
        {
            inherit_which = rand() % 2 + 1;
            if (inherit_which==1)
                temp_connection = new Gene_connection(*(gene1->get_gene_connections().at(p1)));
            else
                temp_connection = new Gene_connection(*(gene2->get_gene_connections().at(p2)));

            kid->add_gene_connection(temp_connection);
            p1++;
            p2++;
        }
        else if (id1>id2)
        {
            if (gene2->get_fitness()>=gene1->get_fitness())
            {
                temp_connection = new Gene_connection(*(gene2->get_gene_connections().at(p2)));
                kid->add_gene_connection(temp_connection);
            }
            p2++;
        }
        else
        {
            if (gene1->get_fitness()>=gene2->get_fitness())
            {
                temp_connection = new Gene_connection(*(gene1->get_gene_connections().at(p1)));
                kid->add_gene_connection(temp_connection);
            }
            p1++;
        }
        if (p1>=gene1->get_gene_connections().size())
        {
            while(p2<gene2->get_gene_connections().size())
            {
                if (gene2->get_fitness()>=gene1->get_fitness())
                {
                    temp_connection = new Gene_connection(*(gene2->get_gene_connections().at(p2)));
                    kid->add_gene_connection(temp_connection);
                }
                p2++;
            }
            break;
        }
        if (p2>=gene2->get_gene_connections().size())
        {
            while(p1<gene1->get_gene_connections().size())
            {
                if (gene1->get_fitness()>=gene2->get_fitness())
                {
                    temp_connection = new Gene_connection(*(gene1->get_gene_connections().at(p1)));
                    kid->add_gene_connection(temp_connection);
                }
                p1++;
            }
            break;
        }
    }

    int max_node_id;
    if (gene1->get_max_node_id()>=gene2->get_max_node_id())
        max_node_id = gene1->get_max_node_id();
    else
        max_node_id = gene2->get_max_node_id();

    Gene_node *temp_node;
    int temp_type;
    for (int i=1;i<=max_node_id;i++)
    {
        if (kid->node_in_connection(i))
        {
            if (gene1->find_node(i)>=0)
            {
                temp_type = gene1->get_gene_nodes().at(gene1->find_node(i))->get_type();
                temp_node = new Gene_node(i,temp_type);
                kid->add_gene_node(temp_node);
            }
            else if(gene2->find_node(i)>=0)
            {
                temp_type = gene2->get_gene_nodes().at(gene2->find_node(i))->get_type();
                temp_node = new Gene_node(i,temp_type);
                kid->add_gene_node(temp_node);
            }

        }
    }
    return kid;
}

Genotype* Species::mutate()
{
    Genotype* origin = this->organisms->at(rand() % this->organisms->size());
    int mutation_rate_weight = 80;
    int mutation_rate_weight_perturbed = 90;
    int mutation_rate_add_link = 15;
    int mutation_rate_add_node = 10;
    int i;
    int temp_random;
    float perturbed_value;
    Genotype* variation = new Genotype(*origin);
    Gene_node *temp_node;
    Gene_connection *temp_connection;
    for (i=0;i<variation->get_gene_connections().size();i++)
    {
        temp_random = rand() % 100 + 1;
        //mutate
        if (temp_random<=mutation_rate_weight)
        {
            temp_random = rand() % 100 + 1;
            //uniformly perturbe the weight
            if (temp_random<=mutation_rate_weight_perturbed)
            {
                //range of perturb
                perturbed_value = variation->create_random(0);
                variation->perturb(i,perturbed_value);
            }
            //set a new random value
            else
            {
                perturbed_value = variation->create_random(2);
                variation->set_weight(i,perturbed_value);
            }
        }
    }

    int chosen_in, chosen_out, chosen_weight;
    int connection_random;
    temp_random = rand() % 100 + 1;
    //add new node
    if (temp_random<=mutation_rate_add_node)
    {
        //create a new node
        temp_node = new Gene_node(variation->get_node_size()+1/*Gene_node::next_node_id()*/,2);
        variation->add_gene_node(temp_node);
        //choose which connection to put the new node
        connection_random = rand() % variation->get_connection_size();
        int j;
        for (j=0;j<10;j++)
        {
            if (!variation->get_connection_enabled(connection_random))
                connection_random = rand() % variation->get_connection_size();
            else
                break;
        }
        if (j<10)
        {
            chosen_in = variation->get_connection_in(connection_random);
            chosen_out = variation->get_connection_out(connection_random);
            chosen_weight = variation->get_connection_weight(connection_random);

            //disable the original connection
  //          variation->set_connection_enabled(connection_random,0);
            int temp_innovation_num = Innovation::check_if_innovation(chosen_in,temp_node->get_node_num());
            if (temp_innovation_num==0)
                temp_innovation_num = Innovation::get_innovation_num()+1;

            //add one connection
            temp_connection = new Gene_connection(chosen_in,temp_node->get_node_num(),1,1,temp_innovation_num);
            variation->add_gene_connection(temp_connection);
            Innovation::add_innovation(temp_connection);

            temp_innovation_num = Innovation::check_if_innovation(temp_node->get_node_num(),chosen_out);
            if (temp_innovation_num==0)
                temp_innovation_num = Innovation::get_innovation_num()+1;

            //add the other connection
            temp_connection = new Gene_connection(temp_node->get_node_num(),chosen_out,chosen_weight,1,temp_innovation_num);
            variation->add_gene_connection(temp_connection);
            Innovation::add_innovation(temp_connection);
        }
    }

    temp_random = rand() % 100 + 1;
    //add new link
    if (temp_random<=mutation_rate_add_link)
    {
        variation->link_a_hidden_node();
    }

    return variation;
}

void Species::add_organism(Genotype* new_organism)
{
    this->organisms->push_back(new_organism);
}

int Species::get_species_size()
{
    return this->organisms->size();
}

void Species::print_species()
{
    int i;   
    cout<<"spawn amount:"<<this->spawn_amount<<endl;
    for (i=0;i<this->organisms->size();i++)
    {
        this->organisms->at(i)->print_genotype();
    }
}

void Species::evaluate()
{
    int i;
    for (i=0;i<this->organisms->size();i++)
    {
        xor_evaluate(this->organisms->at(i));
    }
}

void Species::set_representative(vector<Genotype*> &store)
{
    this->representative = new Genotype(*this->organisms->at(0));
    int i;
    for (i=this->organisms->size()-1;i>=1;i--)
    {
        store.push_back(this->organisms->at(i));
        this->organisms->pop_back();
    }
}

void Species::adjust_fitness()
{
    int i;
    float adjusted;
    for (i=0;i<this->organisms->size();i++)
    {
        adjusted = this->organisms->at(i)->get_fitness() / this->organisms->size();
 //       cout<<"adjusted "<<i+1<<":"<<adjusted<<endl;
        this->organisms->at(i)->set_adjusted_fitness(adjusted);
    }
}

void Species::sort_by_fitness()
{
    int i,j;
    Genotype *temp = NULL;
    for (i=0;i<this->organisms->size();i++)
    {
        for (j=0;j<this->organisms->size()-i-1;j++)
        {
            if (this->organisms->at(j)->get_fitness() < this->organisms->at(j+1)->get_fitness())
            {
                temp = this->organisms->at(j);
                this->organisms->at(j) = this->organisms->at(j+1);
                this->organisms->at(j+1) = temp;
            }
        }
    }
}

void Species::set_representative(Genotype *new_genotype)
{
    this->representative = new Genotype(*new_genotype);
}

float Species::get_adjusted_fitness()
{
    float sum = 0;
    int i;
    for (i=0;i<this->organisms->size();i++)
    {
        sum += this->organisms->at(i)->get_adjusted_fitness();
    }
    return sum;
}

void Species::calculate_spawn_amount(float average)
{
    int i;
    float total_amout = 0;
    for (i=0;i<this->organisms->size();i++)
    {
        this->organisms->at(i)->calculate_spawn_amount(average);
        total_amout += this->organisms->at(i)->get_spawn_amount();
    }
    this->spawn_amount = (int)total_amout;
}

int Species::get_spawn_amount()
{
    return this->spawn_amount;
}

void Species::set_spawn_amount(int amount)
{
    this->spawn_amount = amount;
}

void Species::spawn()
{

    int cross_num, mutate_num, eliminate_num, remain_num;
    eliminate_num = (int)(this->organisms->size() * 0.2);
    cross_num = (int)(this->spawn_amount * 0.4);
    mutate_num = (int)(this->spawn_amount * 0.4);
    remain_num = this->spawn_amount - cross_num - mutate_num;
    int i;
    for (i=this->organisms->size()-1;i>organisms->size()-1-eliminate_num;i--)
    {
     //   delete this->organisms.at(i);
        this->organisms->erase(this->organisms->begin()+i);
    }
    if (remain_num>this->organisms->size())
        remain_num = this->organisms->size();
    vector<Genotype*> *offspring = new vector<Genotype*>();

    this->sort_by_fitness();

    for (i=0;i<remain_num;i++)
    {
        offspring->push_back(new Genotype(*this->organisms->at(i)));
    }

    for (i=0;i<cross_num;i++)
    {
        offspring->push_back(this->crossover());
    }

    for (i=0;i<mutate_num;i++)
    {
        offspring->push_back(this->mutate());
    }
//    cout<<"offspring size:"<<offspring->size()<<endl;

//    cout<<"offspring:"<<endl;
//    for (i=0;i<offspring->size();i++)
//    {
//        offspring->at(i)->print_genotype();
//    }
//    cout<<"organisms:"<<endl;
//    for (i=0;i<this->organisms->size();i++)
//    {
//        this->organisms->at(i)->print_genotype();
//    }
    this->organisms->swap(*offspring);
//    cout<<"spawn:"<<this->spawn_amount<<" remain_num:"<<remain_num<<" cross_num:"<<cross_num;
//    cout<<" mutate_num:"<<mutate_num<<" eliminate_num:"<<eliminate_num<<""<<endl;

//    for (i=0;i<offspring->size();i++)
//    {
//        delete offspring->at(i);
//    }
//    offspring->clear();


}

Species::~Species()
{

    delete this->representative;
    this->representative = NULL;
    int i;
    for (i=0;i<this->organisms->size();i++)
    {
        delete this->organisms->at(i);
    }
    this->organisms->clear();
    delete this->organisms;
    this->organisms = NULL;

}

vector<Genotype*>* Species::get_organisms()
{
    return this->organisms;
}

Genotype* Species::get_representative()
{
    return this->representative;
}

int Species::get_best_fitness()
{
    this->sort_by_fitness();
    if (this->organisms->size()==0)
        return 0;
    else
        return this->organisms->at(0)->get_fitness();
}

void Species::print_best()
{
    if (this->organisms->size()>0)
    {
//        this->organisms->at(0)->print_genotype();
        this->organisms->at(0)->write_genotype();
    }
    else
        cout<<"no organisms!"<<endl;
}

void Species::get_all_genotypes(vector<Genotype*> &all)
{
    int i;
    for (i=0;i<this->organisms->size();i++)
    {
        all.push_back(this->organisms->at(i));
    }
}
