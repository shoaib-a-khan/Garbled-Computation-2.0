#ifndef PARTY_H
#define PARTY_H
#include "server.h"
#include "client.h"
#include <vector>

class Party
{
protected:
    int n_p = 0;
    int n_d = 0;
    int rc = 0;
    int alpha_rc = 0;
    long pc = 0;
    long output = 0;
    bool termination_condition = false;
    bool trace = false;
    std::vector<std::vector<long>> P;
    std::vector<std::vector<long>> D;
    std::vector<long> Randoms;
    std::vector<long> AlphaRandoms;
    std::vector<int> Pi_P;
    std::vector<int> Pi_D;
    std::vector<std::vector<long>> R_P;
    std::vector<std::vector<long>> R_D;
    std::vector<std::vector<long>> U_P;
    std::vector<std::vector<long>> U_D;

public:
    bool ObliviousExecute();
    bool ObliviousShuffle();
    bool ObliviousTranslate();
};

#endif