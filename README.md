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