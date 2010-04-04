#include <iostream>
#include <boost/shared_ptr.hpp>
#include "voxels/Voxel.hpp"
#include "voxels/VoxelSet.hpp"
#include "voxels/VoxSetToTree.hpp"
#include "voxels/TreeToVoxSet.hpp"
#include "voxels/TreeToTVoxel.hpp"
#include <vector>
#include "TLorentzVector.h"
int main() {
	std::string filename("/storage/epp1/phraar/Documents/Programs/Voxels/events/genie-770MeVQELCC.root");
	std::string treename("genie_qel_run");
	std::string branchname("hits");
        LArVox::TreeToVoxSet t2vox(filename, treename, branchname);
        LArVox::VoxelSet * vset_ptr = t2vox.getNextEvent();
	std::cout << "creating LArVox::VoxSetToTree vstt" << std::endl;

	//Add a scalar to voxels of the VoxelSet vset_ptr
	LArVox::VoxelSet::index<LArVox::lex_xzy>::type& lex_index = vset_ptr->get<LArVox::lex_xzy>();
        LArVox::VoxelSet::index<LArVox::lex_xzy>::type::iterator vox_iter1 = lex_index.begin();
	 while(vox_iter1 != lex_index.end()) {
		boost::shared_ptr<LArVox::StructTensorVoxel> stv_ptr = boost::static_pointer_cast<LArVox::StructTensorVoxel, LArVox::Voxel>(*vox_iter1);
		stv_ptr->append_scalar(-1.0);
		stv_ptr->append_scalar(-2.0);
		vox_iter1++;
	}
	LArVox::VoxSetToTree* vstt = new LArVox::VoxSetToTree("root_out.root", "test_tree");
	//make a vector of LorentzVectors and add it to a new branch
	
	std::string br1("br1");
	std::vector<TLorentzVector*> feat1;
	feat1.push_back(new TLorentzVector(1.,2.,3.,4.));
	feat1.push_back(new TLorentzVector(2.,3.,5.,4.));
	
	vstt->AppendClonesBranch(br1, feat1);
	(*vstt)(*vset_ptr);
	vset_ptr = t2vox.getNextEvent();
        std::cout << "creating LArVox::VoxSetToTree vstt" << std::endl;
        (*vstt)(*vset_ptr);
	delete vstt;
	//Now test the reading in of TVoxels

	LArVox::TreeToTVoxel tttv("root_out.root", "test_tree", "voxel_data_branch");
	std::vector<LArVox::TVoxel>* tvoxvec = tttv.getNextEvent();
	
	std::vector<LArVox::TVoxel>::iterator tvoxvec_it = tvoxvec->begin();
	while(tvoxvec_it != tvoxvec->end()) {
		std::cout << tvoxvec_it->vox_coords_.X() << std::endl;
		tvoxvec_it++;
	} 
		
	return 0;
}

