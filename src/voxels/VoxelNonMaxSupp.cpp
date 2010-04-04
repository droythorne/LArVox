#include "voxels/VoxelNonMaxSupp.hpp"
#include <iostream>
double LArVox::ForsterAccessor::operator()(const LArVox::Voxel_ptr& v_ptr) const {return boost::dynamic_pointer_cast<LArVox::StructTensorVoxel, LArVox::Voxel>(v_ptr)->get_scalars()[LArVox::ForsterAccessor::INDEX];}
const int LArVox::ForsterAccessor::INDEX = 13;

double LArVox::ForsterThresholdAccessor::operator()(const LArVox::Voxel_ptr& v_ptr) const {return boost::dynamic_pointer_cast<LArVox::StructTensorVoxel, LArVox::Voxel>(v_ptr)->get_scalars()[LArVox::ForsterThresholdAccessor::INDEX];}
const int LArVox::ForsterThresholdAccessor::INDEX = 15;

double LArVox::HPMinEigValAccessor::operator()(const LArVox::Voxel_ptr& v_ptr) const {return boost::dynamic_pointer_cast<LArVox::StructTensorVoxel, LArVox::Voxel>(v_ptr)->get_scalars()[LArVox::HPMinEigValAccessor::INDEX];}
const int LArVox::HPMinEigValAccessor::INDEX = 16;
LArVox::ScalarComparator::ScalarComparator(LArVox::ScalarAccessor& sa) 
	: sa_(&sa)
{}
bool LArVox::ScalarComparator::operator()(const LArVox::Voxel_ptr& v1, const LArVox::Voxel_ptr& v2) {return ((*sa_)(v1) < (*sa_)(v2));}
LArVox::CrudeVoxelNonMaxSupp::CrudeVoxelNonMaxSupp(ScalarAccessor& ac)  
        : window_size_(25), threshold_(1e-3), vt_(new LArVox::v_tree(std::ptr_fun(LArVox::vpn_accessor))), sa_(&ac), scal_comp_(ac), v_std_set_(new std::set<LArVox::Voxel_ptr, LArVox::ScalarComparator>(scal_comp_))
{}
LArVox::CrudeVoxelNonMaxSupp::~CrudeVoxelNonMaxSupp()
{
	delete vt_;
	delete v_std_set_;
}
void LArVox::CrudeVoxelNonMaxSupp::SetThreshold(double thresh) {threshold_ = thresh;}
void LArVox::CrudeVoxelNonMaxSupp::SetWindowSize(double win_size) {window_size_ = win_size;}
std::vector<TLorentzVector>  LArVox::CrudeVoxelNonMaxSupp::operator()(LArVox::VoxelSet& v_set) {
	
	std::vector<TLorentzVector> feature_voxels;
	LArVox::VoxelSet::index<LArVox::lex_xyz>::type& lex_index = v_set.get<LArVox::lex_xyz>();		
	LArVox::VoxelSet::index<lex_xyz>::type::iterator vset_iter = lex_index.begin();
	while(vset_iter != lex_index.end()) {
		if((*sa_)(*vset_iter) > threshold_) {
			v_std_set_->insert(*vset_iter);
			vt_->insert(Voxel_ptr_node(*vset_iter));
		}	
		++vset_iter;
		//if(v_std_set_->size() > 100) break;
	}
	//std::cout << " v_std_set_.size() = " << v_std_set_->size() <<  std::endl;
	//std::cout << "entering  while(std_set_iter != v_std_set_->rend()) " << std::endl;
	std::set<LArVox::Voxel_ptr, LArVox::ScalarComparator>::reverse_iterator std_set_iter = v_std_set_->rbegin();
	std::set<LArVox::Voxel_ptr, LArVox::ScalarComparator>::iterator std_set_iter_duplicate;
	int count1 = 0;
	while(std_set_iter != v_std_set_->rend()) {
		std::set<LArVox::Voxel_ptr, LArVox::ScalarComparator>::reverse_iterator std_set_iter_temp = std_set_iter;
		//std::cout << count1 << std::endl;
		//std::cout << "**std_set_iter" << std::endl;
		//std::cout << **std_set_iter << std::endl;
		//std::cout << "done" << std::endl;
		//std::cout << "feature_voxels.push_back(*std_set_iter)" << std::endl;
		TLorentzVector temp_lor = vox_ptr_to_lorvec(*std_set_iter);
		feature_voxels.push_back(temp_lor);
		//std::cout << "done" << std::endl;
		LArVox::Voxel_ptr_node vpn_temp(*std_set_iter);
                std::vector<LArVox::Voxel_ptr_node>* vpn_vec = new std::vector<LArVox::Voxel_ptr_node>();
		//std::cout << "find_within_range(vpn_temp, window_size_, std::back_inserter(*vpn_vec));" << std::endl;
                vt_->find_within_range(vpn_temp, window_size_, std::back_inserter(*vpn_vec));
		//std::cout << "done" << std::endl;
		std::vector<LArVox::Voxel_ptr_node>::const_iterator vpn_iter = vpn_vec->begin();
		int count2 = 0;
		while(vpn_iter != vpn_vec->end()) {
			//std::cout << "count2 = " << count2 << std::endl;
			//std::cout << "size vpn_vec = " << vpn_vec->size() << std::endl;
			std_set_iter_duplicate = v_std_set_->find(vpn_iter->get_voxel_ptr());
			///if(std_set_iter_duplicate != v_std_set_->end()) std::cout << "*(std_set_iter_duplicate) = " << **std_set_iter_duplicate << std::endl;
			//else {std::cout << "there is an element in vt_ which is not in v_std_set_" <<std::endl; }
			if((*std_set_iter_duplicate) == (*std_set_iter)) {
				//std::cout << "matched (*std_set_iter_duplicate) == (*std_set_iter)" << std::endl; //++std_set_iter; 
			} 
			else { 
				//std::cout << "no match - deleting element" << std::endl;
				v_std_set_->erase(std_set_iter_duplicate);
			}
			//std::cout << "vt_->size() " << vt_->size() << std::endl;
			vt_->erase(*vpn_iter);
			//std::cout << "vt_->size() " << vt_->size() << std::endl;
//			vt_->optimise();
			//std::cout << "v_std_set->size() = " << v_std_set_->size() << std::endl;
			++vpn_iter;
			++count2;
		}
		delete vpn_vec;
		
		++std_set_iter;
		if(std_set_iter !=  v_std_set_->rend()) {
			//std::cout << "next elementof set is " << std::endl << **std_set_iter;
			
		}
		//else std::cout << "std_set_iter is at rend" << std::endl;
		//std::cout << "std_set_iter_temp is " << std::endl << **std_set_iter_temp ;
//		std::cout << "convert to base " << std::endl << **((++std_set_iter_temp).base());
		v_std_set_->erase((++std_set_iter_temp).base());
		//std::cout << "erased current element,  v_std_set->size() = " << v_std_set_->size() << std::endl;
		std_set_iter = v_std_set_->rbegin();

		//std::cout << "check for consistency after erasure " << std::endl;

		if(std_set_iter !=  v_std_set_->rend()) {
                        //std::cout << "next elementof set is " << std::endl; 
			//std::cout << **std_set_iter;
                        
                }
                //else std::cout << "std_set_iter is at rend" << std::endl;

		count1++;

	}
	std::cout << "feature_voxels.size() = " << feature_voxels.size() << std::endl;
	return feature_voxels;
}

TLorentzVector LArVox::CrudeVoxelNonMaxSupp::vox_ptr_to_lorvec(const Voxel_ptr& vox) {
	boost::shared_ptr<StructTensorVoxel> stvox = boost::dynamic_pointer_cast<StructTensorVoxel, Voxel>(vox);
	TLorentzVector lorvec(stvox->get_x(), stvox->get_y(), stvox->get_z(), stvox->get_weight());
	return lorvec;
}
	  
