
#include "File.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include "options.h"


std::string readSourceFile(std::string path) {
    std::string expandedPath = realpath(path.c_str(), nullptr);
    std::ifstream t(expandedPath);
    std::stringstream buffer;

    if (!t.is_open()) {
	if (verbose)
	    std::cout << "Failed to open file: " << path << std::endl;
	throw 1;
    }

    buffer << t.rdbuf();

    return buffer.str();
}


bool isReadableFile(std::string path) {
    std::ifstream f(path.c_str());
    return f.good();
}
