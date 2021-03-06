//
// Created by wax on 1/25/17.
//

#include <iostream>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <limits.h>

#include "options.hh"
#include "IO.hh"

namespace IO {
    std::ifstream in;

    std::string getWorkingDir();

    bool init() {
        std::string path = getWorkingDir();
        std::string inPath = path + "/interact";

        in.open(inPath);

        if (in.fail()) {
	    if (verbose)
		std::cout << "Could not open FIFO '" << inPath << "': " << std::strerror(errno) << std::endl;
            return false;
        }

        return true;
    }

    std::string getWorkingDir() {
        char buf[PATH_MAX];
        getcwd(buf, PATH_MAX);
        return std::string(buf);
    }

}

