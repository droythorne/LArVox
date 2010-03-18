#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include <unistd.h>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include "voxels/TreeToVoxSet.hpp"
#include "voxels/VoxSetToTree.hpp"
#include "voxels/VoxelSet.hpp"
#include "voxels/VoxelNonMaxSupp.hpp"

#include "my_kdrecon_classes/HarrisPlessey.hpp"
#include "./CmdLineParser.hpp"
void displayHelp(const char* name);
int main(int argc, char* argv[])
{ 
	LArVox::CmdLineParser cmd;
	LArVox::Options opt = cmd.parse(argc, argv);	
	
	LArVox::TreeToVoxSet* t2vox = new  LArVox::TreeToVoxSet(opt.root_filename_in, opt.root_treename, opt.root_branchname);
	t2vox->skipEvents(opt.skipEvents);
	int num_events_in_tree = t2vox->numEvents();
// Insert a utility function here to determine the number, type and parameters of the features functions (contruct a vector comprising
// objects of these types once a base class has been constructed which they all inherit from) 
	LArVox::VoxSetToTree vstt(opt.root_filename_out, opt.root_treename);
//these filenames are now redundant - replace with automatically generated branchnames which are aded to the output ROOT files with AppendClonesBranch
	std::string forster_filename("forster.root");
	std::string forster_thresh_filename("forster_thresh.root");
	std::string forster_ev_filename("forster_ev.root");
	std::cout << opt.root_filename_out << std::endl;
	std::cout << "Entering event loop" << std::endl;
//see above
	LArVox::ScalarVoxSetToTree* for_tree = new LArVox::ScalarVoxSetToTree(forster_filename, opt.root_treename);
       	LArVox::ScalarVoxSetToTree* for_thresh_tree = new LArVox::ScalarVoxSetToTree(forster_thresh_filename, opt.root_treename); 
       	LArVox::ScalarVoxSetToTree* for_ev_tree = new LArVox::ScalarVoxSetToTree(forster_ev_filename, opt.root_treename); 
	LArVox::ForsterAccessor fa;
	LArVox::ForsterThresholdAccessor fta;
	LArVox::HPMinEigValAccessor mev;
	KDRecon::HarrisPlessey* hp = new KDRecon::HarrisPlessey(opt.kernel_size);
        LArVox::CrudeVoxelNonMaxSupp* nms = new LArVox::CrudeVoxelNonMaxSupp(fa);
        LArVox::CrudeVoxelNonMaxSupp* nmst = new LArVox::CrudeVoxelNonMaxSupp(fta);
	LArVox::CrudeVoxelNonMaxSupp* nmsev = new LArVox::CrudeVoxelNonMaxSupp(mev);
	// need to implement a facility for parsing these options - DONE 18/03/10
	nms->SetThreshold(0.00034);
	nms->SetWindowSize(40);
        nmst->SetThreshold(0.7);
	nmst->SetWindowSize(30);
	nmsev->SetThreshold(0.05);
	nmsev->SetWindowSize(30);
	LArVox::VoxelSet* vset_ptr;
	unsigned long num_processed_events(0);	
	while(((opt.maxEvents == 0) || (num_processed_events < opt.maxEvents)) && num_processed_events < num_events_in_tree) {
	
		vset_ptr = t2vox->getNextEvent();

		bool vset_empty = vset_ptr->get<LArVox::lex_xyz>().empty();
		std::cout << "vset.size() = " << vset_ptr->size() << std::endl;
		std::cout << "*check v_set_ptr empty " << vset_empty << std::endl;
		if(vset_empty) {
			std::cout << "vset_empty\n"; 
			++(num_processed_events);
			break;
		}
		int vset_size = vset_ptr->get<LArVox::lex_xyz>().size();
       		std::cout << "Num elements of v_set = " << vset_size;
		//LArVox::VoxelSet_out_by<LArVox::lex_xyz>(*vset_ptr);	
		(*hp)(*vset_ptr, opt.voxel_size, opt.scale_t, opt.gamma);
		//	LArVox::VoxelSet_out_by<LArVox::lex_xyz>(*vset_ptr);
		vstt(*vset_ptr);
        	std::list<LArVox::Voxel_ptr> vox_list = (*nms)(*vset_ptr);
		std::cout << "vox_list contains " << vox_list.size() << " voxels" << std::endl;
		(*for_tree)(vox_list);
		vox_list.clear();
		vox_list = (*nmst)(*vset_ptr);
		(*for_thresh_tree)(vox_list);
		vox_list.clear();
		vox_list = (*nmsev)(*vset_ptr);
		(*for_ev_tree)(vox_list);


		++num_processed_events;
		
		std::cout << "num_processed_events" << num_processed_events << std::endl;
	}
	std::cout << "exited processing - processed " << num_processed_events << " events\n";
	delete t2vox;
	delete hp;
	delete for_tree;
	delete for_thresh_tree;
	delete for_ev_tree;
	
	delete nms;
	delete nmst;
	delete nmsev;
	return 0;
}

 

void displayHelp(const char* name)
{
        std::cerr << "\nUsage: " << name << " [options] datafile\n";
        std::cerr << "Options:\n";
        std::cerr << "\t-R root_file_filename_out\tFilename for writing ROOT TTree.\n";
        std::cerr << "\t-T root_treename\tName of input ROOT TTree.\n";
	std::cerr << "\t-B root_branchname\tName of input ROO TBranch.\n";
	std::cerr << "\t-F (TODO)\tSpecifies which feature points are to be extracted.\n";
	std::cerr << "\t-m maxEvents\tSets the maximum number of events to be read from root_treename.\n";
	std::cerr << "\t-q Quiet mode\tDoes no output any ROOT file (NOT IMPLEMENTED).\n";
	std::cerr << "\t-s skipEvents\tSelects every skipEvents'th event from root_treename.\n";
	std::cerr << "\t-t scale_t\tSet the scale-space parameter (mm) for the Harris & Stevens/Plessey transform.\n";
	std::cerr << "\t-g gamma\tSets the gamma factor for the Harris & Stevens/Plessey integration window, s = gamma^2*t.\n";
	std::cerr << "\t-k kernel_size\tCube side-length in voxel units of the scale-space convolution for the H&P/P transform.\n";
	std::cerr << "\t-v voxel_size\tVoxel dimension (mm).\n"; 
}
