#include "construction.h"
#include "solution.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <set>
#include <algorithm>
#include <climits>
using namespace std;

void Construction::constructiveProcedure(Solution *s, const double alpha)
{
    s->totalDistance_ = 0;
    for(int v = 0; v < s->in->nVehicle(); v++){
        CL.clear();
        for(int i = 1; i < s->tour_[v].size()-1; i++)
            CL.push_back(s->tour_[v][i]);

        s->tour_[v] = {0,0};
        s->tourDistance_[v] = 0;
        calculaCustoInsercao(s,v);
        while (CL.size() > 0)
        {
            int position = rand() % (int(floor(custoInsercao.size() * alpha)) + 1);
            s->tour_[v].insert(s->tour_[v].begin() + custoInsercao[position].arestaRemovida + 1, custoInsercao[position].noInserido);
            s->tourDistance_[v] += custoInsercao[position].custo;
            remove(CL.begin(), CL.end(), custoInsercao[position].noInserido);
            CL.resize(CL.size() - 1);
            calculaCustoInsercao(s,v);
        }
        s->totalDistance_+=s->tourDistance_[v];
    }
}

void Construction::calculaCustoInsercao(Solution *s, int vehicle)
{

    custoInsercao.resize((s->tour_[vehicle].size() - 1) * CL.size());

    for (unsigned i = 0, j = 1, l = 0; i < s->tour_[vehicle].size() - 1; i++, j++)
    {
        for (auto k : CL)
        {
            custoInsercao[l].custo =
                s->in->distance(s->tour_[vehicle][i], k) +
                s->in->distance(s->tour_[vehicle][j], k) -
                s->in->distance(s->tour_[vehicle][i], s->tour_[vehicle][j]);
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = i;
            l++;
        }
    }
    sort(custoInsercao.begin(), custoInsercao.end(), compareByCost);
}

ostream &operator<<(ostream &out, const Construction &c)
{

    out << "noInserido"
        << " | "
        << "arestaRemovida"
        << " | "
        << "custo" << endl;

    for (size_t i = 0; i < c.custoInsercao.size(); i++)
    {
        out << setw(10) << c.custoInsercao[i].noInserido << " | " << setw(14) << c.custoInsercao[i].arestaRemovida << " | " << setw(5) << c.custoInsercao[i].custo << endl;
    }
    return out;
}

// int main(int argc, char *argv[])
// {
//     Input in(argc, argv);
//     Solution s(&in);
//     Construction c;
//     c.constructiveProcedure(&s,0.1);
//     return 0;
// }