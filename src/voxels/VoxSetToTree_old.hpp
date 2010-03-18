#ifndef VOXELS_VOXSETTOTREE_HPP_
#define VOXELS_VOXSETTOTREE_HPP_
#include <string>
#include <list>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "TArrayD.h"
#include "voxels/VoxelSet.hpp"
namespace LArVox {
class VoxSetToTree {
public:
	VoxSetToTree(std::string root_filename, std::string root_treename);
	~VoxSetToTree();
	void operator()(LArVox::VoxelSet& v_set);
	void Open(void);
	void Write(void);
private:
	void open_tfile(std::string mode);
	std::string root_filename_;
	std::string root_treename_;
	TTree* root_tree_;
	TTree* voxel_data_tree_;
	TFile* root_file_;
	
	TBranch* voxel_data_branch_;
	TBranch* voxel_position_branch_;
	TBranch* voxel_scalars_branch_;
	std::vector<TBranch*> voxel_features_branches;
	TLorentzVector* vox_coords_;
	TArrayD* vox_scalars_;	
	static const int split;
	static const int bsize;

};
class ScalarVoxSetToTree {
public:
	ScalarVoxSetToTree(std::string root_filename, std::string root_treename);
        ~ScalarVoxSetToTree();
        void operator()(LArVox::VoxelSet& v_set);
	void operator()(std::list<LArVox::Voxel_ptr>& v_list);
private:
        std::string root_filename_;
        std::string root_treename_;
        TTree* root_tree_;
        TTree* voxel_data_tree_;
        TFile* root_file_;
 
        TBranch* voxel_data_branch_;
        TBranch* voxel_position_branch_;
        TLorentzVector* vox_coords_;
        static const int split;
        static const int bsize;
                
};

} //namespace LArVox
#endif //VOXELS_VOXSETTOTREE_HPP_
