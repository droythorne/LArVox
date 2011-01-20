
#ifndef VOXELS_TREETOTVOXEL_HPP_
#define VOXELS_TREETOTVOXEL_HPP_
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TVoxel.hpp"
#include <string>

namespace LArVox {

class TreeToTVoxel {
	public:
		TreeToTVoxel(std::string filename, std::string tree_name, std::string branch_name);
		~TreeToTVoxel();
		std::vector<LArVox::TVoxel>* getNextEvent(void);
		int numEvents(void);
	private:
		TFile t_file_;
		TTree* t_tree_;
		TClonesArray* tca_ ;	
		long num_events_;
		long event_counter_;	
}; 
} //namespace LArVox
#endif //VOXELS_TREETOTVOXEL_HPP_
