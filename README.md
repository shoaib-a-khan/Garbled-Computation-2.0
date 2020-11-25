# Garbled-Computation-2.0

The present artifact is a proof-of-concept implementation of our Garbled Computation protocol in Figure 5 (Section 4) of the related paper. Below you will find a quick introduction to the goals that our protocol aims to achieve.

## Introduction
A Garbled Computer is a _universal_ and _secure_ computational model where an adversary observing the execution of a program learns nothing about the code being executed, nor the data it is operating on (whether inputs or intermediate values), nor the outputs it is producing. The model aims to achieve, using a single general approach, the multiple goals of software obfuscation, confidentiality of input data, and confidentiality of intermediate and output data values. It enables execution on untrusted platforms, of trusted and confidential code whose inputs and outputs are sensitive. For example, using GC we can utilize any cloud services provider X without revealing to X the nature of the computation or the data, and without requiring X to change the operation of its cloud services.

## Problem Statement
Alice has confidential software program P, Bob has confidential data D and they want to compute P(D) which is the output of P on input D. However, Alice does not want to reveal P to anyone,and Bob does not want to reveal D to anyone. They both agree beforehand as to which of the computed answers will be revealed to each of them. No other computed value is accessible to either of them. In particular, Alice learns nothing else about D, and Bob learns nothing else about P.


## One Instruction Set Architecture
Our protocol is based on the Turing-complete one instruction set architecture called subble (acronym for subtract and branch on less than or equal). In a subble programming environment, the computer memory space is divided into two segments : code and data. For convenience, we assume that they are addressed separately and they contain different data types. The input data is contained in an integer-type array D of size n_d. The program is contained an array P of size n_p. Each subble instruction is a quadruple (a,b,c,d) s.t 0 <= a,b <= n_d and 0 <= c,d <= n_p. To execute a single subble instruction, the following sequence of steps takes place:
```
1. D[a] := D[a] - D[b]
2. if (D[a] <= 0)
3.    next instruction is P[c]
4. else
5.    next instruction is P[d]
```

## Protocol Details
The details of our Garbled Computation protocol may be found in our related research paper. For a quick overview, the reader may find `Project Presentation.pdf` a helpful guide.

## Overview of Software Prototype
The prototype implements four independent entities:

### Preprocessor:
Simulates the actions of program-owner and data-owner. Preprocessor splits into random additive shares both program `P.txt` and data `D.txt`. It outputs pair `P_A.txt`, `D_A.txt` to `./inputs/alice/` and pair `P_B.txt`, `D_B.txt` to `./inputs/bob/`. In real world, the program P is split by the program owner (or Alice) and the data D is split by data owner (or Bob). All these computations happen in the offline phase.

### Helper: 
Generates correlated randoms for Alice and Bob to be used during the online phase of GC protocol. It takes as input the sizes of program and data, and outputs sets of randoms for Alice in `./randoms/alice/` directory and for Bob in `./randoms/bob/` directory. All these computations happen in the offline phase.

### Alice: 
Simulates all the steps performed by Alice in the online phase of our GC protocol. Reads as input `P_A.txt` and `D_A.txt` from directory `./inputs/alice/`. Outputs her share of the result of computation to her terminal.

### Bob: 
Simulates all the steps performed by Bob in the online phase of our protocol. Reads as input `P_B.txt` and `D_B.txt` in directory `./inputs/bob/`. Outputs his share of the result of computation to his terminal.

## Pre-requisites to Compile
- g++ compiler (v7.4.0)
- [libsodium](https://libsodium.gitbook.io/doc/installation) library.
- CMake v3.10.2 or higher (Optional)

## Compilation
- **Option 1**

On the terminal, navigate to project directory. Enter following sequence of command lines to compile the four executables: 
```
  mkdir build
  cd build
  g++ ../src/preprocessor.cpp ../src/preprocessor_main.cpp -o preprocessor
  g++ ../src/helper.cpp ../src/helper_main.cpp -o helper
  g++ ../src/alice.cpp ../src/alice_main.cpp ../src/server.cpp -o alice -pthread
  g++ ../src/bob.cpp ../src/bob_main.cpp ../src/client.cpp -o bob -pthread
  
```
- **Option 2** (Requires CMake v3.10.2 or higher)

On the terminal, navigate to project directory. Enter following sequence of command lines to compile:
```
  mkdir build
  cd build
  cmake ..
  make
```


## Execution
To see our prototype in action, perform the following sequence of program executions:
```
  ./preprocessor
```
This will ask for a path to program and data to be split. We have provided some example programs in `./Experiments/` directory. To run the fault tree analysis example presented in our paper (the circuit diagram may be accessed at `./Experiments/Circuit/Fault_Tree.png`), enter the following program and data paths:
```
 Please enter Program Path: ../Experiments/Circuit/P.txt
 
 Please enter Data Path: ../Experiments/Circuit/D.txt  
```
Next run the helper to generate randoms:
```
  ./helper
```
This will ask for program and data size. For the privacy-preserving fault tree analysis example, the program size is 16 and the data size is 9.

Now we are all set for the online phase of our GC protocol. Open two terminals and navigate to the project's build directory on both.

On terminal 1, run:
```
  ./alice
```
On terminal 2, run:
```
  ./bob
```
And viola! The magic starts to happen =)

## Output
You can see the exection trace and the eventual output of privacy preserving fault tree analysis on the two terminals. Alice's view on terminal 1 and Bob's view on terminal 2. Adding the final two values output by Alice and Bob will give the actual ouptput of the circuit evaluation. 

## Acknowledgement and Disclaimer
Part of this work was made possible by the NPRP award NPRP X-063-1-014 from the Qatar National Research Fund (a member of The Qatar Foundation). The statements made herein are solely the responsibility of the authors.

This implementation is for proof of concept. There may be areas in the code that were implemented insecurely for the sake of code readibility and understanding. We are not responsible for any damages if the code is used for commercial purposes.
