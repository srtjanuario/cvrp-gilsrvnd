#include "perturbation.h"
#include "solution.h"
#include "construction.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <set>
#include <algorithm>
#include <vector>
#include <utility>
#include <cmath>
using namespace std;

void Perturbation::bridgePerturbation(Solution *s, int bridge)
{
    for (int vehicle = 0; vehicle < s->in->nVehicle(); vehicle++)
    {
        if(s->tour_[vehicle].size() < 9) 
            continue;
        
        vector<pair<int, int>> section(makeBridges(s, bridge, vehicle));
        vector<int> localSolution = s->tour_[vehicle];
    
        int t = 1;
        for (auto k:section)
            for(int i = k.first; i <= k.second; i++)
                s->tour_[vehicle][t++] = localSolution[i];
    }
    s->totalDistance_ = 0;
    s->totalDistance();
}

void Perturbation::reinsertion(Solution *s)
{
    // Backup
    vector<vector<int> > tour = s->tour_;
    vector<double> capacity = s->capacityVehicle_;
    vector<int> location;
    int few = INT_MAX;
    int bunch = INT_MIN;

    for (int vehicle = 0; vehicle < s->in->nVehicle(); vehicle++)
        if( tour[vehicle].size()-2 > 0 && tour[vehicle].size()-2 < few)
            few = tour[vehicle].size()-2;

    few = (few+1)/2;

    for (int vehicle = 0; vehicle < s->in->nVehicle(); vehicle++)
        for (int f = 0; f < few; f++){
            if(tour[vehicle].size()-2){
                int l = rand()%(tour[vehicle].size()-2)+1;
                location.push_back(tour[vehicle][l]);
                capacity[vehicle] += s->in->demand(tour[vehicle][l]);
                tour[vehicle].erase(tour[vehicle].begin()+l);
            }
        }

    while(location.size()){
        double increase;
        double best = INT_MAX;
        int best_v;
        int best_p;
        int best_l = -1;
        for(int l = 0; l < location.size(); l++){
            for(int v = 0; v < tour.size(); v++){
                for(int p = 1; p < tour[v].size()-1; p++){
                    if(capacity[v] - s->in->demand(location[l]) >= 0){
                        increase = s->in->distance(tour[v][p-1],location[l]) + s->in->distance(tour[v][p],location[l]);
                        if(increase < best){
                            best = increase;
                            best_v = v;
                            best_p = p;
                            best_l = l;
                        }
                    }
                }
            }
        }
        capacity[best_v] -= s->in->demand(location[best_l]);
        tour[best_v].insert(tour[best_v].begin()+best_p,location[best_l]);
        location.erase(location.begin()+best_l);  
        // cout<<"."; 
    }

    s->tour_ = tour;
    
}

vector<pair<int, int>> Perturbation::makeBridges(const Solution *s, int bridges, int vehicle)
{
    int down = floor((s->tour_[vehicle].size()-1)/4.0);
    int up = ceil((s->tour_[vehicle].size()-1)/4.0);
    pair<int,int> a = make_pair(1,down);
    pair<int,int> b = make_pair(a.second+1,a.second+up);
    pair<int,int> c = make_pair(b.second+1,b.second+down);
    pair<int,int> d = make_pair(c.second+1,s->tour_[vehicle].size()-2);
    return {d,c,b,a};
}


// int main(int argc, char *argv[])
// {
// 	srand(time(0));
//     Input in(argc, argv);
//     Solution s(&in);
//     Construction c;
//     Perturbation p;

//     int Imax = 50;
// 	int Iils = (in.nLocation() >= 150) ? in.nLocation() / 2 : in.nLocation();
//     vector<double> R;
// 	R.push_back(0.00);
// 	for (int i = 1; i <= 25; i++)
// 		R.push_back(R[i - 1] + 0.01);
    
//     for (auto r: R){
//         c.constructiveProcedure(&s,r);
//         cout<<s<<endl;
//         p.reinsertion(&s);
//         cout<<s<<endl;
//         s.totalDistance_=0;
//         cout<<s<<endl;
//         exit(0);
//     }

//     return 0;
// }