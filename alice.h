#ifndef ALICE_H
#define ALICE_H
#include "party.h"
#include "server.h"
#include <vector>


class Alice : public Party
{
private:
    Server *srv;
    std::vector<std::vector<long>> Pi_P_diff_R_PB_R_PA;
    std::vector<std::vector<long>> Pi_D_diff_R_DB_R_DA;

    
    
public:
    Alice();
    ~Alice();
    int GoLive(int portno);
    void LoadProgramShare();
    void LoadDataShare();
    void LoadCorrelatedRandoms();
    void LoadAlphaRandoms();
    void LoadNextRandomPermutation();
    void LoadNextShuffleVectors();
    void RunGCProtocol();
    void ObliviousExecute();
    void ObliviousShuffle();
    void ObliviousTranslate();
    void ObliviousTranslate(std::vector<std::pair<long,long>>Perm_P, std::vector<std::pair<long,long>>Perm_D);
    long ObliviousTranslateSingleAddress(long addr, std::vector<std::pair<long,long>>Perm);
    void UpdateAlphas();
    void PermuteProgram();
    void BlindPermuteProgram();
    void PermuteData();
    void BlindPermuteData();
    long ObliviousAddition(long arg1, long arg2);
    double ObliviousMultiplication(long arg1, long arg2);
    long ObliviousComparison(std::string op, long arg1, long arg2);
    std::vector<std::pair<long,long>> ExtractPermutationShare(std::vector<std::vector<long>>V, int rows, int cols);

};

#endif