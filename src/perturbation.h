#ifndef PERTURBATION_H_INCLUDED
#define PERTURBATION_H_INCLUDED
#include "solution.h"

class Perturbation
{
public:
    void bridgePerturbation(Solution *s, int maxBridges);
    void reinsertion(Solution *s);
    vector<pair<int, int> > makeBridges(const Solution *s, int bridge, int vehicle);

    friend ostream &operator<<(ostream &out, const Perturbation &c);
};

#endif //PERTURBATION_H_INCLUDED