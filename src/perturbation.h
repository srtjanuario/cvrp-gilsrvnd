#ifndef PERTURBATION_H_INCLUDED
#define PERTURBATION_H_INCLUDED
#include "solution.h"

class Perturbation{
    public:
        /**
         * Double Bridge
        */
        void bridgePerturbation(Solution* s, int maxBridges);
        vector<pair<int,int> > makeBridges(const Solution* s,int bridge,int vehicle);

    //private:
    friend ostream & operator << (ostream &out, const Perturbation &c);
    
};

#endif //PERTURBATION_H_INCLUDED