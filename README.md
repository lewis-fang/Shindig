# Shindig
 Author: Lewis F.

Introduction to the Project:
This project is an implementation of inference and training for convolutional neural networks (CNNs) based on C++, without referencing other existing implementations. It is primarily divided into the following parts:

Interactive Interface
1 The interactive interface is implemented using Qt and includes:
	1.1 Parsing Training and Testing Sets
	1.2 Constructing Neural Network Structures
	1.3 Visualizing Loss Changes During Training
	1.4 Saving and Loading Existing Models
These functionalities are mainly implemented in FNN.cpp and its header files. In the future, the development of the interface will focus primarily on the presentation of the training process.

2 Model Construction
This part is mainly responsible for constructing the structure of the convolutional network model and includes:
	2.1 Data Invocation
	2.2 Building Model Interfaces
	2.3 Invoking Model Inference Interfaces
	2.4 Invoking Training Interfaces
	2.5 Querying Model Information Interfaces
	2.6 Parsing Existing Model Interfaces
These functionalities are carried in CNNModel.cpp and its header files.

3 Model Calculation
This part constructs the computational units of the neural network model on a per-layer basis, including convolutional layers and fully connected layers. Each layer consists of input processing, convolution, activation, pooling, and output processing. The computational units are highly modular and can be constructed sequentially. These functionalities are carried in CNNCalc.cpp and its header files.
The above outlines the layered structure of the entire project. Regarding the implementation framework for inference and training of convolutional neural networks, there are three options:
Part 1. Naive Basic Implementation
	This implementation is not highly optimized and uses 8-byte full precision. The priority is to ensure the correctness of the results, serving as a benchmark for the SIMD and CUDA versions.
Part 2. SIMD Instruction Set Version
	This version aims for improved training speed compared to the basic implementation and includes the following methods:
	--> Using SSE, AVX, AVX2, etc., CPU extended instruction sets. AVX512 is not used. If your CPU is powerful enough and has a large cache line, you can use instructions with higher parallelism.
	--> Parallel computation for each layer during training. Note that this parallelization may be counterproductive for extremely small models due to the overhead of creating and destroying CPU threads. However, if some layers have significantly more computation than the thread creation cost, efficiency can be significantly improved.
	--> Using single-precision for inputs, weights, and outputs.

Part 3. CUDA Implementation
Currently, the CUDA implementation only covers inference. The training part is under development.

All the code was completed in its first version in August 2024 and was authored by a single individual. This version is named: FNN5-simd 1.5 - multiThread 2. It is important to note that, due to the code being completed by one person in a relatively short period, errors are inevitable. Any use of this project requires careful attention. In the near future, the author may continue to focus on improving the CUDA version rather than fixing bugs.

Introduction to the Author:
the Author currently works for a medical instruments company, developing various types of algorithms. His responsibilities include:
1 Evaluating Business Requirements and Outputting Algorithm Solutions:
2 Leading algorithm development, parameter establishment, and model deployment.
3 Building Algorithm Development Platforms:
3.1 Data Construction and Database Setup
3.2 Neural Network Training Platform Setup:
	Optimized CPU computation using SIMD instruction sets.
	GPU implementation based on CUDA.
	Design and deployment of model text and binary structures.
3.3 Development and Optimization of Traditional Numerical Algorithms:
	Various iterative fitting algorithms.
	Signal Kalman fusion.
	Algorithms based on specific mathematical models.
3.4 Multi-threaded Platform for Parallel Computation of Millions of Data Points
3.5 Software Development to Support the Above Functionalities

Other Responsibilities:
	Formulating the architecture of product algorithms.
	Establishing technical standards for algorithm development.
	Defining the algorithm development process.

