ImageSorter: ImageSorter.cpp ImageSorter.hpp
	g++ $< -lboost_filesystem -lboost_system -o $@

clean:
	rm ImageSorter