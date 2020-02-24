#ifndef HLEPER_H
#define HELPER_H
#include<vector>

class Helper
{
    private:
    int p_size = 0;
    int d_size = 0;
    int count = 0;
    
    public:
    Helper(int n_p, int n_d, int n);
    ~Helper();
    void Init();
    void GenerateCorrelatedRandoms(int n);
    void GenerateAlphaRandoms(int n);
    void GenerateRandomPermutations(int n_p, int n_d, int count);
    void GenerateShuffleVectors(int n_p, int n_d, int count);
    void GeneratePermutedVectors(int n_p, int n_d, int count);
    std::vector<int> LoadNextRandomPermutation(int file_no, int size);

};



#endif