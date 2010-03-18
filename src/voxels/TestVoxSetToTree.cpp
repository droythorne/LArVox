#include <iostream>
#include "voxels/VoxelSet.hpp"
#include "voxels/VoxSetToTree.hpp"
#include "voxels/TreeToVoxSet.hpp"

int main() {
	std::string filename("/storage/epp1/phraar/Documents/Programs/Voxels/events/genie-770MeVQELCC.root");
	std::string treename("genie_qel_run");
	std::string branchname("hits");
        LArVox::TreeToVoxSet t2vox(filename, treename, branchname);
        LArVox::VoxelSet * vset_ptr = t2vox.getNextEvent();
	std::cout << "creating LArVox::VoxSetToTree vstt" << std::endl;
	LArVox::VoxSetToTree vstt("root_out.root", "test_tree");
	vstt(*vset_ptr);
	vset_ptr = t2vox.getNextEvent();
        std::cout << "creating LArVox::VoxSetToTree vstt" << std::endl;
        vstt(*vset_ptr);
	
	return 0;
}

