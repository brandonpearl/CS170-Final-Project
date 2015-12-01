# CS170-Final-Project
Max acyclic subgraph.

Instructions for Instance Generator:

    Build the executable by typing 'make'

    Program takes three command line args:
    1: Node count; integer from 0 to 100
    2: Edge probability; integer from 0 to 100 representing percentage change that  
      a given edge exists
    3: Destination filename; filename must not already exist. Should include
      extenstion, for example: ex.in

    An example usage would be
    ./instance_generator 100 33 test.in
=======


Algorithms Implemented - Josh

Note that calling my solve_instance will run all of these algorithms by itself.

1. Randomly shuffle x number of times and chosse the best version
2. Swap adjacent blocks of size 1,2,4,...,log(n) if the swap leads to improvement
3. Greedy algorithm:
      For every choice of start vertex:
           choose the next vertex that leads to the best score on the current size solution.
      Ie. If we have 6 nodes, we first choose 1 as our start, then figure out which
      of nodes 2,3,4,5,6 to pick as the second, based on which leads to the
      best score for the size (2) solution.
      Repeat until all nodes have been picked, then iterate to the next possible start vertex.
      We will have n possible solutions (can choose each of the n vertices as the start),
      and so we return the one that has the best score
4. Sort by indegree ascdending, and outdegree descending, and retain if they lead to
    improvement.
5. Break the problem into subgroups of size at most X. Then do a brute force on each
of these subgroups (X is probably <= 10). Then, figure out the best way to combine
these subgroups.


Algorithms Implemented - Brandon

Like Josh's, my solve_instance will pick the best result of all algorithms within the .cpp file.

1. Monte Carlo-style swapping: swap individual indices x times, but only perform the swap if the score improves.
2. Frugal dynamic programming routine which performs a DP step on subproblems of size subSize or below.
3. 


Algorithms Implemented - Matt

1. Random and Reverse: Generate a random ordering and its reverse. Use the one with
   highest score
2. Greedy Edges Forward: Order the nodes from most outgoing edges to least
3. Greedy Edges Backwards: Order the nodes from least incoming edges to most
4. Pure Random: Generate 1,000 random orders and return the max scoring one
5. Pair Swap Forward: Given an ordering, check each pair of indices
   - If swapping those indices produces a better score, do so
   - Iterate through the pairs in order:
        - (1,2), (1,3), ... (1,100), (2,3), ... (98,100), (99,100)
6. Pair Swap Backward: Given an ordering, check each pair of indices
   - If swapping those indices produces a better score, do so
   - Iterate through the pairs in order:
        - (100,99), (100,98), ... (100,1), (99,98), ... (3,1), (2,1)
7. Topological Sort: (may be useful if we're given a DAG)
   For each node:
   - Run DFS starting at that node and return the nodes in topological order
   Return the highest-scoring order


Bash Stuff - Ibrahim

When running on input <.in> files:
    - Add your quota of .in files locally to this directory
    - bash setup.sh 
This bash script should call main on all files with the ".in" extension in your directory and output their results to the terminal. 
