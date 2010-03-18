#include <iostream>
#include "./CmdLineParser.hpp"

int main(int argc, char ** argv) {

	LArVox::CmdLineParser cmd;
        LArVox::Options opt = cmd.parse(argc, argv);

	std::cout << opt;

	return 0;
}

