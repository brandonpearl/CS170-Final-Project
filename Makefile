all:
	g++ instance_generator.cpp -o instance_generator
	#g++ solver_josh.cpp -o solver_josh
	#g++ solver_matt.cpp -o solver_matt
	#g++ solver_brandon.cpp -o solver_brandon
	g++ MAS.cpp -o MAS
clean:
	rm instance_generator
	# rm solver_josh
	# rm solver_matt
	# rm solver_brandon
	rm MAS
