#ifndef NEIGHBORHOOD_H_INCLUDED
#define NEIGHBORHOOD_H_INCLUDED
#include "solution.h"

class Neighborhood{
    public:
        // Neighborhood(Input *input, Statistics *st);
        Neighborhood(Input *input);
        vector<string> NeighborhoodList(){return NL;};
        void improove(Solution* s,string choosenNeighborhood);
        /**
         * SWAP
        */
        void bestSwap(Solution* s);
        void firstSwap(Solution* s);
        double swapDeltaEvaluation(Solution* s,int i,int j);
        void swapMove(Solution* s,int i,int j, double delta);

        /**
         * 2-opt
        */
        void firstTwoOpt(Solution* s);
        void bestTwoOpt(Solution* s);
        double twoOptDeltaEvaluation(Solution* s,int arci,int arcj);
        void twoOptMove(Solution* s, int arci,int arcj, double delta);

        /**
         * reInsertion
        */
        void firstReInsertion(Solution* s, int size = 1);
        void bestReInsertion(Solution* s, int size = 1 );
        double reInsertionDeltaEvaluation(Solution* s, int arc, int size, int shift);
        void reInsertionMove(Solution* s, int arc, int size, int shift, double delta);
        

    //private:
    Input* in;
    vector<string> NL;
    // vector<int> reInsertionAuxVector;
    friend ostream & operator << (ostream &out, const Neighborhood &c);
    vector< pair<int,int> > sigma;

};

#endif //NEIGHBORHOOD_H_INCLUDED