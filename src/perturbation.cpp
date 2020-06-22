#include "perturbation.h"
#include "solution.h"
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

Perturbation::Perturbation(Input *input)
{
    this->in = input;
    localSolution = new Solution(in);
}

Solution Perturbation::bridgePerturbation(const Solution *s, int bridge)
{

    vector<pair<int, int>> section(makeBridges(s, bridge));
    int beginning;
    int end;
    int position = 0;

    for (int k = section.size() - 1; k >= 0; k--)
    {
        beginning = section[k].first;
        end = section[k].second;

        if (beginning == end)
        {
            cout << "Error on " << __FILE__ << " line " << __LINE__ << endl;
            exit(0);
        }
        else if (beginning > end)
        {
            while (beginning != end + 1)
            {
                localSolution->location[position++] = s->location[beginning];
                beginning = (beginning + 1) % (s->location.size() - 1);
            }
        }
        else
        {
            while (beginning <= end)
            {
                localSolution->location[position++] = s->location[beginning++];
            }
        }
    }
    localSolution->location[s->location.size() - 1] = localSolution->location[0];

    localSolution->computeCostValueTSP();

    return (*localSolution);
}

vector<pair<int, int>> Perturbation::makeBridges(const Solution *s, int bridges)
{

    int minimum = 2;
    // int maximum = max(minimum, int(s->location.size()-1)/10);
    vector<pair<int, int>> section;
    bool selected;
    for (int i = 0; i < bridges; i++)
    {
        pair<int, int> myPosition(0, 0);
        do
        {
            selected = true;
            myPosition.second = rand() % (s->location.size() - 1);
            for (unsigned j = 0; j < section.size() && selected; j++)
            {
                if (myPosition == section[j])
                    selected = false;
                //Guarantee minimum size for all subsequences
                if (abs(myPosition.second - section[j].second) < 2)
                    selected = false;
                if (section[j].second == 0 && myPosition.second >= int(s->location.size()) - minimum)
                    selected = false;
                if (myPosition.second == 0 && section[j].second >= int(s->location.size()) - minimum)
                    selected = false;
            }
        } while (!selected);
        section.push_back(myPosition);
        // Sorting...
        for (int k = section.size() - 1; k > 0; --k)
            if (section[k].second < section[k - 1].second)
            {
                swap(section[k], section[k - 1]);
            }
    }
    // Fixing first pair
    section[0].first = (section[section.size() - 1].second + 1) % (s->location.size() - 1);
    ;
    for (int k = section.size() - 1; k > 0; --k)
        section[k].first = (section[k - 1].second + 1) % (s->location.size() - 1);
    return section;
}