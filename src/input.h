#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include <iostream>
#include <vector>
using namespace std;

class Input
{
public:
    Input(int, char **);

    int nLocation() const;
    void nLocationSet(unsigned d);

    int nVehicle() const;
    void nVehicleSet(unsigned d);

    double capacity() const;
    void capacitySet(unsigned d);

    double demand(unsigned i) const;
    void demandSet(unsigned i, double value);

    double distance(unsigned i, unsigned j);
    void distanceSet(unsigned i, unsigned j, double value);

    friend ostream &operator<<(ostream &out, Input &c);

private:
    int nLocation_;
    int nVehicle_;
    int capacity_;
    vector<double> demand_;
    vector<vector<double> > distance_;
};

#endif //INPUT_H_INCLUDED