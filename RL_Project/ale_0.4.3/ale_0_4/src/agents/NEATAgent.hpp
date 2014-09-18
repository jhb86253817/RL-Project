
#ifndef __NEAT_AGENT_HPP__
#define __NEAT_AGENT_HPP__

#include "../common/Constants.h"
#include "PlayerAgent.hpp"
#include "../emucore/OSystem.hxx"
#include "../NEAT/genotype.h"
#include "../NEAT/generation.h"

class NEATAgent : public PlayerAgent {
    public:
        NEATAgent(OSystem * _osystem, RomSettings * _settings);
        void episode_end(void);
        void get_episode_score(int score);

		
	protected:
        /* *********************************************************************
            Returns the best action from the set of possible actions
         ******************************************************************** */
        virtual Action act();

    private:
        Generation *generation;
        vector<Genotype*> all_genotypes;
        Genotype *chosen_genotype;
        int current_genotype;
};

#endif // __NEAT_AGENT_HPP__
