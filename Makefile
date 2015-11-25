all:
	g++ instance_generator.cpp -o instance_generator
	g++ solver_josh.cpp -o solver_josh
clean:
	rm instance_generator
	rm solver_josh