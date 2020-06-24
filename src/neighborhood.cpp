#include <climits>
#include "neighborhood.h"
#include "perturbation.h"
#include "construction.h"
#include "solution.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <set>
#include <algorithm>
using namespace std;

Neighborhood::Neighborhood(Input *input)
{
    this->in = input;

    NL.push_back("bestSwap");
    NL.push_back("bestTwoOpt");
    NL.push_back("bestReInsertion-1");
    NL.push_back("bestReInsertion-2");
    NL.push_back("bestReInsertion-3");
    // NL.push_back("swapVehicle");
}

void Neighborhood::bestSwap(Solution *s)
{
    for (int vehicle = 0; vehicle < s->in->nVehicle(); vehicle++)
    {
        int i_best;
        int j_best;
        double delta = 0.0;
        double delta_best = INT_MAX;
        for (unsigned i = 1; i < s->tour_[vehicle].size() - 1; i++)
            for (unsigned j = i + 1; j < s->tour_[vehicle].size() - 1; j++)
            {
                delta = swapDeltaEvaluation(s, vehicle, i, j);
                if (delta + 0.0001 < 0.0 && delta + 0.0001 < delta_best)
                {
                    delta_best = delta;
                    i_best = i;
                    j_best = j;
                }
            }
        if (delta_best + 0.0001 < 0.0)
            swapMove(s, vehicle, i_best, j_best, delta_best);
    }
}

double Neighborhood::swapDeltaEvaluation(Solution *s, int vehicle, int i, int j)
{

    if (i > j)
        swap(i, j);

    if (i + 1 == j)
    { //It doesn't work for non simmetric instances
        return in->distance(s->tour_[vehicle][j], s->tour_[vehicle][i - 1]) +
               in->distance(s->tour_[vehicle][i], s->tour_[vehicle][j + 1]) -
               in->distance(s->tour_[vehicle][i], s->tour_[vehicle][i - 1]) -
               in->distance(s->tour_[vehicle][j], s->tour_[vehicle][j + 1]);
    }
    else
    { // Exactly like in "O kit"
        return in->distance(s->tour_[vehicle][i], s->tour_[vehicle][j - 1]) +
               in->distance(s->tour_[vehicle][i], s->tour_[vehicle][j + 1]) +
               in->distance(s->tour_[vehicle][j], s->tour_[vehicle][i - 1]) +
               in->distance(s->tour_[vehicle][j], s->tour_[vehicle][i + 1]) -
               in->distance(s->tour_[vehicle][i], s->tour_[vehicle][i - 1]) -
               in->distance(s->tour_[vehicle][i], s->tour_[vehicle][i + 1]) -
               in->distance(s->tour_[vehicle][j], s->tour_[vehicle][j - 1]) -
               in->distance(s->tour_[vehicle][j], s->tour_[vehicle][j + 1]);
    }
}

void Neighborhood::swapMove(Solution *s, int vehicle, int a, int b, double delta)
{
    if (a > b)
        swap(a, b);

    swap(s->tour_[vehicle][a], s->tour_[vehicle][b]);
    if (a == 0)
        s->tour_[vehicle][s->tour_[vehicle].size() - 1] = s->tour_[vehicle][a];

    s->tourDistance_[vehicle] += delta;
    s->totalDistance_ += delta;
}


void Neighborhood::swapVehicle(Solution *s)
{
    int l1_best;
    int l2_best;
    int v1_best;
    int v2_best;
    pair<double,double> zero = make_pair(0,0);
    pair<double,double> delta = zero;
    pair<double,double> delta_best = make_pair(INT_MAX/2.0,INT_MAX/2.0);
    for (int v1 = 0; v1 < s->in->nVehicle(); v1++)
        for (int v2 = v1 + 1; v2 < s->in->nVehicle(); v2++)
            for (int l1 = 1; l1 < s->tour_[v1].size() - 1; l1++)
                for (int l2 = 1; l2 < s->tour_[v2].size() - 1; l2++)
                {
                    delta = swapVehicleDeltaEvaluation(s, v1, v2, l1, l2);
                    if (delta < zero && delta  < delta_best)
                    {
                        delta_best = delta;
                        l1_best = l1;
                        l2_best = l2;
                        v1_best = v1;
                        v2_best = v2;
                    }
                }

        if (delta_best < zero)
            swapVehicleMove(s, v1_best, v2_best, l1_best, l2_best, delta_best);

}

pair<double,double> Neighborhood::swapVehicleDeltaEvaluation(Solution *s, int v1, int v2, int l1, int l2)
{
    // cout<<*s<<endl;
    if(s->capacityVehicle(v1) + s->in->demand(s->tour_[v1][l1]) - s->in->demand(s->tour_[v2][l2]) < 0)
        return make_pair(INT_MAX/2-1,INT_MAX/2-1);
    if(s->capacityVehicle(v2) + s->in->demand(s->tour_[v2][l2]) - s->in->demand(s->tour_[v1][l1]) < 0)
        return make_pair(INT_MAX/2-1,INT_MAX/2-1);

    int first = in->distance(s->tour_[v1][l1-1], s->tour_[v1][l2]) +
           in->distance(s->tour_[v1][l2], s->tour_[v1][l1+1]) -
           in->distance(s->tour_[v1][l1-1], s->tour_[v1][l1]) -
           in->distance(s->tour_[v1][l1], s->tour_[v1][l1 + 1]);
           
    int second = in->distance(s->tour_[v2][l2-1], s->tour_[v2][l1]) +
           in->distance(s->tour_[v2][l1], s->tour_[v2][l2 + 1]) -
           in->distance(s->tour_[v2][l2], s->tour_[v2][l2 - 1]) -
           in->distance(s->tour_[v2][l2], s->tour_[v2][l2 + 1]);

    return make_pair(first,second);
}

void Neighborhood::swapVehicleMove(Solution *s, int v1, int v2, int l1, int l2, pair<double,double> delta)
{
    swap(s->tour_[v1][l1], s->tour_[v2][l2]);
    s->totalDistance_ += delta.first + delta.second;
    s->capacityVehicle_[v1] = s->in->demand(s->tour_[v1][l1]) - s->in->demand(s->tour_[v2][l2]);
    s->capacityVehicle_[v2] = s->in->demand(s->tour_[v2][l2]) - s->in->demand(s->tour_[v1][l1]);
    s->tourDistance_[v1] += delta.first;
    s->tourDistance_[v2] += delta.second;

}


void Neighborhood::bestTwoOpt(Solution *s)
{
    for (int vehicle = 0; vehicle < s->in->nVehicle(); vehicle++)
    {
        bool stuck = false;
        double delta = 0.0;

        int i_best;
        int j_best;
        double delta_best;

        int last = s->tour_[vehicle].size() - 1;
        //while(!stuck){
        stuck = true;
        delta_best = INT_MAX;
        for (int i = 1; i < last; i++)
            for (int j = i + 2; j < last; j++)
            {
                if (!((i == 0 && j == last - 1) || (abs(i - j) < 2)))
                {
                    delta = twoOptDeltaEvaluation(s, vehicle, i, j);
                    if (delta + 0.0001 < 0.0 && delta + 0.0001 < delta_best)
                    {
                        delta_best = delta;
                        i_best = i;
                        j_best = j;
                    }
                }
            }
        if (delta_best + 0.0001 < 0.0)
        {
            twoOptMove(s, vehicle, i_best, j_best, delta_best);
            stuck = false;
        }
    }
}

double Neighborhood::twoOptDeltaEvaluation(Solution *s, int vehicle, int i, int j)
{
    if (i > j)
        swap(i, j);

    return in->distance(s->tour_[vehicle][i], s->tour_[vehicle][j]) +
           in->distance(s->tour_[vehicle][i + 1], s->tour_[vehicle][j + 1]) -
           in->distance(s->tour_[vehicle][i], s->tour_[vehicle][i + 1]) -
           in->distance(s->tour_[vehicle][j], s->tour_[vehicle][j + 1]);
}

void Neighborhood::twoOptMove(Solution *s, int vehicle, int i, int j, double delta)
{
    if (i > j)
        swap(i, j);
    reverse(s->tour_[vehicle].begin() + i + 1, s->tour_[vehicle].begin() + j + 1);

    s->tourDistance_[vehicle] += delta;
    s->totalDistance_ += delta;
    
}


void Neighborhood::bestReInsertion(Solution *s, int size)
{
    for (int vehicle = 0; vehicle < s->in->nVehicle(); vehicle++)
    {
        bool stuck = false;
        double delta = 0.0;
        int origin_best;
        int destination_best;
        double delta_best;
        int last = s->tour_[vehicle].size() - 1;

        //while(!stuck){
        stuck = true;
        delta_best = INT_MAX;
        for (int origin = 1; origin < last; origin++)
        {
            for (int destination = 1; destination < last; destination++)
            {
                delta = reInsertionDeltaEvaluation(s,vehicle, origin, destination, size);
                if (delta + 0.0001 < 0.0 && delta + 0.0001 < delta_best)
                {
                    delta_best = delta;
                    origin_best = origin;
                    destination_best = destination;
                }
            }
        }
        if (delta_best + 0.0001 < 0.0)
        {
            reInsertionMove(s,vehicle, origin_best, destination_best, size, delta_best);
            stuck = false;
        }
    }
}

double Neighborhood::reInsertionDeltaEvaluation(Solution *s, int vehicle, int origin, int destination, int size)
{
    double delta = 0;
    int last = s->tour_[vehicle].size() - 1;
    if (origin + size <= destination && (destination + 1) % last != origin)
    {
        if (origin == 0)
            delta -= in->distance(s->tour_[vehicle][last - 1], s->tour_[vehicle][origin]);
        else
            delta -= in->distance(s->tour_[vehicle][origin - 1], s->tour_[vehicle][origin]);

        delta -= in->distance(s->tour_[vehicle][origin + size - 1], s->tour_[vehicle][origin + size]);
        delta -= in->distance(s->tour_[vehicle][destination], s->tour_[vehicle][destination + 1]);

        if (origin == 0)
            delta += in->distance(s->tour_[vehicle][last - 1], s->tour_[vehicle][origin + size]);
        else
            delta += in->distance(s->tour_[vehicle][origin - 1], s->tour_[vehicle][origin + size]);
        delta += in->distance(s->tour_[vehicle][origin], s->tour_[vehicle][destination]);
        delta += in->distance(s->tour_[vehicle][destination + 1], s->tour_[vehicle][origin + size - 1]);

        return delta;
    }
    return INT_MAX;
}

void Neighborhood::reInsertionMove(Solution *s, int vehicle, int origin, int destination, int size, double delta)
{
    s->tourDistance_[vehicle] += delta;
    s->totalDistance_ += delta;
    int new_origin;
    int new_destination;
    int last = s->tour_[vehicle].size() - 1;
    if (origin < destination)
    {
        rotate(s->tour_[vehicle].begin() + origin, s->tour_[vehicle].begin() + origin + size, s->tour_[vehicle].begin() + destination + 1);
    }
    else if (origin > destination)
    {
        if (origin + size < int(s->tour_[vehicle].size()))
        {
            rotate(s->tour_[vehicle].begin() + destination, s->tour_[vehicle].begin() + origin, s->tour_[vehicle].begin() + origin + size);
        }
        else
        {
            new_origin = (last + (origin - size) % last) % (last);
            new_destination = (last + (destination - size) % last) % (last);
            rotate(s->tour_[vehicle].begin(), s->tour_[vehicle].begin() + size, s->tour_[vehicle].end() - 1);
            if (new_origin > new_destination)
            {
                rotate(s->tour_[vehicle].begin() + new_destination, s->tour_[vehicle].begin() + new_origin, s->tour_[vehicle].begin() + new_origin + size);
            }
            else if (new_origin < new_destination)
            {
                rotate(s->tour_[vehicle].begin() + new_origin, s->tour_[vehicle].begin() + new_origin + size, s->tour_[vehicle].begin() + new_destination + 1);
            }
        }
    }
    else
    {
        cout << __FILE__ << __LINE__ << endl;
        cout << "Origin and destination are the same!" << endl;
    }
    s->tour_[vehicle][last] = s->tour_[vehicle][0];
}



void Neighborhood::improve(Solution *s, string choosenNeighborhood)
{
    if (choosenNeighborhood == "bestSwap")
        bestSwap(s);
    else if (choosenNeighborhood == "bestTwoOpt")
        bestTwoOpt(s);
    else if (choosenNeighborhood == "bestReInsertion-1")
        bestReInsertion(s, 1);
    else if (choosenNeighborhood == "bestReInsertion-2")
        bestReInsertion(s, 2);
    else if (choosenNeighborhood == "bestReInsertion-3")
        bestReInsertion(s, 3);
    else if (choosenNeighborhood == "swapVehicle")
        swapVehicle(s);
        
}

// int main(int argc, char *argv[])
// {
// 	srand(time(0));
//     Input in(argc, argv);
//     Solution s(&in);
//     Construction c;
//     Perturbation p;
//     Neighborhood n(&in);

//     int Imax = 50;
// 	int Iils = (in.nLocation() >= 150) ? in.nLocation() / 2 : in.nLocation();
//     vector<double> R;
// 	R.push_back(0.00);
// 	for (int i = 1; i <= 25; i++)
// 		R.push_back(R[i - 1] + 0.01);
    
//     int best = INT_MAX;
//     for (auto r: R){
//         c.constructiveProcedure(&s,r);
//         for(int i = 0; i < 5000; i++){
//         n.bestTwoOpt(&s);
//         n.bestSwap(&s);
//         }
//         if(s.totalDistance() < best){
//             cout<<s<<endl;
//             best = s.totalDistance();
//         }
//         p.bridgePerturbation(&s,4);
//     }

//     return 0;
// }