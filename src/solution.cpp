#include "solution.h"
#include <cmath>
#include <climits>
#include <random>
#include <ctime>
using namespace std;

Solution::Solution(Input *input)
{
    this->in = input;
    totalDistance_ = 0;

    capacityVehicle_ = vector<double>(in->nVehicle(), in->capacity());
    tour_.resize(in->nVehicle());
    tourDistance_ = vector<double>(in->nVehicle(), 0.0);
    // Build a trivial solution

    vector<int> location;
    do
    {
        location.resize(0);
        for (int i = 1; i < in->nLocation(); i++)
            location.push_back(i);
        
        shuffle(location.begin(), location.end(), default_random_engine(time(0)));

        for (int v = 0; v < in->nVehicle(); v++){
            tour_[v] = {0};
            capacityVehicle_[v]=in->capacity();
        }

        for (int v = 0; v < in->nVehicle() && location.size(); v++)
        {
            for( int l = 0; l < location.size();)
            {
                if(in->demand(location[l]) <= capacityVehicle_[v]){
                    capacityVehicleSet(v, capacityVehicle(v) - in->demand(location[l]));
                    tour_[v].push_back(location[l]);
                    location.erase(location.begin()+l);
                    l--;
                }
                l++;
            }
        }

        for (int v = 0; v < in->nVehicle(); v++)
            tour_[v].push_back(0);
    } while (location.size());
}

double Solution::totalDistance()
{
    if (this->totalDistance_)
        return this->totalDistance_;

    for (int v = 0; v < in->nVehicle(); v++)
    {
        tourDistance_[v] = 0;
        capacityVehicle_[v] = in->capacity();
        capacityVehicle_[v] -= in->demand(tour_[v][0]);
        for (int i = 1; i < tour_[v].size(); i++){
            tourDistance_[v] += in->distance(tour_[v][i], tour_[v][i -1]);
            capacityVehicle_[v] -= in->demand(tour_[v][i]);
        }
        if(capacityVehicle_[v] < 0)
            cout<<"Vehicle "<<v<<" is over the capacity limit by "<<-capacityVehicle_[v]<<endl;
        totalDistance_ += tourDistance_[v];
    }
    return totalDistance_;
}

double Solution::capacityVehicle(int vehicle) const
{
    return this->capacityVehicle_[vehicle];
}

void Solution::capacityVehicleSet(int vehicle, double value)
{
    this->capacityVehicle_[vehicle] = value;
}

ostream &operator<<(ostream &out, Solution &s)
{
    out << s.totalDistance() << " 0"
        // << "\\n"; 
        << endl;

    for (int v = 0; v < s.in->nVehicle(); v++)
    {
        // cout<<s.capacityVehicle(v)<<" - ";
        for (int i = 0; i < s.tour_[v].size(); i++)
        {
            out << s.tour_[v][i];
            if (i < s.tour_[v].size() - 1)
                out << " ";
        }
        // out << "\\n";
        out << endl;
    }
    return out;
}

// int main(int argc, char *argv[])
// {
//     Input in(argc, argv);
//     // cout << in << endl;
//     Solution s(&in);
//     cout << s << endl;
//     return 0;
// }