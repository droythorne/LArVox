#ifndef LARVOX_CMDLINEPARSER_HPP_
#define LARVOX_CMDLINEPARSER_HPP_
#include <iostream>
#include <string>
#include "VoxelNonMaxSupp.hpp"
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
	
	std::vector<std::string> feature_names;
	std::vector<double> thresholds;
	std::vector<double> windows;
};
class CmdLineParser {
	public:
		Options parse(int argc, char** argv);
		std::vector<std::string>& getAllowedFeatures();
	private:

		std::vector<std::string> allowed_features_;
		
};
class ScalarFieldAllocator {
	public:
	ScalarFieldAllocator(Options& opt, std::vector<std::string>& allowed_features);
	~ScalarFieldAllocator();
	std::vector<VoxelNonMaxSuppInterface*>& getScalars();

	private:
	ScalarFieldAllocator(const ScalarFieldAllocator& sfa);
	ScalarFieldAllocator& operator=(const ScalarFieldAllocator& sfa);
	std::vector<LArVox::VoxelNonMaxSuppInterface*> scalars_;
};
} //namespace LArVox
#endif //LARVOX_CMDLINEPARSER_HPP_

