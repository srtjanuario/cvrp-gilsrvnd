#include "input.h"

#include <climits>
#include <cmath>
#include <fstream>
using namespace std;

int Input::nLocation() const
{
    return this->nLocation_;
}

void Input::nLocationSet(unsigned d){
    this->nLocation_ = d;
}

int Input::nVehicle() const{
    return this->nVehicle_;
}

void Input::nVehicleSet(unsigned d){
    this->nVehicle_ = d;
}

double Input::capacity() const{
    return this->capacity_;
}

void Input::capacitySet(unsigned d){
    this->capacity_ = d;
}

double Input::demand(unsigned i) const{
    return this->demand_[i];
}

void Input::demandSet(unsigned i, double value){
    this->demand_[i] = value;
}

double Input::distance(unsigned i, unsigned j){
    return this->distance_[i][j];
}

void Input::distanceSet(unsigned i, unsigned j, double value){
    this->distance_[i][j] = value;
}

ostream &operator<<(ostream &out, Input &in)
{
    out << in.nLocation() <<' '<< in.nVehicle() <<' '<< in.capacity()<<endl;
    for (int i = 0; i < in.nLocation(); i++)
    {
        for (int j = 0; j < in.nLocation(); j++)
        {
            out << in.distance(i, j) << "\t";
        }
        out << endl;
    }
    return out;
}

Input::Input(int argc, char **argv)
{

    if (argc < 2)
    {
        cout << "./cvrp [instance] " << endl;
        exit(1);
    }

    ifstream in(argv[1], ios::in);

    if (!in)
    {
        cerr << "File error: "<<argv[1]<<endl;
        exit(1);
    }

    in>>nLocation_>>nVehicle_>>capacity_;
    distance_ = vector<vector<double> >(nLocation(),vector<double>(nLocation(),numeric_limits<double>::infinity()));
    demand_ = vector<double>(nLocation());

    vector<double> x(nLocation());
    vector<double> y(nLocation());
    for(int i = 0; i < nLocation(); i++)
        in>>demand_[i]>>x[i]>>y[i];

    for(int i = 0; i < nLocation(); i++)
        for(int j = 0; j < nLocation(); j++)
            distance_[i][j] = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));
    }

    // int main(int argc, char* argv[]){
    //     Input in(argc,argv);
    //     cout<<in<<endl;
    //     return 0;
    // }