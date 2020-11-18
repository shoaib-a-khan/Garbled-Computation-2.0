#include "../include/alice.h"
#include "../include/message.h"
#include <thread>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>

Alice::Alice()
{
    LoadProgramShare();
    LoadDataShare();
    LoadCorrelatedRandoms();
    LoadAlphaRandoms();
    LoadNextRandomPermutation();
    LoadNextShuffleVectors();  
    srv = new Server("Alice");    
}

Alice::~Alice()
{
}

void Alice::LoadProgramShare()
{
    std::ifstream fin("../inputs/alice/P_A.txt");
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

void Alice::LoadDataShare()
{
    std::ifstream fin("../inputs/alice/D_A.txt");
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

void Alice::LoadCorrelatedRandoms()
{
    std::ifstream fin("../randoms/alice/Randoms_A.txt");
    std::istream_iterator<long> start(fin), end;
    std::vector<long> numbers(start, end);
    Randoms.assign(numbers.begin(), numbers.end());
    fin.close();
}

void Alice::LoadAlphaRandoms()
{
    std::ifstream fin("../randoms/alice/AlphaRandoms_A.txt");
    std::istream_iterator<long> start(fin), end;
    std::vector<long> numbers(start, end);
    AlphaRandoms.assign(numbers.begin(), numbers.end());
    fin.close();
}

void Alice::LoadNextRandomPermutation()
{
    static std::ifstream pin("../randoms/alice/Permutations_PA.txt");
    static std::ifstream din("../randoms/alice/Permutations_DA.txt");
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

void Alice::LoadNextShuffleVectors()
{
    static std::ifstream upin("../randoms/alice/U_PA.txt");
    static std::ifstream udin("../randoms/alice/U_DA.txt");
    static std::ifstream rpin("../randoms/alice/R_PA.txt");
    static std::ifstream rdin("../randoms/alice/R_DA.txt");
    static std::ifstream pipin("../randoms/alice/Pi_diff_R_PB_PA.txt");
    static std::ifstream pidin("../randoms/alice/Pi_diff_R_DB_DA.txt");
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
    Pi_P_diff_R_PB_R_PA.clear();
    Pi_D_diff_R_DB_R_DA.clear();
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
    Pi_P_diff_R_PB_R_PA.assign(W.begin(), W.end());
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
    Pi_D_diff_R_DB_R_DA.assign(Z.begin(), Z.end());
}

int Alice::GoLive(int portno)
{
    srv->Init(portno);
    RunGCProtocol();    
}

void Alice::RunGCProtocol()
{
    int count = 0;
    auto start = std::chrono::high_resolution_clock::now();
    while(!termination_condition)
    {
        printf("Alice: pc # %d = %ld\n", count++, pc);
        ObliviousExecute();
        ObliviousShuffle();
        ObliviousTranslate();
    }
    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);   
    printf("Alice: Program Output = %ld\n", output);   
    printf("Execution Time = %ld microseconds\n", duration.count()); 

}

void Alice::ObliviousExecute()
{
    static std::ofstream fout("../debug/alice/Debug_A.txt");
    static int count = 0;
    fout<<"Alice: Oblivious Execute Run # "<<count++<<"\n";
    fout<<"Alice: pc = "<<pc<<"\n";
    /*------- Step 1 -------*/
    Message msg_for_Bob, msg_from_Bob;
    msg_for_Bob.scalar1 = P[pc][0];
    msg_for_Bob.scalar2 = P[pc][1];
    srv->Send(msg_for_Bob);
    fout<<"Alice: Msg sent to Bob : "<<msg_for_Bob.scalar1 << " "<<msg_for_Bob.scalar2<<"\n";
    msg_from_Bob = srv->Receive();
    fout<<"Alice: Msg rcvd from Bob: "<<msg_from_Bob.scalar1<<" "<<msg_from_Bob.scalar2<<"\n";
    /*------- Step 2 -------*/
    int a = P[pc][0] + msg_from_Bob.scalar1;
    int b = P[pc][1] + msg_from_Bob.scalar2;
    fout<<"Alice: a = "<<a<<" b = "<<b<<"\n";
    if(a<0 || b<0||(a==0 && b==0)){
        termination_condition = true;
        return;
    }
    /*------- Step 3 -------*/
    D[a][0] = D[a][0] - D[b][0];
    output = D[a][0];
    /*------- Step 4 -------*/
    long beta = ObliviousComparison(">=", 0, D[a][0]);
    fout<<"Alice: OC output  beta' = "<<beta<<"\n";
    /*------- Step 5 -------*/
    double next_pc = ObliviousMultiplication(beta, P[pc][2]);
    fout<< "Alice: OM output beta'* P'[pc][2] : "<<next_pc<<"\n";
    double temp = ObliviousMultiplication(1-beta, P[pc][3]);
    fout<< "Alice: OM output (1-beta')(P'[pc][3]) : "<<temp<<"\n";
    pc = next_pc+temp;    
}   

long Alice::ObliviousComparison(std::string op, long arg1, long arg2)
{
    long beta;
    Message msg_for_Bob, msg_from_Bob;
    msg_from_Bob = srv->Receive();
    if(op == ">=")
    {
        beta = ((arg1 + msg_from_Bob.scalar1) >= (arg2 + msg_from_Bob.scalar2) ? 1 : 0);
        srand(time(NULL));		
		msg_for_Bob.scalar1 = rand()%100;
        srv->Send(msg_for_Bob);
        beta = beta - msg_for_Bob.scalar1;                
    }
    else if(op == "==")
    {
        beta = ((arg1 + msg_from_Bob.scalar1) == (arg2 + msg_from_Bob.scalar2) ? 1 : 0);
        srand(time(NULL));		
		msg_for_Bob.scalar1 = rand()%100;
        srv->Send(msg_for_Bob);
        beta = beta - msg_for_Bob.scalar1; 
    }
    return beta;
}

long Alice::ObliviousAddition(long arg1, long arg2)
{
    return arg1 + arg2;
}

double Alice::ObliviousMultiplication(long arg1, long arg2)
{
    double result = arg1 * arg2; //Alice locally computes x'y'
    Message msg_for_Bob, msg_from_Bob;
    rc = ((rc + 3) <= Randoms.size() ? rc : 0);
    msg_for_Bob.scalar1 = arg1 - Randoms[rc];     //for computing x'y"
    msg_for_Bob.scalar2 = arg2 - Randoms[rc + 2]; //for computing x"y'
    srv->Send(msg_for_Bob);
    msg_from_Bob = srv->Receive();
    result += ((arg1 + Randoms[rc]) * (msg_from_Bob.scalar1) + Randoms[rc + 1]) /(double)2;
    result += ((arg2 + Randoms[rc + 2]) * (msg_from_Bob.scalar2) + Randoms[rc + 3]) /(double)2;
    rc = (rc+4) % Randoms.size();
    // printf("Alice's Share: %f\n", result);
    return result;
}

void Alice::ObliviousShuffle()
{
    PermuteProgram();
    BlindPermuteProgram();
    PermuteData();
    BlindPermuteData();
    LoadNextRandomPermutation();
    LoadNextShuffleVectors();
}

void Alice::PermuteProgram()
{
    std::vector<std::vector<long>> Pi_R_P(n_p), S(n_p);
    std::vector<Instruction> V(n_p), Temp(n_p);
    V = srv->ReceiveProgramVector();    //receive Bob's masked share of P
    Temp.assign(V.begin(), V.end());
    S.assign(P.begin(), P.end());
    std::cout<<"Permutation Pi_A: ";
    for(int i=0; i < n_p; i++)
    {
        std::cout<<Pi_P[i]<<" ";
        for(int j=0;  j < 5; j++)
        {
            P[Pi_P[i]][j] = S[i][j]; //Applying Pi on Alice's Share of P
            V[Pi_P[i]].val[j] = Temp[i].val[j]; //Applying Pi on Bob's masked Share of P
            Pi_R_P[Pi_P[i]].push_back(R_P[i][j]);  //Prepping shuffle vector needed for resplit
        }
    }
    std::cout<<"\nAlice's Permuted Program P_A:\n ";
    for(int i=0; i < n_p; i++)
    {
        for(int j=0;  j < 5; j++)
        {
            P[i][j] += U_P[i][j];   //resplitting Alice's share of P
            std::cout<<P[i][j]<<" ";
            V[i].val[j] = V[i].val[j] - Pi_R_P[i][j] - U_P[i][j]; //resplitting Bob's share of P
        }
        std::cout<<std::endl;
    }
    srv->SendProgramVector(V, n_p); //sending Bob's his BlindPermuted share of P
}

void Alice::BlindPermuteProgram()
{
    std::vector<Instruction> V(n_p);
    for(int i=0; i<n_p; i++)
        for(int j=0; j<5; j++)
            V[i].val[j] = P[i][j] + R_P[i][j];
    srv->SendProgramVector(V, n_p);
    V = srv->ReceiveProgramVector();
    std::cout<<"Alice's Permuted Program P_A:\n";
    for(int i=0; i<n_p; i++){
        for(int j=0; j<5; j++){
            P[i][j] = V[i].val[j] + Pi_P_diff_R_PB_R_PA[i][j];
            std::cout<<P[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}

void Alice::PermuteData()
{
    std::vector<std::vector<long>> Pi_R_D(n_d), S(n_d);
    std::vector<DataElement> V(n_d), Temp(n_d);
    V = srv->ReceiveDataVector();    //receive Bob's masked share of D
    Temp.assign(V.begin(), V.end());
    S.assign(D.begin(), D.end());
    std::cout<<"Permutation Pi_A: ";
    for(int i=0; i < n_d; i++)
    {
        std::cout<<Pi_D[i]<<" ";
        for(int j=0;  j < 2; j++)
        {
            D[Pi_D[i]][j] = S[i][j]; //Applying Pi on Alice's Share of D
            V[Pi_D[i]].val[j] = Temp[i].val[j]; //Applying Pi on Bob's masked Share of D
            Pi_R_D[Pi_D[i]].push_back(R_D[i][j]);  //Prepping shuffle vector needed for resplit
        }
    }
    std::cout<<"\nAlice's Permuted Data D_A:\n ";
    for(int i=0; i < n_d; i++)
    {
        for(int j=0;  j < 2; j++)
        {
            D[i][j] += U_D[i][j];   //resplitting Alice's share of D
            std::cout<<D[i][j]<<" ";
            V[i].val[j] = V[i].val[j] - Pi_R_D[i][j] - U_D[i][j]; //resplitting Bob's share of D
        }
        std::cout<<std::endl;
    }
    srv->SendDataVector(V, n_p); //sending Bob his BlindPermuted share of D
}

void Alice::BlindPermuteData()
{
    std::vector<DataElement> V(n_d);
    for(int i=0; i<n_d; i++)
        for(int j=0; j<2; j++)
            V[i].val[j] = D[i][j] + R_D[i][j]; //masking Alice's data share
    srv->SendDataVector(V, n_d);    //sending Alice's masked data share to Bob for BlindPermutation
    V = srv->ReceiveDataVector();   //receiving Alice's BlindPermuted share of D
    for(int i=0; i<n_d; i++)
        for(int j=0; j<2; j++)
            D[i][j] = V[i].val[j] + Pi_D_diff_R_DB_R_DA[i][j]; //Adjusting for the resplit
}

void Alice::ObliviousTranslate()
{
    std::vector<std::pair<long,long>> Perm_P = ExtractPermutationShare(P, n_p, 5);
    std::vector<std::pair<long,long>> Perm_D = ExtractPermutationShare(D, n_d, 2);
    ObliviousTranslate(Perm_P, Perm_D);
    pc = ObliviousTranslateSingleAddress(pc, Perm_P);
    Message msg_for_Bob, msg_from_Bob;
    msg_for_Bob.scalar1 = pc;
    srv->Send(msg_for_Bob);
    msg_from_Bob = srv->Receive();
    pc = pc + msg_from_Bob.scalar1;
    UpdateAlphas();  
}

std::vector<std::pair<long,long>>Alice::ExtractPermutationShare(std::vector<std::vector<long>>V, int rows, int cols)
{
    std::vector<std::pair<long,long>> Perm(rows);
    std::pair<long, long> last(-1,-1);
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            Perm[i].first = V[i][cols-1];   //old address
            Perm[i].second = i;             //permuted address   
        }
    }
    Perm.push_back(last);
    return Perm;
}

void Alice::ObliviousTranslate(std::vector<std::pair<long,long>> Perm_P, std::vector<std::pair<long,long>>Perm_D)
{
    static int count=0;
    static std::ofstream fout("../debug/alice/DebugTranslate_A.txt");
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

long Alice::ObliviousTranslateSingleAddress(long addr, std::vector<std::pair<long,long>> Perm)
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

void Alice::UpdateAlphas()
{
    for(int i = 0; i < n_p; i++)
    {
        P[i][4] = i + AlphaRandoms[alpha_rc++];
        alpha_rc = alpha_rc % AlphaRandoms.size();
    }
    for(int i = 0; i < n_d; i++)
    {
        D[i][1] = i + AlphaRandoms[alpha_rc++];
        alpha_rc = alpha_rc % AlphaRandoms.size(); 
    }  
}
