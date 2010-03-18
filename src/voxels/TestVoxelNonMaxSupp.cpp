#include <string>
#include <list>
#include "voxels/TreeToVoxSet.hpp"
#include "voxels/VoxelNonMaxSupp.hpp"
#include "voxels/VoxelSet.hpp"
#include "voxels/Voxel.hpp"
using namespace LArVox;
int main(int argc, char** argv) {
	std::string filename("/storage/epp1/phraar/Documents/Programs/Voxels/events/genie-770MeVQELCC.root");
        std::string treename("genie_qel_run");
        std::string branchname("hits");
	
	TreeToVoxSet t2vox(filename, treename, branchname);
	VoxelSet* vset_ptr = t2vox.getNextEvent();
	ForsterAccessor	fa;
	CrudeVoxelNonMaxSupp nms(fa);
	std::list<Voxel_ptr> vox_list = nms(*vset_ptr);
	
		
	std::list<Voxel_ptr>::const_iterator feat_it = vox_list.begin();
	while(feat_it != vox_list.end()) {
		std::cout << **feat_it << std::endl;
	}	
	return 0;
}
