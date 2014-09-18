
#include "NEATAgent.hpp"
#include "random_tools.h"

NEATAgent::NEATAgent(OSystem* _osystem, RomSettings* _settings) :
    PlayerAgent(_osystem, _settings) {

    //for training, uncomment the code
//    this->generation = new Generation(10);
//    this->generation->get_all_genotypes(this->all_genotypes);
//    this->current_genotype = 0;
    this->chosen_genotype = new Genotype();
    this->chosen_genotype->read_genotype();
}

Action NEATAgent::act() {
    //Get original image and then transform it to binary vectors as inputs of NEAT
    //------------------------------------------------------------------
    int screen_width = p_osystem->console().mediaSource().width();//160
    int screen_height = p_osystem->console().mediaSource().height();//210

    //Initialization
    IntMatrix screen_matrix;
    for (int i=0; i<screen_height; i++) {
        IntVect row;
        for (int j=0; j<screen_width; j++)
            row.push_back(-1);
        screen_matrix.push_back(row);
    }
    //get color image
    uInt8* pi_curr_frame_buffer = p_osystem->console().mediaSource().currentFrameBuffer();
    int ind_i, ind_j;
    for (int i = 0; i < screen_width * screen_height; i++) {
        uInt8 v = pi_curr_frame_buffer[i];
        ind_i = i / screen_width;
        ind_j = i - (ind_i * screen_width);
        screen_matrix[ind_i][ind_j] = v;
    }

    //this->p_osystem->p_export_screen->save_png(&screen_matrix,"222.png");

    //Initialization
    IntMatrix screen_matrix_grayscale;
    for (int i=0; i<screen_height; i++) {
        IntVect row;
        for (int j=0; j<screen_width; j++)
            row.push_back(-1);
        screen_matrix_grayscale.push_back(row);
    }
    //get grayscale image
    int r=0, g=0, b=0;
    for (int i=0; i<screen_height; i++)
    {
        for (int j=0; j<screen_width; j++)
        {
            this->p_osystem->p_export_screen->get_rgb_from_palette(screen_matrix[i][j], r, g, b);
            screen_matrix_grayscale[i][j] = 0.2126*r + 0.7152*g + 0.0722*b;
        }
    }

    //Initialization
    IntMatrix screen_matrix_binary;
    for (int i=0; i<screen_height; i++) {
        IntVect row;
        for (int j=0; j<screen_width; j++)
            row.push_back(0);
        screen_matrix_binary.push_back(row);
    }
    //get binary image
    int sum[100] = {0};
    int k=0;
    for (int i=90; i<=180; i=i+10)
    {
        for (int j=30; j<=120; j=j+10)
        {
            for (int ii=i;ii<i+10;ii++)
            {
                for (int jj=j;jj<j+10;jj++)
                {
                    sum[k] += screen_matrix_grayscale[ii][jj];
                }
            }
            sum[k] = sum[k] / 100;
            k++;
        }
    }
    int sum_total = 0;
    for (int i=0;i<100;i++)
    {
        sum_total += sum[i];
    }
    sum_total = sum_total / 100;

    //transform binary image to binary vector
    for (int i=0; i<100; i++)
    {
        if (sum[i]>sum_total)
            sum[i] = 1;
        else
            sum[i] = 0;
    }
    //-----------------------------------------------------------------------------
    //testing
//    k=0;
//    for (int i=90; i<=180; i=i+10)
//    {
//        for (int j=30; j<=120; j=j+10)
//        {
//            for (int ii=i;ii<i+10;ii++)
//            {
//                for (int jj=j;jj<j+10;jj++)
//                {
//                    if(sum[k]==1)
//                    {
//                        screen_matrix_binary[ii][jj] = 50;
//                    }
//                }
//            }
//            k++;
//        }
//    }
    //-------------------------------------------------------------------------------

//testing
//    this->p_osystem->p_export_screen->save_png(&screen_matrix,"color.png");
//    this->p_osystem->p_export_screen->save_png(&screen_matrix_grayscale,"grayscale.png");
//    this->p_osystem->p_export_screen->save_png(&screen_matrix_lbp,"lbp.png");
//    this->p_osystem->p_export_screen->save_png(&screen_matrix_binary,"binary.png");

    //------------------------------------------------------------------

    this->chosen_genotype->clear_node_value();
    this->chosen_genotype->activate(sum);
    int output = this->chosen_genotype->output_value();
    return available_actions.at(output);
}

void NEATAgent::episode_end(void)
{
    episode_number++;


  if (i_max_num_episodes > 0 && episode_number >= i_max_num_episodes)
    end_game();

  //--------------------------------------------------------------------------
  //for training, uncomment the code
  /*
  this->current_genotype++;
  if (this->current_genotype>=this->all_genotypes.size())
  {
      this->current_genotype = 0;
      this->generation->sort_by_fitness();
      this->generation->sepciate(15);
      this->generation->sort_by_fitness();
      this->generation->adjust_fitness();
      this->generation->calculate_spawn_amount();
      this->generation->spawn();
      this->all_genotypes.clear();
      this->generation->get_all_genotypes(this->all_genotypes);
      this->generation->print_one_of_best();
  }
*/
}

void NEATAgent::get_episode_score(int score)
{
    //for training, uncomment the code
//    this->all_genotypes.at(this->current_genotype)->set_fitness(score);
}
