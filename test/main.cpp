#include "hiconfig.h"
#include "classify.h"


int main (int argv, char **argc) {
	
	Classify c ( "../../crawlerd/cats_powers.txt",
		 		"../../crawlerd/idfs.txt",
				"../../crawlerd/term_cats_nhits.txt");
	
	return 0;
}

