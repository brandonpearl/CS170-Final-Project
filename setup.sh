g++ -o MAS MAS.cpp

for i in `ls *.in | sort -g`
do
  echo $i >> results.out
  ./MAS $i >> results.out
done
