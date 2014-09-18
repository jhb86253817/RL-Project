#include "genotype.h"

Genotype::Genotype(int input_num, int output_num, int weight_range)
{
    this->gene_nodes = new vector<Gene_node*>();
    this->gene_connections = new vector<Gene_connection*>();
    int i,j;
    float random_weight;
    int temp_node_id = 1;
    int temp_connection_id = 1;

    for (i=0;i<input_num;i++)
    {
        this->add_gene_node(new Gene_node(temp_node_id++,1));
    }
    for (i=0;i<output_num;i++)
    {
        this->add_gene_node(new Gene_node(temp_node_id++,3));
    }
    //add bias node
    this->add_gene_node(new Gene_node(temp_node_id++,0));

//    if (Gene_node::get_node_id()==0)
//    {
//        for (i=0;i<input_num+output_num+1;i++)
//        {
//            Gene_node::next_node_id();
//        }
//    }
    Gene_connection *temp_connection = NULL;
    for (i=0;i<input_num;i++)
    {
        for(j=0;j<output_num;j++)
        {   
            //for input nodes
            random_weight = this->create_random(weight_range);
            temp_connection = new Gene_connection(i+1,input_num+j+1,random_weight,true,temp_connection_id++);
            this->add_gene_connection(temp_connection);
            Innovation::add_innovation(temp_connection);          
        }
    }

    for(j=0;j<output_num;j++)
    {
        //for bias node
        random_weight = this->create_random(weight_range);
        temp_connection = new Gene_connection(input_num+output_num+1,input_num+j+1,random_weight,true,temp_connection_id++);
        this->add_gene_connection(temp_connection);
        Innovation::add_innovation(temp_connection);
    }

    this->fitness = 0.0;
    this->adjusted_fitness = 0.0;
    this->spawn_amount = 0.0;
    this->max_innovation_id = 0;
}

Genotype::Genotype()
{
    this->gene_nodes = new vector<Gene_node*>();
    this->gene_connections = new vector<Gene_connection*>();
    this->fitness = 0.0;
    this->adjusted_fitness = 0.0;
    this->spawn_amount = 0.0;
    this->max_innovation_id = 0;
}

Genotype::Genotype(const Genotype &origin)
{
    this->gene_nodes = new vector<Gene_node*>();
    this->gene_connections = new vector<Gene_connection*>();
    int i;
    for (i=0;i<origin.get_gene_nodes().size();i++)
    {
        this->gene_nodes->push_back(new Gene_node(*origin.get_gene_nodes().at(i)));
    }
    for (i=0;i<origin.get_gene_connections().size();i++)
    {
        this->gene_connections->push_back(new Gene_connection(*origin.get_gene_connections().at(i)));
    }
    this->gene_nodes = new vector<Gene_node*>(origin.get_gene_nodes());
    this->gene_connections = new vector<Gene_connection*>(origin.get_gene_connections());
    this->fitness = origin.get_fitness();
    this->adjusted_fitness = origin.get_adjusted_fitness();
    this->spawn_amount = origin.get_spawn_amount();
    this->max_innovation_id = origin.get_max_innovation_id();
}

void Genotype::add_gene_node(Gene_node *new_node)
{
    this->gene_nodes->push_back(new Gene_node(*new_node));
}

void Genotype::add_gene_connection(Gene_connection *new_connection)
{
    this->gene_connections->push_back(new Gene_connection(*new_connection));
}

vector<Gene_node*> Genotype::get_gene_nodes() const
{
    return *this->gene_nodes;
}

vector<Gene_connection*> Genotype::get_gene_connections() const
{
    return *this->gene_connections;
}

void Genotype::print_genotype()
{
    int i;
    for (i=0;i<this->gene_nodes->size();i++)
    {
        this->gene_nodes->at(i)->print_node();
    }
    for (i=0;i<this->gene_connections->size();i++)
    {
        this->gene_connections->at(i)->print_connection();
    }
    cout<<"fitness:"<<this->fitness<<endl;
    cout<<"adjusted fitness:"<<this->adjusted_fitness<<endl<<endl;
}

int Genotype::find_node(int node_id)
{
    int i;
    for (i=0;i<this->gene_nodes->size();i++)
    {
        if (this->gene_nodes->at(i)->get_node_num()==node_id)
            return i;
    }
  //  cout<<"cannot find node!"<<endl;
    return -1;
}

void Genotype::activate(int *inputs)
{
    int i,j;
    //set value for input nodes
    for (i=0,j=0;i<this->gene_nodes->size();i++)
    {
        if (this->gene_nodes->at(i)->get_type()==1)
        {
            this->gene_nodes->at(i)->set_value(inputs[j]);
            j++;
        }
        //set value for bias node
        else if (this->gene_nodes->at(i)->get_type()==0)
        {
            this->gene_nodes->at(i)->set_value(1);
        }
    }

    //get a copy of connection
    vector<Gene_connection*> *temp = new vector<Gene_connection*>(*this->gene_connections);
    float value_temp = 0.0;
    float weight_temp = 0.0;

    //delete disabled connections
//    for (i=0;i<temp->size();i++)
//    {
//        if (!temp->at(i)->isEnabled())
//        {
//            temp->erase(temp->begin()+i);
//        }
//    }

    int check_cyclic = 1;
    //pass the value from input to output in an appropriate order
    while(!temp->empty())
    {
        if (check_cyclic==0)
        {
            break;
        }
        check_cyclic = 0;
        for (i=0;i<temp->size();i++)
        {
            for(j=0;j<temp->size();j++)
            {
                if (temp->at(j)->get_out()==temp->at(i)->get_in())
                    break;
            }
            if (j==temp->size())
            {
                check_cyclic = 1;
                value_temp = this->gene_nodes->at(this->find_node(temp->at(i)->get_in()))->get_value();
                //sigmoid the output value of hidden nodes
                if (this->gene_nodes->at(this->find_node(temp->at(i)->get_in()))->get_type()!=1
                        &&this->gene_nodes->at(this->find_node(temp->at(i)->get_in()))->get_type()!=0)
                {
                    value_temp = this->sigmoid(value_temp);
                }
                weight_temp = temp->at(i)->get_weight();
                this->gene_nodes->at(this->find_node(temp->at(i)->get_out()))->add_value(value_temp*weight_temp);
                temp->erase(temp->begin()+i);
                break;
            }
        }
    }
}

float Genotype::sigmoid(float value_before)
{
    float value_after = 1 / (1+exp(-4.9*value_before));
    return value_after;
}

int Genotype::output_value()
{
    int i;
    float temp;
    float best_score = 0;
    int best_one, count_which=0;
    for (i=0;i<this->gene_nodes->size();i++)
    {
        if (this->gene_nodes->at(i)->get_type()==3)
        {
            temp = this->gene_nodes->at(i)->get_value();
            temp = this->sigmoid(temp);
            if (temp>best_score)
            {
                best_score = temp;
                best_one = count_which;
            }
            count_which++;
        }
    }
    return best_one;
}

void Genotype::clear_node_value()
{
    int i;
    for (i=0;i<this->gene_nodes->size();i++)
    {
        this->gene_nodes->at(i)->set_value(0.0);
    }
}

void Genotype::set_fitness(float new_fitness)
{
    this->fitness = new_fitness;
}

float Genotype::get_fitness() const
{
    return this->fitness;
}

int Genotype::get_max_innovation_id() const
{
    if (!this->gene_connections->empty())
        return this->gene_connections->at(gene_connections->size()-1)->get_innovation_num();

    return 0;
}

int Genotype::get_max_node_id()
{
    if (!this->gene_nodes->empty())
        return this->gene_nodes->at(gene_nodes->size()-1)->get_node_num();

    return 0;
}

bool Genotype::node_in_connection(int node_id)
{
    int i;
    for (i=0;i<this->gene_connections->size();i++)
    {
        if (this->gene_connections->at(i)->get_in()==node_id)
            return true;
        if (this->gene_connections->at(i)->get_out()==node_id)
            return true;
    }
    return false;
}

float Genotype::get_adjusted_fitness() const
{
    return this->adjusted_fitness;
}

float Genotype::get_spawn_amount() const
{
    return this->spawn_amount;
}

float Genotype::create_random(int range)
{
    float temp_random = (rand() % 2000 - 1000) / 1000.0;
    if (range!=0)
        temp_random = temp_random * range;
    else
        temp_random = temp_random / 2;
    return temp_random;
}

void Genotype::perturb(int which,float perturbed_value)
{
    float old_weight = this->gene_connections->at(which)->get_weight();
    this->gene_connections->at(which)->set_weight(old_weight+perturbed_value);
}

//if there exist hidden nodes, randomly pick one and link it to other node
int Genotype::link_a_hidden_node()
{
    int i;
    int temp_random;
    int hidden_node, non_hidden_node;
    //find a hidden node
    for (i=0;i<10;i++)
    {
        temp_random = rand() % this->gene_nodes->size();
        if (this->gene_nodes->at(temp_random)->get_type()==2)
        {
            hidden_node = this->gene_nodes->at(temp_random)->get_node_num();
            break;
        }
    }
    if (i>=10)
        return -1;
    //find a non hidden node
    for (i=0;i<10;i++)
    {
        temp_random = rand() % this->gene_nodes->size();
        if (this->gene_nodes->at(temp_random)->get_type()!=2)
        {
            non_hidden_node = this->gene_nodes->at(temp_random)->get_node_num();
            break;
        }
    }
    if (i>=10)
        return -1;

    Gene_connection *temp_connection;
    float temp_weight = this->create_random(10);//need better definition
    if (this->gene_nodes->at(temp_random)->get_type()==1
            ||this->gene_nodes->at(temp_random)->get_type()==0)
    {
        for (i=0;i<this->gene_connections->size();i++)
        {
            if (this->gene_connections->at(i)->get_in()==non_hidden_node && this->gene_connections->at(i)->get_out()==hidden_node)
                return -1;
        }
        temp_connection = new Gene_connection(non_hidden_node,hidden_node,temp_weight,true,Innovation::get_innovation_num()+1);
        this->add_gene_connection(temp_connection);
        Innovation::add_innovation(temp_connection);
    }
    else if (this->gene_nodes->at(temp_random)->get_type()==3)
    {
        for (i=0;i<this->gene_connections->size();i++)
        {
            if (this->gene_connections->at(i)->get_in()==hidden_node && this->gene_connections->at(i)->get_out()==non_hidden_node)
                return -1;
        }

            int temp_innovation_num = Innovation::check_if_innovation(hidden_node,non_hidden_node);
            if (temp_innovation_num==0)
                temp_innovation_num = Innovation::get_innovation_num()+1;


        temp_connection = new Gene_connection(hidden_node,non_hidden_node,temp_weight,1,temp_innovation_num);
        this->add_gene_connection(temp_connection);
        Innovation::add_innovation(temp_connection);
    }


}

void Genotype::set_adjusted_fitness(float adjusted)
{
    this->adjusted_fitness = adjusted;
}

bool Genotype::operator <(Genotype &g)
{
    return this->get_fitness()<g.get_fitness();
}

void Genotype::calculate_spawn_amount(float average)
{
    this->spawn_amount = this->adjusted_fitness / average;
}

Genotype::~Genotype()
{

    int i;
    for (i=0;i<this->gene_nodes->size();i++)
    {
        delete this->gene_nodes->at(i);
    }
    this->gene_nodes->clear();
    delete this->gene_nodes;
    this->gene_nodes = NULL;

    for (i=0;i<this->gene_connections->size();i++)
    {
        delete this->gene_connections->at(i);
    }
    this->gene_connections->clear();
    delete this->gene_connections;
    this->gene_connections = NULL;

}

int Genotype::get_connection_in(int which)
{
    if (which<this->gene_connections->size())
        return this->gene_connections->at(which)->get_in();
    else
        cout<<"no such connection!"<<endl;
    return 0;
}

int Genotype::get_connection_out(int which)
{
    if (which<this->gene_connections->size())
        return this->gene_connections->at(which)->get_out();
    else
        cout<<"no such connection!"<<endl;
    return 0;
}

float Genotype::get_connection_weight(int which)
{
    if (which<this->gene_connections->size())
        return this->gene_connections->at(which)->get_weight();
    else
        cout<<"no such connection!"<<endl;
    return 0;
}

bool Genotype::get_connection_enabled(int which)
{
    if (which<this->gene_connections->size())
        return this->gene_connections->at(which)->isEnabled();
    else
        cout<<"no such connection!"<<endl;
    return 0;
}

int Genotype::get_connection_size()
{
    return this->gene_connections->size();
}

void Genotype::set_weight(int which, float new_weight)
{
    int i;
    for (i=0;i<this->gene_connections->size();i++)
    {
        if (this->gene_connections->at(i)->get_innovation_num()==which)
        {
            this->gene_connections->at(i)->set_weight(new_weight);
            break;
        }
    }
}

void Genotype::set_connection_enabled(int which,bool enable)
{
//    int i;
//    for (i=0;i<this->gene_connections->size();i++)
//    {
//        if (this->gene_connections->at(i)->get_innovation_num()==which)
//        {
//            this->gene_connections->at(i)->set_enabled(enable);
//            break;
//        }
//    }
    this->gene_connections->at(which)->set_enabled(enable);
}

int Genotype::get_node_size()
{
    return this->gene_nodes->size();
}

void Genotype::write_genotype()
{
    ofstream outstream_node("/home/hjin/Nick/node.txt");
    ofstream outstream_connection("/home/hjin/Nick/connection.txt");
    int i;
    for (i=0;i<this->gene_nodes->size();i++)
    {
        outstream_node<<this->gene_nodes->at(i)->get_node_num()<<" "
                     <<this->gene_nodes->at(i)->get_type()<<" "
                    <<this->gene_nodes->at(i)->get_value()<<endl;
    }
    for (i=0;i<this->gene_connections->size();i++)
    {
        outstream_connection<<this->gene_connections->at(i)->get_in()<<" "
                           <<this->gene_connections->at(i)->get_out()<<" "
                          <<this->gene_connections->at(i)->get_weight()<<" "
                         <<this->gene_connections->at(i)->isEnabled()<<" "
                        <<this->gene_connections->at(i)->get_innovation_num()<<endl;
    }
    outstream_node.close();
    outstream_connection.close();
}

void Genotype::read_genotype()
{
    ifstream instream_node("./node.txt");
    ifstream instream_connection("./connection.txt");
    this->gene_nodes->clear();
    this->gene_connections->clear();
    Gene_node *temp_node;
    int temp_node_num, temp_type;
    float temp_value;
    while(instream_node>>temp_node_num)
    {
        instream_node>>temp_type>>temp_value;
        this->add_gene_node(new Gene_node(temp_node_num,temp_type,temp_value));
        //cout<<"node_num:"<<temp_node_num<<" type:"<<temp_type<<" value:"<<temp_value<<endl;
    }
    instream_node.close();

    int temp_in, temp_out;
    float temp_weight;
    bool temp_enabled;
    int temp_innovation_num;
    while(instream_connection>>temp_in)
    {
        instream_connection>>temp_out>>temp_weight>>temp_enabled>>temp_innovation_num;
        this->add_gene_connection(new Gene_connection(temp_in,temp_out,temp_weight,temp_enabled,temp_innovation_num));
    }
    instream_connection.close();

    cout<<"read successfully"<<endl;
//    this->print_genotype();

}
