#ifndef VOXELS_TREETOVOXSET_HPP_
#define VOXELS_TREETOVOXSET_HPP_
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

#include <string>

#include "VoxelSet.hpp"
namespace LArVox {

class TreeToVoxSet {
	public:
		TreeToVoxSet(std::string filename, std::string tree_name, std::string branch_name);
		~TreeToVoxSet();
		LArVox::VoxelSet* getNextEvent(void);
		LArVox::VoxelSet* getNextEvent(unsigned int skip);
		int numEvents(void);
		void skipEvents(unsigned long skip_events);
	private:
		TFile t_file_;
		TTree* t_tree_;
		TClonesArray* tca_ ;	
		long num_events_;
		long event_counter_;	
}; 
} //namespace LArVox
#endif //VOXELS_TREETOVOXSET_HPP_
