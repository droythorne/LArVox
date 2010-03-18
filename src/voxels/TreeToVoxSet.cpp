#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include<iostream>
#include <string>
#include "voxels/Voxel.hpp"
#include "voxels/VoxelSet.hpp"
#include "voxels/TreeToVoxSet.hpp"

LArVox::TreeToVoxSet::TreeToVoxSet(std::string filename, std::string tree_name, std::string branch_name) 
: t_file_(filename.data(), "READ"), event_counter_(0) 
{
	t_tree_ = (TTree*)t_file_.Get(tree_name.data());
        tca_ = new TClonesArray("TLorentzVector");
	t_tree_->GetBranch(branch_name.data())->SetAutoDelete(kFALSE);
	t_tree_->SetBranchAddress(branch_name.data(),&tca_);
	num_events_ = t_tree_->GetEntries();
}
LArVox::TreeToVoxSet::~TreeToVoxSet()
{
//	delete tca_;  //assuming this get deleted when the TTree goes out of scope 
}
LArVox::VoxelSet* LArVox::TreeToVoxSet::getNextEvent(void)
{
	TLorentzVector* read_lorvec;
	LArVox::VoxelSet* v_set = new LArVox::VoxelSet();
	if (event_counter_ < num_events_) {
		tca_->Clear();
		t_tree_->GetEntry(event_counter_);
                int num_voxels = tca_->GetEntriesFast();

                for(int j = 0; j < num_voxels; j++) {
			
                	read_lorvec = (TLorentzVector*)(*tca_)[j];
			LArVox::Voxel_ptr temp_vox = LArVox::Voxel_ptr(new LArVox::StructTensorVoxel(read_lorvec->X(),  read_lorvec->Y(), read_lorvec->Z(), read_lorvec->E()));
			
			v_set->insert(temp_vox);	
                }
		++event_counter_;
	}
	return v_set;
}
//reads every skip'th event to a VoxelSet
LArVox::VoxelSet* LArVox::TreeToVoxSet::getNextEvent(unsigned int skip)
{
	TLorentzVector* read_lorvec;
        LArVox::VoxelSet* v_set = new LArVox::VoxelSet();
        if (event_counter_ < num_events_) {
        
                int get_entry_flag = t_tree_->GetEntry(event_counter_);
		if(get_entry_flag == 0 || get_entry_flag == -1) std::cout << "t_tree_->GetEntry(event_counter_) failed\n";
                int num_voxels = tca_->GetEntries();


                for(int j = 0; j < num_voxels; j++) {

                        read_lorvec = (TLorentzVector*)(*tca_)[j];
                        LArVox::Voxel_ptr temp_vox = LArVox::Voxel_ptr(new LArVox::StructTensorVoxel(read_lorvec->X(),  read_lorvec->Y(), read_lorvec->Z(), read_lorvec->E()));

                        v_set->insert(temp_vox);
                }
                event_counter_ += skip;
        }
        return v_set;
}
int LArVox::TreeToVoxSet::numEvents(void) {
	return num_events_;
}
void LArVox::TreeToVoxSet::skipEvents(unsigned long skip_events) {
	
	event_counter_ = skip_events;
}
