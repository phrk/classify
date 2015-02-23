#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#include "hiconfig.h"
#include <iostream>
#include <fstream>

#include <map>
#include <string>

#include "hiaux/structs/hashtable.h"

#include "hiaux/strings/string_utils.h"

class Classify {

	typedef std::map< int, int > CatsHits;
	
	std::map<int, float> m_cats_weigths;
	
	std::map<int, int> m_cats_powers;
	hiaux::hashtable<std::string, float> m_idfs;
	hiaux::hashtable<std::string, CatsHits> m_term_cats_hits;

public:
	
	Classify(const std::string &_cats_powers_file,
			const std::string &_idfs_file,
			const std::string &_term_cats_nhits_file);
	
	virtual ~Classify();
	
	int getCategory (const std::string &_text);
	
};

#endif
