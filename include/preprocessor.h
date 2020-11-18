#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include<string>

#define S_PARAM 1000

class Preprocessor
{
    private:
    std::string p_path, p_A_path, p_B_path;
    std::string d_path, d_A_path, d_B_path;
    
    public:
    Preprocessor(std::string prog_path, std::string data_path);
    ~Preprocessor();
    void Init();    
};


#endif


