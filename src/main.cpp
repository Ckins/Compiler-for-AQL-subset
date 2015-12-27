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

int main(int argc, char**argv) {
	std::string dir_name(argv[2]);
	std::string dataset_path = "./../dataset/"+dir_name;

	char* c_dataset_path = new char[255];
	int i;
	for (i = 0; i < dataset_path.length(); i++) c_dataset_path[i] = dataset_path[i];
	c_dataset_path[i] = '\0'; 

	struct dirent *pdirent;
	DIR *d_info =  opendir(c_dataset_path);
	
	if (d_info) {   // is a dir
		// traverse the dir
		while ((pdirent = readdir(d_info)) != NULL) {
			if (pdirent->d_type & DT_REG) {   // is a file
				int len = 0;
				char* name = pdirent->d_name;

				while (name[len] != '\0') len++;
				if (len > 6) {
					if (name[len-1] == 't' && name[len-2] == 'u' && name[len-3] == 'p' && name[len-4] == 'n'
						&& name[len-5] == 'i' && name[len-6] == '.') {
						std::string file_name(name);
						Tokenizer test("./../dataset/"+dir_name+"/"+file_name);
						
						Lexer lexer(argv[1]);
						vector<CodeToken> v = lexer.get_list();

						Parser parser(v, test);

						cout << endl << endl << "===========================================================" << endl<< endl;
					}
				}
			}
		}
	} else {   // is a file
		Tokenizer test(dataset_path);

		Lexer lexer(argv[1]);
		vector<CodeToken> v = lexer.get_list();

		Parser parser(v, test);
	}
	
	return 0;
}
