#include <iostream>
#include "voxels/TreeToTVoxel.hpp"


LArVox::TreeToTVoxel::TreeToTVoxel(std::string filename, std::string tree_name, std::string branch_name) 
: t_file_(filename.data(), "READ"), event_counter_(0) 
{
	t_tree_ = (TTree*)t_file_.Get(tree_name.data());
        tca_ = new TClonesArray("LArVox::TVoxel");
	t_tree_->GetBranch(branch_name.data())->SetAutoDelete(kFALSE);
	t_tree_->SetBranchAddress(branch_name.data(),&tca_);
	num_events_ = t_tree_->GetEntries();
}
LArVox::TreeToTVoxel::~TreeToTVoxel()
{
//	delete tca_;  //assuming this get deleted when the TTree goes out of scope 
}
std::vector<LArVox::TVoxel>* LArVox::TreeToTVoxel::getNextEvent(void)
{
	LArVox::TVoxel* read_voxel;
	std::vector<LArVox::TVoxel>* v_set = new std::vector<LArVox::TVoxel>();
	if (event_counter_ < num_events_) {
		tca_->Clear();
		t_tree_->GetEntry(event_counter_);
                int num_voxels = tca_->GetEntriesFast();

                for(int j = 0; j < num_voxels; j++) {
			
                	read_voxel = (LArVox::TVoxel*)(*tca_)[j];
			
			v_set->push_back(*read_voxel);	
                }
		++event_counter_;
	}
	return v_set;
}
