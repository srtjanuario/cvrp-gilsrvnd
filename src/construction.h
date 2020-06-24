#ifndef CONSTRUCTION_H_INCLUDED
#define CONSTRUCTION_H_INCLUDED
#include "solution.h"

class 
Construction{
    public:
        void calculaCustoInsercao(Solution* s, int vehicle);
        void constructiveProcedure(Solution* s, const double alpha);

    struct InsertionInfo {
        int noInserido; // no k a ser inserido
        int arestaRemovida; // aresta {i,j} onde o no k sera inserido
        double custo ; // delta ao inserir k na aresta {i , j} 
    };

    static bool compareByCost(const InsertionInfo &a, const InsertionInfo &b)
    {
        return a.custo < b.custo;
    }
    
    //private:
    vector<int> CL;
    vector<InsertionInfo> custoInsercao; 

    friend ostream & operator << (ostream &out, const Construction &c);
};

#endif //CONSTRUCTION_H_INCLUDED