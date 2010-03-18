#include "./TreeToVoxSet.hpp"
#include "voxels/Voxel.hpp"
#include "voxels/VoxelSet.hpp"
#include "voxels/VoxelSetTransforms.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <iterator>
#include <vector>
#include <boost/math/special_functions/erf.hpp>

int write_to_file(LArVox::VoxelSet * vset_ptr, std::string filename) {

	LArVox::VoxelSet::index<LArVox::lex_xyz>::type& index = vset_ptr->get<LArVox::lex_xyz>();
        LArVox::VoxelSet::index<LArVox::lex_xyz>::type::iterator vox_iter = index.begin();
        std::ofstream os;
        os.open(filename.data(), std::ios::app);
        os << "# " << filename << " - voxel coords (x, y, z), evals and e_vecs" << std::endl;
        while(vox_iter != index.end()) {
                os << **vox_iter; 
	
                ++vox_iter;
        }
        os.close();
	return 0;
}
int main(int argc, char** argv) 
{

	std::string file_name("/storage/epp1/phraar/Documents/Programs/Voxels/events/genie-770MeVQELCC.root");
	std::string tree_name("genie_qel_run");
	std::string output_name("default.dat");
	std::string branch_name("hits");

	LArVox::TreeToVoxSet t2vox(file_name, tree_name, branch_name);
	for(int i = 0; i < t2vox.numEvents(); i++) {

		LArVox::VoxelSet * vset_ptr = t2vox.getNextEvent();
//		LArVox::VoxelSet_out_by<LArVox::lex_xyz>(*vset_ptr);
		std::cout << "i = " << i << std::endl;
	}
	
//	write_to_file(vset_ptr, output_name);

	return 0;
}
