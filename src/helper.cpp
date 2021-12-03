#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include "../include/helper.h"

Helper::Helper(int n_p, int n_d, int n)
{
    p_size = n_p;
    d_size = n_d;
    count = n;
}

Helper::~Helper()
{

}

void Helper::Init()
{
    GenerateCorrelatedRandoms(p_size*d_size*count);
    GenerateAlphaRandoms(p_size*d_size*count);
    GenerateRandomPermutations(p_size, d_size, count);
    GenerateShuffleVectors(p_size, d_size, count);
    GeneratePermutedVectors(p_size, d_size, count);
}

void Helper::GenerateCorrelatedRandoms(int n)
{
    std::vector<long> R1(n);
    std::vector<long> R2(n);
    std::vector<long> R(n);
    std::ofstream fout;
    time_t t;
    srand((unsigned)time(&t));

    for (int i = 0; i < n; i++)
    {
        R1[i] = rand()%10000;
        R2[i] = rand()%10000;
        R[i] = rand()%10000;
    }
    fout.open("../randoms/alice/Randoms_A.txt");
    for (int i = 0; i < n; i++)
    {
        fout << R1[i] << " ";
        fout << R1[i] * R2[i] + R[i] << "\n";
    }
    fout.flush();
    fout.close();
    fout.open("../randoms/bob/Randoms_B.txt");
    for (int i = 0; i < n; i++)
    {
        fout << R2[i] << " ";
        fout << R1[i] * R2[i] - R[i] << "\n";
    }
    fout.flush();
    fout.close();
}

void Helper::GenerateAlphaRandoms(int n)
{
    std::ofstream fa, fb;
    std::vector<long> R(n);
    time_t t;
    fa.open("../randoms/alice/AlphaRandoms_A.txt");
    fb.open("../randoms/bob/AlphaRandoms_B.txt");
    srand((unsigned)time(&t));
    for (int i = 0; i < n; i++)
        R[i] = rand() % 10000;
    for (int i = 0; i < n; i++)
    {
        fa << R[i] << " ";
        fb << -R[i] << " ";
    }
    fa.flush();
    fa.close();
    fb.flush();
    fb.close();
}

void Helper::GenerateRandomPermutations(int n_p, int n_d, int count)
{
    std::vector<int> pi_p(n_p, 0), flags_p(n_p, 0);
    std::vector<int> pi_d(n_d, 0), flags_d(n_d, 0);
    std::ofstream output_file_pa("../randoms/alice/Permutations_PA.txt");
    std::ostream_iterator<int> output_iterator_pa(output_file_pa, " ");
    std::ofstream output_file_da("../randoms/alice/Permutations_DA.txt");
    std::ostream_iterator<int> output_iterator_da(output_file_da, " ");
    std::ofstream output_file_pb("../randoms/bob/Permutations_PB.txt");
    std::ostream_iterator<int> output_iterator_pb(output_file_pb, " ");
    std::ofstream output_file_db("../randoms/bob/Permutations_DB.txt");
    std::ostream_iterator<int> output_iterator_db(output_file_db, " ");
    time_t t;
    srand((unsigned)time(&t));

    for (int j = 0; j < 2 * count; j++)
    {
        for (int i = 0, r = 0; i < n_p;)
        {
            r = rand() % n_p;
            if (flags_p[r] == 0)
            {
                flags_p[r] = 1;
                pi_p[i] = r;
                // std::cout << r << " ";
                i++;
            }
        }
        // std::cout << "\n";
        for (int i = 0, r = 0; i < n_d;)
        {
            r = rand() % n_d;
            if (flags_d[r] == 0)
            {
                flags_d[r] = 1;
                pi_d[i] = r;
                // std::cout << r << " ";
                i++;
            }
        }
        // std::cout << "\n";
        if (j % 2 == 0)
        {
            std::copy(pi_p.begin(), pi_p.end(), output_iterator_pa);
            std::copy(pi_d.begin(), pi_d.end(), output_iterator_da);
            output_file_pa << std::endl;
            output_file_da << std::endl;
            output_file_pa.flush();
            output_file_da.flush();
        }
        else
        {
            std::copy(pi_p.begin(), pi_p.end(), output_iterator_pb);
            std::copy(pi_d.begin(), pi_d.end(), output_iterator_db);
            output_file_pb << std::endl;
            output_file_db << std::endl;
            output_file_pb.flush();
            output_file_db.flush();
        }
        std::fill(flags_p.begin(), flags_p.end(), 0);
        std::fill(flags_d.begin(), flags_d.end(), 0);
    }
    output_file_pa.close();
    output_file_da.close();
    output_file_pb.close();
    output_file_db.close();
}

void Helper::GenerateShuffleVectors(int n_p, int n_d, int count)
{
    std::ofstream output_file;
    std::vector<std::string> file_name = {"../randoms/alice/U_PA.txt", "../randoms/bob/U_PB.txt", "../randoms/alice/R_PA.txt", "../randoms/bob/R_PB.txt",
                                          "../randoms/alice/U_DA.txt", "../randoms/bob/U_DB.txt", "../randoms/alice/R_DA.txt", "../randoms/bob/R_DB.txt"};
    time_t t;
    srand((unsigned)time(&t));

    for (int i = 0; i < 8; i++)
    {
        output_file.open(file_name[i]);
        int n = (i < 4 ? n_p : n_d);
        int m = (i < 4 ? 5 : 2);
        int l = count;
        while (l--)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < m; k++)
                    output_file << rand() %10000 << " ";
                output_file << std::endl;
            }
            output_file << std::endl;
        }
        output_file.flush();
        output_file.close();
    }
}

void Helper::GeneratePermutedVectors(int n_p, int n_d, int count)
{
    std::ofstream output_file;
    std::ifstream fin1, fin2;
    std::vector<std::string> file_name = {"../randoms/bob/Pi_diff_R_PA_PB.txt", "../randoms/alice/Pi_diff_R_PB_PA.txt",
                                          "../randoms/bob/Pi_diff_R_DA_DB.txt", "../randoms/alice/Pi_diff_R_DB_DA.txt",
                                          "../randoms/alice/R_PA.txt", "../randoms/bob/R_PB.txt", "../randoms/alice/R_DA.txt", "../randoms/bob/R_DB.txt"};
    for (int i = 0; i < 4; i++)
    {
        output_file.open(file_name[i]);
        int n = (i < 2 ? n_p : n_d);
        int m = (i < 2 ? 5 : 2);
        switch (i)
        {
        case 0:
            fin1.open(file_name[4]);
            fin2.open(file_name[5]);
            break;
        case 1:
            fin1.open(file_name[5]);
            fin2.open(file_name[4]);
            break;
        case 2:
            fin1.open(file_name[6]);
            fin2.open(file_name[7]);
            break;
        case 3:
            fin1.open(file_name[7]);
            fin2.open(file_name[6]);
            break;
        default:
            break;
        }

        int l = count;
        long temp1, temp2;
        while (l--)
        {
            std::vector<int> Pi = LoadNextRandomPermutation(i, n);
            std::vector<std::vector<long>> R(n),temp(n);
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < m; k++)
                {
                    fin1 >> temp1;
                    fin2 >> temp2;
                    temp[j].push_back(temp1-temp2);
                }
            }
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < m; k++)
                    R[Pi[j]].push_back(temp[j][k]);
                    
            }
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < m; k++)
                {
                   output_file<<R[j][k]<<" "; 
                }
                output_file<<std::endl;
            }
            output_file<<std::endl;
        }
        fin1.close();
        fin2.close();
        output_file.flush();
        output_file.close();
    }
}

std::vector<int> Helper::LoadNextRandomPermutation(int file_no, int size)
{
    static std::ifstream pin_a("../randoms/alice/Permutations_PA.txt");
    static std::ifstream pin_b("../randoms/bob/Permutations_PB.txt");
    static std::ifstream din_a("../randoms/alice/Permutations_DA.txt");
    static std::ifstream din_b("../randoms/bob/Permutations_DB.txt");
    std::vector<int> Pi;
    long temp;
    for (int i = 0; i < size; i++)
    {
        switch (file_no)
        {
        case 0:
            pin_a >> temp;
            break;
        case 1:
            pin_b >> temp;
            break;
        case 2:
            din_a >> temp;
            break;
        case 3:
            din_b >> temp;
            break;
        default:
            break;
        }
        Pi.push_back(temp);
    }
    return Pi;
}

