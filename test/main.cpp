#include "hiconfig.h"
#include "classify.h"


int main (int argv, char **argc) {
	
	Classify c ( "../../crawlerd/cats_powers.txt",
		 		"../../crawlerd/idfs.txt",
				"../../crawlerd/term_cats_nhits.txt");
	
	int cat = c.getCategory("навальный путин выборы");			
	
	std::cout << "cat: " << cat << std::endl;
	
	return 0;
}

