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
	
	auto cit = m_cats_powers.begin();
	auto cend = m_cats_powers.end();
	
	while (cit != cend) {
		
		m_cats_weigths [ cit->first ] = 0.0f; 
		cit++;
	}
}

Classify::~Classify() {
	
}

int Classify::getCategory (const std::string &_text) {
	
	std::vector<std::string> tokens;
		
	std::set<uint32_t> delims;
	delims.insert(0x2E); // '.'
	delims.insert(0xA); // '\n' 
	
	delims.insert(0x20); // ' '
	delims.insert(0x2C); // ','
	delims.insert(0x3B); // ';'
	delims.insert(0x3A); // ':'
	delims.insert(0x2D); // '-'
	delims.insert(0x28); // '('
	delims.insert(0x29); // ')'
	delims.insert(0x7B); // '{'
	delims.insert(0x7D); // '}'
	delims.insert(0x22); // '"'
	delims.insert(0x27); // '''
	delims.insert(0x3C); // '<'
	delims.insert(0x3E); // '>'
	
	splitUtf8(_text, delims, tokens);
	
	std::map<int, float> cats_weigths = m_cats_weigths;
	
	//std::cout << "cats_weigths: " << cats_weigths.size() << std::endl;
	
	auto cit = cats_weigths.begin();
	auto cend = cats_weigths.end();
	
	while (cit != cend) {
		
		int category = cit->first;
		float cat_weight = 0.0f;
		
		for (int i = 0; i<tokens.size(); i++)
			if (tokens[i].size() > 3) {
						
				auto token_idf_it = m_idfs.find(tokens[i]);
				
				if (token_idf_it == m_idfs.end()) {
					
					//std::cout << "no such token: " << tokens[i] << std::endl; 
					continue;
				}
				
				//std::cout << "token idf: " << tokens[i]  << " " << token_idf_it->second << std::endl;
				
				if (token_idf_it->second < 0)
					continue;
				
				
				float idf = token_idf_it->second;
				
				auto token_freq_it = m_term_cats_hits.find( tokens[i] );
				if (token_freq_it == m_term_cats_hits.end()) {
					//std::cout << "no token_freq_it for " << tokens[i] << std::endl;
					continue;
				}
				
				auto token_cat_hits_it = token_freq_it->second.find( category );
				if (token_cat_hits_it == token_freq_it->second.end()) {
					
					//std::cout << "no token_cat_hits_it for " << tokens[i] << " cat:" << category << std::endl; 
					
					continue;
				}
				
				//std::cout << "got token_cat_hits_it " << tokens[i] << " cat:" << category << std::endl;
				
				float tf = token_cat_hits_it->second;
				float k1 = 2.0f;
				float b = 0.75f;
				
				float cat_length = 1.0f;
				float avgcl = 1.0f;
				
				//std::cout << tokens[i] << " : " << category << " tf: " << tf << " idf: " << idf << std::endl;
				
				float cat_weight_add = idf * tf * ( k1 + 1 ) / ( tf + k1 * (1 - b + b * cat_length / avgcl ) );
				
				//std::cout << "cat_weight_add: " << cat_weight_add << std::endl;
				
				cat_weight += cat_weight_add;
			}
		
		cit->second = cat_weight;
		cit++;
	}
	
	auto it = cats_weigths.begin();
	auto end = cats_weigths.end();
	
	float winner_w = 0;
	int winner_cat = 0;
	
	while (it != end) {
		
		//std::cout << it->first << " / " << it->second << std::endl;
		
		if (it->second > winner_w) {
			
			winner_w = it->second;
			winner_cat = it->first;
		}
		
		it++;
	}
	
	return winner_cat;
}

