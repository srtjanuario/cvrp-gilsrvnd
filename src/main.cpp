#include "input.h"
#include "solution.h"
#include "construction.h"
#include "neighborhood.h"
#include "perturbation.h"
#include "localsearch.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>
#include <list>
#include <iomanip>
#include <stack>
#include <climits>
#include <queue>
using namespace std;

int main(int argc, char **argv)
{
	int seed = time(0);
	srand(seed);

	Input in(argc, argv);
	Solution sol(&in);
	LocalSearch ls(&in);

	int Imax = 50;
	int Iils = (in.nLocation() >= 150) ? in.nLocation() / 2 : in.nLocation();
	vector<double> R;
	R.push_back(0.00);
	for (int i = 1; i <= 25; i++)
		R.push_back(R[i - 1] + 0.01);

	sol = ls.GILSRVND(Imax, Iils, R);

	int cd = 300; // segundos

	cout<<sol<<endl;

	return 0;
}
