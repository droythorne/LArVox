#include <iostream>
#include <string>
#include <vector>
#include "TLorentzVector.h"
#include "voxels/VoxelSet.hpp"
#include "voxels/VoxSetToTree.hpp"
#include "voxels/TreeToVoxSet.hpp"

int main() {
	std::string filename("/storage/epp1/phraar/Documents/Programs/Voxels/events/genie-770MeVQELCC.root");
        std::string treename("genie_qel_run");
        std::string branchname("hits");
	std::string feature_branch("feature1");
	std::string feature_branch2("feature2");
        LArVox::TreeToVoxSet t2vox(filename, treename, branchname);
        LArVox::VoxelSet * vset_ptr = t2vox.getNextEvent();
        std::cout << "creating LArVox::VoxSetToTree vstt" << std::endl;
        LArVox::VoxSetToTree vstt("test_vstt_mk2.root", "test_tree");
	
	std::vector<TLorentzVector*> four_vecs;
	std::vector<TLorentzVector*> four_vecs2;
	
	for(int i = 0; i<10000; i++) four_vecs.push_back(new TLorentzVector(i,i,i,i));
	for(int i = 0; i<10000; i++) four_vecs2.push_back(new TLorentzVector(2*i,2*i,2*i,2*i));
		
	vstt.AppendClonesBranch(feature_branch, four_vecs);
	vstt.AppendClonesBranch(feature_branch2, four_vecs2);
	vstt(*vset_ptr);
	
	//suspect this will result in a seg fault!

//	std::vector<TLorentzVector*>::iterator four_vecs_it = four_vecs.begin();
//	while(four_vecs_it != four_vecs.end()) delete *four_vecs_it;
		
	return 0;
}
