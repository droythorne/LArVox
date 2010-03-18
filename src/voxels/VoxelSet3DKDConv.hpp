#ifndef VOXEL_VOXELSET3DKDCONV_HPP_
#define VOXEL_VOXELSET3DKDCONV_HPP_
#include <iostream>
#include <iterator>
#include <boost/math/distributions/normal.hpp>
#include "tnt_array2d.h"
#include "tnt_array2d_utils.h"
#include "jama_eig.h"
#include "voxels/VoxelTree.hpp"
#include "voxels/VoxelSet.hpp"
namespace LArVox {

class GaussianKernel {
	public:
		GaussianKernel(double std_dev);
		double operator()(double dist);
	private:
		boost::math::normal normal_;
};
template <typename F> 
class VoxelSet3DKDConv { 
        public: 
                VoxelSet3DKDConv(F& kernel, double range) 
                : kernel_(kernel), vt_(std::ptr_fun(vpn_accessor)), range_(range)
                {} 
                VoxelSet& operator()(VoxelSet& v_set) {
			VoxelSet::index<lex_xyz>::type& lex_index = v_set.get<lex_xyz>();
        		VoxelSet::index<lex_xyz>::type::iterator iter = lex_index.begin();
        		while(iter != lex_index.end()) {
       				vt_.insert(Voxel_ptr_node(*iter));
				iter++;
			}
			iter= lex_index.begin();
			while(iter != lex_index.end()) {
                		TNT::Array2D<double> temp_st(3,3,0.0);
		        	calc_st(temp_st, iter);        
				Voxel_ptr st_vox = Voxel_ptr(new StructTensorVoxel(*(boost::dynamic_pointer_cast<StructTensorVoxel, Voxel>(*iter)), temp_st));
                		lex_index.replace(iter,st_vox);
	                	++iter;
        			//make allocation of temp_st to v_set here
        		}       
        		return v_set;
		}

        private: 
		int calc_st(TNT::Array2D<double>& st,  VoxelSet::index<lex_xyz>::type::iterator iter) {
			Voxel_ptr_node vpn_temp(*iter);
			std::vector<Voxel_ptr_node> vpn_vec;
			vt_.find_within_range(vpn_temp, range_, std::back_inserter(vpn_vec));
		
		vpn_vec.push_back(vpn_temp);
		std::vector<Voxel_ptr_node>::const_iterator vpn_vec_it = vpn_vec.begin();
		double kernel_element, xy_elt, xz_elt, yz_elt;
		
		while(vpn_vec_it != vpn_vec.end()) {
                kernel_element = kernel_(vpn_temp.distance_to(*vpn_vec_it));
                grad temp_grad = boost::static_pointer_cast<StructTensorVoxel, Voxel>(vpn_vec_it->get_voxel_ptr())->get_gradient();
                for(int i=1; i<3; i++) {
                        if(temp_grad[i].second == false) return -1;
                }
                st[0][0] += kernel_element * temp_grad[0].first * temp_grad[0].first;
                st[1][1] += kernel_element * temp_grad[1].first * temp_grad[1].first;
                st[2][2] += kernel_element * temp_grad[2].first * temp_grad[2].first;

                xy_elt = kernel_element * temp_grad[0].first * temp_grad[1].first;
                xz_elt = kernel_element * temp_grad[0].first * temp_grad[2].first;
                yz_elt = kernel_element * temp_grad[1].first * temp_grad[2].first;
                st[0][1] += xy_elt; st[1][0] += xy_elt;
                st[1][2] += yz_elt; st[2][1] += yz_elt;
                st[0][2] += xz_elt; st[2][0] += xz_elt;

                ++vpn_vec_it;
        	}
}
			
                TNT::Array2D<double>& scalar_mult(double scalar, TNT::Array2D<double>& matrix);
		v_tree vt_;
                F& kernel_; 
		double range_;
};
} //namespace LArVox

#endif //VOXEL_VOXELSET3DKDCONV_HPP_
