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
//         p.bridgePerturbation(&s,4);
//         cout<<s<<endl;
        
//     }

//     return 0;
// }