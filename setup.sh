g++ -o MAS MAS.cpp

for i in `ls *.in | sort -g`
do
  echo i
  ./MAS i >> results.out
done
