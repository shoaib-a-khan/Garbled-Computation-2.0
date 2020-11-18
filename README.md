# Garbled-Computation-2.0

## Introduction
A Garbled Computer is auniversal and secure computational model where an adversary observing the execution of a program learns nothing about the code being executed, nor the data it is operating on (whether inputs or intermediate values), nor the outputs it is producing. The model aims to achieve, using a single general approach, the multiple goals of software obfuscation, confidentiality of input data, and confidentiality of intermediate and output data values. It enables execution on untrusted platforms, of trustedand confidential code whose inputs and outputs are sensitive. For example, using GC we can utilize Amazon cloud services without revealing to Amazon the nature of the computation or the data,and without requiring Amazon to change the operation of its cloud services.

## Problem Statement
Alice has confidential software program P, Bob has confidential data D and they want to compute P(D) which is the output of P on input D. However, Alice does not want to reveal P to anyone,and Bob does not want to reveal D to anyone. They both agree beforehand as to which of the computed answers will be revealed to each of them. No other computed value is accessible to either of them. In particular, Alice learns nothing else about D, and Bob learns nothing else about P.


## One Instruction Set Architecture
Our protocol is based on the Turing-complete single instruction set architecture called subble (acronym for subtract and branch on less than or equal). In a subble programming environment, the computer memory space is divided into two segments : code and data. For convenience, we assume that they are addressed separately and they contain different data types. The input data is contained in an integer-type array D of size n_d. The program is contained an array P of size n_p. Each subble instruction is a quadruple (a,b,c,d) s.t 0 <= a,b <= n_d and 0 <= c,d <= n_p. To execute a single subble instruction, the following sequence of steps takes place:
```
1. D[a] := D[a] - D[b]
2. if (D[a] <= 0)
3.    next instruction is P[c]
4. else
5.    next instruction is P[d]
```
