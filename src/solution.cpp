#include "solution.h"
#include <cmath>
#include <climits>
using namespace std;

Solution::Solution(Input *input)
{

    this->in = input;

    // Create a generic basic solution
    for (unsigned i = 1; i <= this->in->dimensionGet(); i++)
        location.push_back(i);
    location.push_back(1);

    // Resize already fill the ellements with zeros!!!
    // duration.resize(in->dimensionGet()+1, vector<int>(in->dimensionGet()+1));
    // cost.resize(in->dimensionGet()+1, vector<int>(in->dimensionGet()+1));

    computeCostValueTSP();
    // computeCostValueMLP();
}

double Solution::t_(unsigned i, unsigned j)
{
    return in->distanceGet(location[i], location[j]);
}

void Solution::reset()
{
    costValueTSP = INT_MAX;
    location.resize(0);
}

void Solution::computeCostValueTSP()
{
    costValueTSP = 0.0;
    for (unsigned i = 0; i < location.size() - 1; i++)
        costValueTSP += in->distanceGet(location[i], location[i + 1]);
}

ostream &operator<<(ostream &out, const Solution &s)
{
    double myCost = 0;
    if (s.location.size() <= s.in->dimensionGet())
        out << "Warning! Incomplete solution!!!" << endl;
    if (s.costValueTSP < 0)
        out << "Negative distance!!!" << endl;

    out << "Dimension: " << s.in->dimensionGet() << endl;
    out << "Distance: " << setprecision(1) << fixed << s.costValueTSP << endl;
    out << "i = [";
    for (unsigned i = 0; i < s.location.size(); i++)
        out << setw(3) << i << " ";
    out << "]" << endl;
    out << "s = {";
    for (unsigned i = 0; i < s.location.size(); i++)
        if (i < s.location.size() - 1)
        {
            if (s.location[i] <= 7)
                out << "\033[" << 30 + s.location[i] << "m";
            if (8 <= s.location[i] && s.location[i] <= 14)
                out << "\033[" << 83 + s.location[i] << "m";
            out << setw(3) << s.location[i] << ",";
        }
        else
        {
            if (s.location[i] <= 7)
                out << "\033[" << 30 + s.location[i] << "m";
            if (8 <= s.location[i] && s.location[i] <= 14)
                out << "\033[" << 83 + s.location[i] << "m";
            out << setw(3) << s.location[i];
        }
    out << "}" << endl;

    for (unsigned i = 0; i < s.location.size() - 1; i++)
    {
        myCost += s.in->distanceGet(s.location[i], s.location[i + 1]);
    }

    if (abs(myCost - s.costValueTSP) > 0.0001)
        out << "Something wrong...the cost should be: " << myCost << endl;
    return out;
}