g++ -o MAS MAS.cpp

for i in *.in 
do
  MAS i >> results.out
done