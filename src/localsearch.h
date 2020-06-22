#ifndef LOCALSEARCH_H_INCLUDED
#define LOCALSEARCH_H_INCLUDED
#include "solution.h"
#include "construction.h"
#include "perturbation.h"
#include "neighborhood.h"
#include <vector>
#include <string>

class LocalSearch{
    public:
        LocalSearch(Input *input);
        Solution GILSRVND(int Imax, int Iils, vector<double> &R);
        void RVND(Solution* s);
        double randomValue(vector<double> R);
        string randomNeighborhood();
        void deleteNeighborhood(string choosenNeighborhood);
        double f(Solution* s);
        void reset();
    private:
        vector<string> NL;
        int iterILS;
        double alpha;
        Solution* s;
        Solution* s_line;
        Solution* s_star;
        Solution* s_rvnd;
        Construction* c;
        Perturbation* p;
        Neighborhood* n;
        Input* in;
};

#endif //LOCALSEARCH_H_INCLUDED