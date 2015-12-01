g++ -o MAS MAS.cpp

for i in `ls *.in | sort -V`
do
  MAS i >> results.out
done
