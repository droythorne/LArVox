#ifndef LARVOX_CMDLINEPARSER_HPP_
#define LARVOX_CMDLINEPARSER_HPP_
#include <iostream>
namespace LArVox {
class Options;
std::ostream& operator<<(std::ostream& os, const Options & opt);
struct Options {
	
	Options();
	friend std::ostream& operator<<(std::ostream& os, const Options & opt);	
	bool parsed;
        unsigned long skipEvents;
        unsigned long maxEvents;
        int kernel_size;
        double voxel_size;
        double scale_t;
        double gamma;

        std::string root_filename_in;
        std::string root_filename_out;
        std::string root_treename;
        std::string root_branchname;
       
	 bool out_to_root;
};
class CmdLineParser {
	public:
		Options parse(int argc, char** argv);
};
} //namespace LArVox
#endif //LARVOX_CMDLINEPARSER_HPP_

