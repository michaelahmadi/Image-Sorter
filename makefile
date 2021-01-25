ImageSorter: ImageSorter.cpp
	g++ $< -lboost_filesystem -lboost_system -lexiv2 -o $@

clean:
	rm ImageSorter