#ifndef BOB_H
#define BOB_H
#include "party.h"
#include "client.h"

class Bob:public Party
{
private:
    Client *cli;
    std::vector<std::vector<long>> Pi_P_diff_R_PA_R_PB;
    std::vector<std::vector<long>> Pi_D_diff_R_DA_R_DB;


public:
    Bob();
    ~Bob();
    void GoLive(char *ip, int portno);
    void LoadProgramShare();
    void LoadDataShare();
    void LoadCorrelatedRandoms();
    void LoadAlphaRandoms();
    void LoadNextRandomPermutation();
    void LoadNextShuffleVectors();
    void RunGCProtocol();
    void ObliviousExecute();
    void ObliviousShuffle();
    void PermuteProgram();
    void BlindPermuteProgram();
    void PermuteData();
    void BlindPermuteData();
    void ObliviousTranslate();
    void ObliviousTranslate(std::vector<std::pair<long,long>>Perm_P, std::vector<std::pair<long,long>>Perm_D);
    long ObliviousTranslateSingleAddress(long addr, std::vector<std::pair<long,long>>Perm);
    void UpdateAlphas();
    long ObliviousAddition(long arg1, long arg2);
    double ObliviousMultiplication(long arg1, long arg2);
    long ObliviousComparison(std::string op, long arg1, long arg2);
    std::vector<std::pair<long,long>> ExtractPermutationShare(std::vector<std::vector<long>>V, int rows, int cols);


};

#endif