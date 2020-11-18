# Garbled-Computation-2.0
A software prototype for a Garbled Computer as proposed in my PhD research

##Introduction
A Garbled Computer is auniversal and secure computational model where an adversary observing the execution of a program learns nothing about the code being executed, nor the data it is operating on (whether inputs or intermediate values), nor the outputs it is producing. The model aims to achieve, using a single general approach, the multiple goals of software obfuscation, confidentiality of input data, and confidentiality of intermediate and output data values. It enables execution on untrusted platforms, of trustedand confidential code whose inputs and outputs are sensitive. For example, using GC we can utilize Amazon cloud services without revealing to Amazon the nature of the computation or the data,and without requiring Amazon to change the operation of its cloud services.
