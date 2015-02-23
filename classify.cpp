#include "classify.h"

Classify::Classify(const std::string &_cats_powers_file,
		const std::string &_idfs_file,
		const std::string &_term_cats_nhits_file) {
	
	std::string line;
	std::ifstream catsf(_cats_powers_file.c_str());
	
	std::getline(catsf, line);
	std::getline(catsf, line);
	
	int cat;
	int power;
	
	while ( catsf >> cat &&
		 	catsf >> power ) {
				
		m_cats_powers[ cat ] = power;
	}
	
	catsf.close();
	
	std::string term;
	float idf;
	
	std::ifstream idfsf(_idfs_file.c_str());
	
	std::getline(idfsf, line);
	std::getline(idfsf, line);
	
	while ( idfsf >> term &&
			 idfsf >> idf) {
		
		m_idfs [ term ] = idf;
	}
	
	idfsf.close();
	
	std::ifstream cats_nhitsf(_term_cats_nhits_file.c_str());
	
	std::getline(cats_nhitsf, line);
	std::getline(cats_nhitsf, line);
	
	while ( std::getline(cats_nhitsf, line) ) {
		
		std::string term;
		
		std::set<uint32_t> delims;
		delims.insert(0x09); // by tabulation
		
		std::vector< std::string > tokens;
		splitUtf8(line, delims, tokens);
		
		if (tokens.size() < 2)
			continue;
		
		term = tokens[0];
		
		CatsHits cats_hits;
		
		for (int i = 1; i<tokens.size(); i++)
			if (tokens[i].size() > 2) {
				
				int delim_pos = tokens[i].find(":");
				if (delim_pos == std::string::npos)
					continue;
				
				std::string cat_str = tokens[i].substr(0, delim_pos);
				std::string nhits_str = tokens[i].substr(delim_pos + 1,  tokens[i].size() - delim_pos - 1);
				
				int cat = strtoint(cat_str);
				int nhits = strtoint(nhits_str);
				
				cats_hits[ cat ] = nhits;
			}
		
		m_term_cats_hits [ term ] = cats_hits;
	}
	
	cats_nhitsf.close();
}

Classify::~Classify() {
	
}

int Classify::getCategory (const std::string &_text) {
	
}

