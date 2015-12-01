g++ -o MAS MAS.cpp

for i in `ls *.in | sort -g`
do
  MAS i >> results.out
done
