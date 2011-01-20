#include "TVoxel.hpp"
#include "VoxSetToTree.hpp"
#include <iostream>
#include <vector>
#include <list>

#include <boost/shared_ptr.hpp>


LArVox::VoxSetToTree::VoxSetToTree(std::string root_filename, std::string root_treename) :
	root_filename_(root_filename), 
	root_treename_(root_treename)
	//voxel_data_tree_(),
{
	std::string recreate("RECREATE");
	open_tfile(recreate);
}
void LArVox::VoxSetToTree::open_tfile(std::string mode)
{
	root_file_ = new TFile(root_filename_.data(), mode.data());
	root_tree_ = new TTree(root_treename_.data(), root_treename_.data());
	tvoxel_array_ = new TClonesArray("LArVox::TVoxel", 2000);
	voxel_data_branch_ = root_tree_->Branch("voxel_data_branch", &tvoxel_array_,  bsize, split);
}
LArVox::VoxSetToTree::~VoxSetToTree()
{
	root_tree_->Print();
	gFile = root_file_;
	root_file_->cd();
	root_tree_->Write();

	root_file_->Close();
	//Check on the what to delete and correct order to avoid Segfaults and/or memory leaks
	delete root_file_;
	//delete root_tree_;
	//delete voxel_data_tree_;
}
// fills voxel_data_tree_ with data from VoxelSet and voxel_features_branches_ with contents of the appropriate vector of TLorentzVectors
void LArVox::VoxSetToTree::operator()(LArVox::VoxelSet& v_set)
{
	
	std::cout << "LArVox::VoxSetToTree::operator()(LArVox::VoxelSet& v_set), vset.size() = " << v_set.size() << std::endl;
	VoxelSet::index<lex_xzy>::type& lex_index = v_set.get<lex_xzy>();
	VoxelSet::index<lex_xzy>::type::iterator vox_iter1 = lex_index.begin();
	TClonesArray& ar = *tvoxel_array_;
	ar.Clear();
	TLorentzVector vox_coords;
	TArrayD vox_scalars;
	int j = 0;
	while(vox_iter1 != lex_index.end()) {


		boost::shared_ptr<StructTensorVoxel> stv_ptr = boost::static_pointer_cast<StructTensorVoxel, Voxel>(*vox_iter1);
		vox_coords.SetXYZT(stv_ptr->get_x(), stv_ptr->get_y(), stv_ptr->get_z(), stv_ptr->get_weight());
		const std::vector<double>& stv_scalars = stv_ptr->get_scalars();
		vox_scalars.Set(stv_scalars.size());
		vox_scalars.Reset();
		for(int i = 0; i < vox_scalars.GetSize(); i++) {
			vox_scalars.SetAt(stv_scalars[i], i);
		}

		new(ar[j]) LArVox::TVoxel(vox_coords, vox_scalars);
//		new(ar[j]) LArVox::TVoxel(vox_coords);
		
		++vox_iter1;
		++j;
		
	}
	//Now fill the remaining branches of the root_tree
	std::cout << "features_input_vectors_.size() = " << features_input_vectors_.size() << std::endl;
	assert(features_branches_.size() ==  clones_vector_.size() &&  clones_vector_.size() == features_input_vectors_.size());
	for(int i=0; i < features_input_vectors_.size();i++) {
		for(int j=0; j < features_input_vectors_[i]->size(); j++) {
			clones_vector_[i]->Clear();
			new((*(clones_vector_[i]))[j]) TLorentzVector((*(features_input_vectors_[i]))[j]);
			std::cout << " i = " << i << " j= " << j << std::endl;	
		}
	}
	
	root_tree_->Fill();
	
}
void LArVox::VoxSetToTree::AppendClonesBranch(std::string branchname,  std::vector<TLorentzVector> & features) 
{
	features_input_vectors_.push_back(&features);
	clones_vector_.push_back(new TClonesArray("TLorentzVector", 200));
	branchnames_.push_back(branchname);
	std::cout << branchname << std::endl;
}
void LArVox::VoxSetToTree::FixBranches(void)
{
	//No option but to add branches at this stage, since the addresses of the pointers to TClonesArrays are not fixed within the vector (use a different container?)
	std::cout << " clones_vector_.size() = " <<  clones_vector_.size() << std::endl;
	for(int i = 0; i < clones_vector_.size(); i++) features_branches_.push_back(root_tree_->Branch(branchnames_[i].data(), &(clones_vector_[i]), bsize, split));
}
void LArVox::VoxSetToTree::Write(void) 
{
	gFile = root_file_;
	root_file_->cd();
	root_tree_->Write();
}
const int LArVox::VoxSetToTree::split = 0;
const int LArVox::VoxSetToTree::bsize = 64000;


LArVox::ScalarVoxSetToTree::ScalarVoxSetToTree(std::string root_filename, std::string root_treename) :
	root_filename_(root_filename), 
	root_treename_(root_treename) 
	//voxel_data_tree_(),

{
	root_file_ = new TFile(root_filename_.data(), "RECREATE");
	root_tree_ = new TTree(root_treename_.data(), root_treename_.data());
	voxel_data_tree_ = new TTree("voxel_data_tree", "voxel_data_tree");
	voxel_data_branch_ = root_tree_->Branch("voxel_data_branch", "TTree", &voxel_data_tree_,  bsize, split);
	vox_coords_ = new TLorentzVector();
	voxel_position_branch_ = voxel_data_tree_->Branch("voxel_position_branch", "TLorentzVector", &vox_coords_, bsize, split); 	
}
LArVox::ScalarVoxSetToTree::~ScalarVoxSetToTree()
{
	root_tree_->Print();
	gFile = root_file_;
	root_file_->cd();
	root_tree_->Write();
	root_file_->Close();
	delete root_file_;
	//delete root_tree_;
	//delete voxel_data_tree_;
	delete vox_coords_;
}
void LArVox::ScalarVoxSetToTree::operator()(LArVox::VoxelSet& v_set)
{
	std::cout << "LArVox::ScalarVoxSetToTree::operator()(LArVox::VoxelSet& v_set)" << std::endl;
	VoxelSet::index<lex_xzy>::type& lex_index = v_set.get<lex_xzy>();
	VoxelSet::index<lex_xzy>::type::iterator vox_iter1 = lex_index.begin();
	while(vox_iter1 != lex_index.end()) {
		boost::shared_ptr<StructTensorVoxel> stv_ptr = boost::static_pointer_cast<StructTensorVoxel, Voxel>(*vox_iter1);
		vox_coords_->SetXYZT(stv_ptr->get_x(), stv_ptr->get_y(), stv_ptr->get_z(), stv_ptr->get_weight());
		//std::cout << stv_ptr->get_weight() << std::endl;
		voxel_data_tree_->Fill();
		++vox_iter1;
	}
	root_tree_->Fill();
	
}
void LArVox::ScalarVoxSetToTree::operator()(std::list<LArVox::Voxel_ptr>& v_list) {
	std::cout << "LArVox::ScalarVoxSetToTree::operator()(std::list<LArVox::Voxel_ptr>& v_list)" << std::endl;
	std::list<LArVox::Voxel_ptr>::iterator v_list_it = v_list.begin();
	 while(v_list_it != v_list.end()) {
                boost::shared_ptr<StructTensorVoxel> stv_ptr = boost::static_pointer_cast<StructTensorVoxel, Voxel>(*v_list_it);
                vox_coords_->SetXYZT(stv_ptr->get_x(), stv_ptr->get_y(), stv_ptr->get_z(), stv_ptr->get_weight());
                //std::cout << stv_ptr->get_weight() << std::endl;
                voxel_data_tree_->Fill();
                ++v_list_it;
        }
	//if(!v_list.empty()) root_tree_->Fill(); 
	root_tree_->Fill();
}
const int LArVox::ScalarVoxSetToTree::split = 0;
const int LArVox::ScalarVoxSetToTree::bsize = 64000;
