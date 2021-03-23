#Matthew Turi
#ID: 822202323
#03/23/21

CXX = g++
CXXFLAGS += -std=c++11


a3 : byu_tracereader.o output_helpers.o helper_methods.o pagetable.o main.o
	$(CXX) $(CXXFLAGS) byu_tracereader.o helper_methods.o pagetable.o output_helpers.o main.o -o pagetable

byu_tracereader.o : include/byutr.h src/byu_tracereader.cpp
	$(CXX) $(CXXFLAGS) -c src/byu_tracereader.cpp

output_helpers.o : include/output_helpers.h src/output_helpers.cpp
	$(CXX) $(CXXFLAGS) -c src/output_helpers.cpp

helper_methods.o : include/helper_methods.h src/helper_methods.cpp
		$(CXX) $(CXXFLAGS) -c src/helper_methods.cpp

pagetable.o: include/pagetable.h src/pagetable.cpp
	$(CXX) $(CXXFLAGS) -c src/pagetable.cpp

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp
clean :
	rm *.o