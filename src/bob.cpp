#include "../include/bob.h"
#include "../include/instruction.h"
#include <thread>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>


Bob::Bob()
{
    LoadProgramShare();
    LoadDataShare();
    LoadCorrelatedRandoms();
    LoadAlphaRandoms();
    LoadNextRandomPermutation();
    LoadNextShuffleVectors();  
    cli = new Client("Bob");	
}

Bob::~Bob()
{

}

void Bob::LoadProgramShare()
{
    std::ifstream fin("../inputs/bob/P_B.txt");
    std::vector<std::vector<long>> R;
    long temp;
    fin>>temp;
    while(!fin.eof())
    {
       std::vector<long> Instruction;
        for (int j = 0; j < 5; j++)
        {
            Instruction.push_back(temp);
            fin >> temp;            
        }
        R.push_back(Instruction);
    }
    P.assign(R.begin(), R.end());
    n_p = P.size();
    fin.close();    
}

void Bob::LoadDataShare()
{
    std::ifstream fin("../inputs/bob/D_B.txt");
    std::vector<std::vector<long>> R;
    long temp;
    fin>>temp;
    while(!fin.eof())
    {
        std::vector<long> Element;
        for (int j = 0; j < 2; j++)
        {
            Element.push_back(temp);
            fin >> temp;
        }
        R.push_back(Element);
    }
    D.assign(R.begin(), R.end());
    n_d = D.size();
    fin.close();
}


void Bob::LoadCorrelatedRandoms()
{
    std::ifstream fin("../randoms/bob/Randoms_B.txt");
    std::istream_iterator<long> start(fin), end;
    std::vector<long> numbers(start, end);
    Randoms.assign(numbers.begin(), numbers.end());
    fin.close();
}

void Bob::LoadAlphaRandoms()
{
    std::ifstream fin("../randoms/bob/AlphaRandoms_B.txt");
    std::istream_iterator<long> start(fin), end;
    std::vector<long> numbers(start, end);
    AlphaRandoms.assign(numbers.begin(), numbers.end());
    fin.close();
}

void Bob::LoadNextRandomPermutation()
{
    static std::ifstream pin("../randoms/bob/Permutations_PB.txt");
    static std::ifstream din("../randoms/bob/Permutations_DB.txt");
    long temp;
    Pi_P.clear();
    Pi_D.clear();
    for(int i=0; i < n_p; i++)
    {
        pin>>temp;
        Pi_P.push_back(temp);
    }
    for(int j=0; j < n_d; j++)
    {
        din>>temp;
        Pi_D.push_back(temp);
    }
}

void Bob::LoadNextShuffleVectors()
{
    static std::ifstream upin("../randoms/bob/U_PB.txt");
    static std::ifstream udin("../randoms/bob/U_DB.txt");
    static std::ifstream rpin("../randoms/bob/R_PB.txt");
    static std::ifstream rdin("../randoms/bob/R_DB.txt");
    static std::ifstream pipin("../randoms/bob/Pi_diff_R_PA_PB.txt");
    static std::ifstream pidin("../randoms/bob/Pi_diff_R_DA_DB.txt");
    std::vector<std::vector<long>> U(n_p);
    std::vector<std::vector<long>> V(n_p);
    std::vector<std::vector<long>> W(n_p);
    std::vector<std::vector<long>> X(n_d);
    std::vector<std::vector<long>> Y(n_d);
    std::vector<std::vector<long>> Z(n_d);
    long temp;
    U_P.clear();
    U_D.clear();
    R_P.clear();
    R_D.clear();
    Pi_P_diff_R_PA_R_PB.clear();
    Pi_D_diff_R_DA_R_DB.clear();
    for (int i = 0; i < n_p; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            upin >> temp;
            U[i].push_back(temp);
            rpin >> temp;
            V[i].push_back(temp);
            pipin >> temp;
            W[i].push_back(temp);
        }
    }
    U_P.assign(U.begin(), U.end());
    R_P.assign(V.begin(), V.end());
    Pi_P_diff_R_PA_R_PB.assign(W.begin(), W.end());
    for (int i = 0; i < n_d; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            udin >> temp;
            X[i].push_back(temp);
            rdin >> temp;
            Y[i].push_back(temp);
            pidin >> temp;
            Z[i].push_back(temp);
        }
    }
    U_D.assign(X.begin(), X.end());
    R_D.assign(Y.begin(), Y.end());
    Pi_D_diff_R_DA_R_DB.assign(Z.begin(), Z.end());    
}

void Bob::GoLive(char* ip, int portno)
{
    cli->Init(ip, portno);
    RunGCProtocol();    
}

void Bob::RunGCProtocol()
{
    int count = 0;
    while(!termination_condition)
    {
        printf("Bob: pc # %d = %ld\n", count++, pc);
        ObliviousExecute();
        ObliviousShuffle();
        ObliviousTranslate();
    }
    printf("Bob: Program Output = %ld\n", output);
}

void Bob::ObliviousExecute()
{
    static std::ofstream fout("../debug/bob/Debug_B.txt");
    static int count = 0;
    fout<<"Bob: Oblivious Execute Run # "<<count++<<"\n";
    fout<<"Bob: pc = "<<pc<<"\n";
    /*------- Step 1 -------*/
    Message msg_for_Alice, msg_from_Alice;
    msg_for_Alice.scalar1 = P[pc][0];
    msg_for_Alice.scalar2 = P[pc][1];
    msg_from_Alice = cli->Receive();
    fout<<"Bob: Msg rcvd from Alice : "<<msg_from_Alice.scalar1 << " "<<msg_from_Alice.scalar2<<"\n";
    cli->Send(msg_for_Alice);
    fout<<"Bob: Msg sent to Alice : "<<msg_for_Alice.scalar1 << " "<<msg_for_Alice.scalar2<<"\n";
    /*------- Step 2 -------*/
    int a = P[pc][0] + msg_from_Alice.scalar1;
    int b = P[pc][1] + msg_from_Alice.scalar2;
    fout<<"Bob: a = "<<a<<" b = "<<b<<"\n";
    if(a<0 || b<0 ||(a==0 && b==0)){
        termination_condition =  true;
        return;
    }        
    /*------- Step 3 -------*/
    D[a][0] = D[a][0] - D[b][0];
    output = D[a][0];
    /*------- Step 4 -------*/
    long beta = ObliviousComparison(">=", 0, D[a][0]);
    fout<<"Bob: OC output  beta\" = "<<beta<<"\n";
    /*------- Step 5 -------*/
    double next_pc = ObliviousMultiplication(beta, P[pc][2]);
    fout<< "Bob: OM output beta\"* P\"[pc][2] : "<<next_pc<<"\n";
    double temp = ObliviousMultiplication(-beta, P[pc][3]);
    fout<< "Bob: OM output (-beta\")(P\"[pc][3]) : "<<temp<<"\n";
    pc = next_pc + temp;    
}   

long Bob::ObliviousComparison(std::string op, long arg1, long arg2)
{
    long beta;
    Message msg_for_Alice, msg_from_Alice;
    msg_for_Alice.scalar1 = arg1;
    msg_for_Alice.scalar2 = arg2;
    cli->Send(msg_for_Alice);
    msg_from_Alice = cli->Receive();
    return msg_from_Alice.scalar1;
}

long Bob::ObliviousAddition(long arg1, long arg2){
    return arg1 + arg2;
}

double Bob::ObliviousMultiplication(long arg1, long arg2)
{
    double result = arg1 * arg2; //Bob locally computes x"y"
    Message msg_for_Alice,  msg_from_Alice;
    rc = ((rc + 3) <= Randoms.size() ? rc : 0);
    msg_for_Alice.scalar1 = arg2 - Randoms[rc];   //for computing x'y"
    msg_for_Alice.scalar2 = arg1 - Randoms[rc + 2]; //for computing x"y'
    msg_from_Alice = cli->Receive();
    cli->Send(msg_for_Alice);
    result += ((arg2 + Randoms[rc])*(msg_from_Alice.scalar1)+ Randoms[rc+1])/(double)2;
    result += ((arg1 + Randoms[rc+2])*(msg_from_Alice.scalar2) + Randoms[rc+3])/(double)2;
    rc = (rc+4) % Randoms.size();
    // printf("Bob's Share: %f\n", result);
    return result;
}

void Bob::ObliviousShuffle()
{
    BlindPermuteProgram();
    PermuteProgram();
    BlindPermuteData();
    PermuteData();
    LoadNextRandomPermutation();
    LoadNextShuffleVectors();
}

void Bob::BlindPermuteProgram()
{
    std::vector<Instruction> V(n_p);
    for(int i=0; i<n_p; i++)
        for(int j=0; j<5; j++)
            V[i].val[j] = P[i][j] + R_P[i][j];
    cli->SendProgramVector(V, n_p);
    V = cli->ReceiveProgramVector();
    std::cout<<"Bob's Permuted Program P_B:\n";
    for(int i=0; i<n_p; i++){
        for(int j=0; j<5; j++){
            P[i][j] = V[i].val[j] + Pi_P_diff_R_PA_R_PB[i][j];
            std::cout<<P[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}

void Bob::PermuteProgram()
{
    std::vector<std::vector<long>> Pi_R_P(n_p), S(n_p);
    std::vector<Instruction> V(n_p), Temp(n_p);
    V = cli->ReceiveProgramVector();    //receive Alice's masked share of P
    Temp.assign(V.begin(), V.end());
    S.assign(P.begin(), P.end());
    std::cout<<"Permutation Pi_B: ";
    for(int i=0; i < n_p; i++)
    {
        std::cout<<Pi_P[i]<<" ";
        for(int j=0;  j < 5; j++)
        {
            P[Pi_P[i]][j] = S[i][j]; //Applying Pi on Bob's Share of P
            V[Pi_P[i]].val[j] = Temp[i].val[j]; //Applying Pi on Alice's masked Share of P
            Pi_R_P[Pi_P[i]].push_back(R_P[i][j]);  //Prepping shuffle vector needed for resplit
        }
    }
    std::cout<<"\nBob's Permuted Program P_B:\n ";
    for(int i=0; i < n_p; i++)
    {
        for(int j=0;  j < 5; j++)
        {
            P[i][j] += U_P[i][j];   //resplitting Bob's share of P
            std::cout<<P[i][j]<<" ";
            V[i].val[j] = V[i].val[j] - Pi_R_P[i][j] - U_P[i][j]; //resplitting Alice's share of P
        }
        std::cout<<std::endl;
    }
    cli->SendProgramVector(V, n_p); //sending Alice's his BlindPermuted share of P
}

void Bob::BlindPermuteData()
{
    std::vector<DataElement> V(n_d);
    for(int i=0; i<n_d; i++)
        for(int j=0; j<2; j++)
            V[i].val[j] = D[i][j] + R_D[i][j];
    cli->SendDataVector(V, n_d);
    V = cli->ReceiveDataVector();
    std::cout<<"Bob's Permuted Data D_B:\n";
    for(int i=0; i<n_d; i++){
        for(int j=0; j<2; j++){
            D[i][j] = V[i].val[j] + Pi_D_diff_R_DA_R_DB[i][j];
            std::cout<<D[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}

void Bob::PermuteData()
{
    std::vector<std::vector<long>> Pi_R_D(n_d), S(n_d);
    std::vector<DataElement> V(n_d), Temp(n_d);
    V = cli->ReceiveDataVector();    //receive Alice's masked share of D
    Temp.assign(V.begin(), V.end());
    S.assign(D.begin(), D.end());
    for(int i=0; i < n_d; i++)
    {
        for(int j=0;  j < 2; j++)
        {
            D[Pi_D[i]][j] = S[i][j]; //Applying Pi on Bob's Share of D
            V[Pi_D[i]].val[j] = Temp[i].val[j]; //Applying Pi on Alice's masked Share of D
            Pi_R_D[Pi_D[i]].push_back(R_D[i][j]);  //Prepping shuffle vector needed for resplit
        }
    }
    for(int i=0; i < n_d; i++)
    {
        for(int j=0;  j < 2; j++)
        {
            D[i][j] += U_D[i][j];   //resplitting Bob's share of D
            V[i].val[j] = V[i].val[j] - Pi_R_D[i][j] - U_D[i][j]; //resplitting Alice's share of D
        }
    }
    cli->SendDataVector(V, n_p); //sending Alice her BlindPermuted share of D
}

void Bob::ObliviousTranslate()
{
    std::vector<std::pair<long,long>> Perm_P = ExtractPermutationShare(P, n_p, 5);
    std::vector<std::pair<long,long>> Perm_D = ExtractPermutationShare(D, n_d, 2);
    ObliviousTranslate(Perm_P, Perm_D);
    pc = ObliviousTranslateSingleAddress(pc, Perm_P);
    Message msg_for_Alice, msg_from_Alice;
    msg_for_Alice.scalar1 = pc;
    msg_from_Alice = cli->Receive();
    cli->Send(msg_for_Alice);
    pc = pc + msg_from_Alice.scalar1;  
    UpdateAlphas();  
}

std::vector<std::pair<long,long>>Bob::ExtractPermutationShare(std::vector<std::vector<long>>V, int rows, int cols)
{
    std::vector<std::pair<long,long>> Perm(rows);
    std::pair<long, long> last(0,0);
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            Perm[i].first = V[i][cols-1];   //old address
            Perm[i].second = 0;             //permuted address   
        }
    }
    Perm.push_back(last);
    return Perm;
}

void Bob::ObliviousTranslate(std::vector<std::pair<long,long>> Perm_P, std::vector<std::pair<long,long>>Perm_D)
{
    static int count=0;
    static std::ofstream fout("../debug/bob/DebugTranslate_B.txt");
    for(int i = 0; i < n_p; i++)
    {
        P[i][0] = ObliviousTranslateSingleAddress(P[i][0], Perm_D);
        P[i][1] = ObliviousTranslateSingleAddress(P[i][1], Perm_D);
        P[i][2] = ObliviousTranslateSingleAddress(P[i][2], Perm_P);
        P[i][3] = ObliviousTranslateSingleAddress(P[i][3], Perm_P);
    }
    fout<<"Translation # "<<count++<<"\n";
    for(int i=0; i<n_p; i++)
    {
        for(int j=0; j<5; j++)
            fout<<P[i][j]<<" ";
        fout<<"\n";
    }
}

long Bob::ObliviousTranslateSingleAddress(long addr, std::vector<std::pair<long,long>> Perm)
{
   double result = 0;
   long beta = 0;
   for(int i=0; i < Perm.size(); i++)
   {
       beta = ObliviousComparison("==", addr, Perm[i].first);
       result += ObliviousMultiplication(beta, Perm[i].second);
   } 
   return result;
}

void Bob::UpdateAlphas()
{
    for(int i = 0; i < n_p; i++)
    {
        P[i][4] = AlphaRandoms[alpha_rc++];
        alpha_rc = alpha_rc % AlphaRandoms.size();
    }    
    for(int i = 0; i < n_d; i++)
    {
        D[i][1] = AlphaRandoms[alpha_rc++];
        alpha_rc = alpha_rc % AlphaRandoms.size();
    }   
}
