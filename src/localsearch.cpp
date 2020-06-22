#include "neighborhood.h"
#include "localsearch.h"
#include "solution.h"
#include "construction.h"
#include "perturbation.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <set>
#include <vector>
#include <algorithm> 
using namespace std;

LocalSearch::LocalSearch( Input* input)
{
    this->in = input;
    s =  new Solution(in);
    s_star =  new Solution(in);
    s_line =  new Solution(in);
    s_rvnd =  new Solution(in);
    c = new Construction(s);
    p = new Perturbation(in);
    n = new Neighborhood(in);
}

void LocalSearch::reset(){
    s_star->reset();
    s_line->reset();
}

// GILS-RVND from EJOR2012-Marcos
Solution LocalSearch::GILSRVND(int Imax, int Iils, vector<double> &R){
    reset();

    for(int i = 1; i <= Imax; i++){
        alpha = randomValue(R);        
        c->constructiveProcedure(s,alpha);
        (*s_line) = (*s);
        iterILS = 0;
        while(iterILS < Iils){
            RVND(s);
            if(f(s) < f(s_line)){
                (*s_line) = (*s);
                iterILS = 0;
            }//end_if
            (*s) = p->bridgePerturbation(s_line,4);
            iterILS = iterILS + 1;
        }//end_while
        if(f(s_line) < f(s_star)){
            (*s_star) = (*s_line);
        }
    }
    return (*s_star);
}

double LocalSearch::f(Solution* s){
    
        return s->costValueTSP;
    
}

double LocalSearch::randomValue(vector<double> R){
    return R[rand()%R.size()];
}

string LocalSearch::randomNeighborhood(){
    return NL[rand()%NL.size()];
}

void LocalSearch::deleteNeighborhood(string choosenNeighborhood){
    auto it = find(NL.begin(), NL.end(), choosenNeighborhood);
    if(it != NL.end())
        NL.erase(it);
}

void LocalSearch::RVND(Solution* s){
    (*s_rvnd) = (*s);
    NL = n->NeighborhoodList();
    string choosenNeighborhood;
    while(NL.size() != 0){
        choosenNeighborhood = randomNeighborhood();
        n->improove(s_rvnd,choosenNeighborhood);
        if(f(s_rvnd) < f(s)){
            (*s) = (*s_rvnd);
        }
        else
            deleteNeighborhood(choosenNeighborhood);
    }
}