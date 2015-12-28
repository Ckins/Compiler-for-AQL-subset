#include "tokenizer.h"
#include "code_token.h"
#include "lexer.h"
#include "parser.h"
#include <cstdio>
#include <iostream>
#include <cstring>

#include <sys/types.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/stat.h>

using namespace std;

char* convert_string_to_char(string);
bool check_end_with(string, string);

string divider = "==================================================================";

int main(int argc, char**argv) {
	string aql_name(argv[1]);
	string aql_path = "./../dataset/"+aql_name;
	char* c_aql_path = convert_string_to_char(aql_path);

	// check if aql file exists
	FILE * fp = fopen(c_aql_path, "r");
	if (fp == NULL) {
		cout << "error: file \""+aql_path+"\" does not exist!" << endl;

		return 0;
	}

	// check if the file ends with .aql
	string end(".aql");
	if (!check_end_with(aql_name, end)) {
		cout << "error: please make sure your aql file ends with \".aql\"" << endl;

		return 0;
	}


	string dataset_name(argv[2]);
	string dataset_path = "./../dataset/"+dataset_name;
	char* c_dataset_path = convert_string_to_char(dataset_path);

	struct dirent *pdirent;
	DIR *d_info =  opendir(c_dataset_path);
	
	if (d_info) {   // is a dir
		// traverse the dir
		while ((pdirent = readdir(d_info)) != NULL) {
			if (pdirent->d_type & DT_REG) {   // is a file
				int len = 0;
				char* name = pdirent->d_name;
				string file_name(name);

				while (name[len] != '\0') len++;
				// check if the file ends with .input
				string end2(".input");
				if (check_end_with(name, end2)) {
					Tokenizer test("./../dataset/"+dataset_name+"/"+file_name);
						
					Lexer lexer(argv[1]);
					vector<CodeToken> v = lexer.get_list();

					Parser parser(v, test);

					cout << endl << endl << divider << endl << endl << endl;
				}
				if (len > 6) {
					if (name[len-1] == 't' && name[len-2] == 'u' && name[len-3] == 'p' && name[len-4] == 'n'
						&& name[len-5] == 'i' && name[len-6] == '.') {
						std::string file_name(name);
						
					}
				}
			}
		}
	} else {   // is a file or not exist
		FILE * fp = fopen(c_dataset_path, "r");
		if (fp == NULL) {
			cout << "error: file \""+dataset_path+"\" does not exist!" << endl;
		} else {
			Tokenizer test(dataset_path);

			Lexer lexer(argv[1]);

			vector<CodeToken> v = lexer.get_list();

			Parser parser(v, test);
		}
	}
	
	return 0;
}

char* convert_string_to_char(string name) {
	int i;
	char* c_name = new char[255];
	for (i = 0; i < name.length(); i++) c_name[i] = name[i];
	c_name[i] = '\0';

	return c_name;
}

bool check_end_with(string name, string end) {
	int name_len = name.length();
	int end_len = end.length();
	if (name_len <= end_len) return false;

	int j = name_len-1;
	int k = end_len-1;
	for (int i = 0; i < end_len; i++) {
		if (name[j] != end[k]) return false;
		j--;
		k--;
	}


	return true;
}
