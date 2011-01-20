#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <boost/math/special_functions/erf.hpp>
#include "tnt_126/tnt.h"
#include "jama125/jama_eig.h"

#include "Voxel.hpp"
#include "VoxelSet.hpp"
#include "VoxelSetTransforms.hpp"
#include "VoxelSet3DKDConv.hpp"

#include "HarrisPlessey.hpp"

KDRecon::HarrisPlessey::HarrisPlessey(int K_SIZE) 
: K_SIZE_(K_SIZE)  
{}
void KDRecon::HarrisPlessey::operator()(LArVox::VoxelSet& v_set, double vox_size, double t, double gamma)
{
	calc_struct_tensor(v_set, vox_size, t, gamma);
	
	
}
std::vector<double>  KDRecon::HarrisPlessey::populate_kernel(int kernel_size, double std_dev, double voxel_size) {

//Insist on an odd size
        //assert((kernel_size / 2)*2 != kernel_size);
        std::vector<double> kernel(kernel_size, 0.0);
        double erf_max = (0.5 * voxel_size * kernel_size) / (std_dev * std_dev);
        assert(erf_max < ERF_CUTOFF);
        double normalisation = 1.0 / boost::math::erf<double>(erf_max);
        kernel[kernel_size/2] = normalisation * boost::math::erf<double>(erf_max / kernel_size);
        for(int i = 1; i <= kernel_size/2; i++) {
                kernel[kernel_size/2 + i] = kernel[kernel_size/2 - i] = 0.5*normalisation*(boost::math::erf<double>((2*i+1)* erf_max / kernel_size) - boost::math::erf<double>((2*i-1)* erf_max / kernel_size));
        }
        return kernel;
}
void KDRecon::HarrisPlessey::calc_struct_tensor(LArVox::VoxelSet& v_set, double vox_size, double t, double gamma) {
	std::vector<double> kernel_in = populate_kernel(K_SIZE_, t, vox_size);
	LArVox::VoxelSet3DConvConcrete* three_d_conv = new LArVox::VoxelSet3DConvConcrete(kernel_in);

        LArVox::VoxelSetDerivativeSymmDiff* test_grad = new LArVox::VoxelSetDerivativeSymmDiff();
	LArVox::VoxelSet v_set_temp(v_set);
	(*three_d_conv)(v_set_temp);
	//std::cout << "# LArVox::VoxelSet_out_by<LArVox::lex_xyz>(v_set_temp)" << std::endl;
	//LArVox::VoxelSet_out_by<LArVox::lex_xyz>(v_set_temp);
	(*test_grad)(v_set_temp, v_set);
	//std::cout << "# LArVox::VoxelSet_out_by<LArVox::lex_xyz>(v_set_temp)" << std::endl;
        //LArVox::VoxelSet_out_by<LArVox::lex_xyz>(v_set_temp);
	//std::cout << "# LArVox::VoxelSet_out_by<LArVox::lex_xyz>(v_set)" << std::endl;
        //LArVox::VoxelSet_out_by<LArVox::lex_xyz>(v_set);

	double s = gamma*gamma*t;
	LArVox::GaussianKernel gk(s/vox_size);
   	LArVox::VoxelSet3DKDConv<LArVox::GaussianKernel> vs_kd(gk, ERF_CUTOFF * s / vox_size);

	vs_kd(v_set);
	//std::cout << "# LArVox::VoxelSet_out_by<LArVox::lex_xyz>(v_set) after vs_kd(v_set)" << std::endl;
        //LArVox::VoxelSet_out_by<LArVox::lex_xyz>(v_set);

        LArVox::VoxelSet::index<LArVox::lex_xyz>::type& index = v_set.get<LArVox::lex_xyz>();
	append_evecs(index);
//	output_evecs(index);	
	delete test_grad;
	delete three_d_conv;
}

void KDRecon::HarrisPlessey::append_evecs(LArVox::VoxelSet::index<LArVox::lex_xyz>::type& index) {
        LArVox::VoxelSet::index<LArVox::lex_xyz>::type::iterator eig_iter = index.begin();
	std::cout << "#KDRecon::HarrisPlessey::append_evecs(LArVox::VoxelSet::index<LArVox::lex_xyz>::type& index) " << std::endl;
 	//std::ofstream os;
        //os.open(filename_.data(), ios::app);
        //os << "# " << filename_ << " - voxel coords (x, y, z), evals and e_vecs" << std::endl;
        while(eig_iter != index.end()) {
		//os << (*eig_iter)->get_x() << " " << (*eig_iter)->get_y() << " " << (*eig_iter)->get_z() << " " << (boost::dynamic_pointer_cast<LArVox::StructTensorVoxel, LArVox::Voxel>(*eig_iter))->get_weight() << " ";	
                TNT::Array2D<double> eig_st = (boost::dynamic_pointer_cast<LArVox::StructTensorVoxel, LArVox::Voxel>(*eig_iter))->get_st();
                JAMA::Eigenvalue<double> eig_calc(eig_st);
                TNT::Array1D<double> evals;
                eig_calc.getRealEigenvalues(evals);
                TNT::Array2D<double> evecs;
                eig_calc.getV(evecs);
		std::vector<double>& scalar_vec = (boost::dynamic_pointer_cast<LArVox::StructTensorVoxel, LArVox::Voxel>(*eig_iter))->modify_scalars();
                for(int i = 0; i<3; i++) {
			scalar_vec.push_back(evals[i]);
		}
		
                for(int i = 0; i<3; i++) {
                	for(int j = 0; j<3; j++) {
				scalar_vec.push_back(evecs[j][i]);
			}
		}
		//Now calculate the Harris-Plessey type functions
		double sum_evals = evals[0] + evals[1] + evals[2];
		
		// 1. Forstner (from evals)
		
		double forstner;
		std::fabs(sum_evals) > TINY_NUM ? forstner = (evals[0] * evals[1] * evals[2])/sum_evals : forstner = 0.0;
		scalar_vec.push_back(forstner);
		// 2. Forstner (from struct tensor)
		double st_det = eig_st[0][0]*(eig_st[1][1]*eig_st[2][2] - eig_st[1][2]*eig_st[2][1]) - eig_st[0][1]*(eig_st[1][0]*eig_st[2][2] - eig_st[1][2]*eig_st[2][0]) + eig_st[0][2]*(eig_st[1][0]*eig_st[2][1] - eig_st[1][1]*eig_st[2][0]);
		double trace_st = (eig_st[0][0]+eig_st[1][1]+eig_st[2][2]);
		std::fabs(trace_st) > TINY_NUM ? forstner = st_det / trace_st : forstner = 0.0;
		scalar_vec.push_back(forstner);
		// 3. Forstner threshold (from evals)
		std::fabs(sum_evals) > TINY_NUM ? forstner = (evals[0] * evals[1] * evals[2])/ std::pow((0.333*sum_evals), 3) : forstner = 0.0; 	
		scalar_vec.push_back(forstner);
		// 4. Forstner threshold (from structure tensor)
		std::fabs(trace_st) > TINY_NUM ? forstner = st_det / std::pow(0.333*trace_st, 3) : forstner = 0.0;
		scalar_vec.push_back(forstner);
		double min_eval = std::min(evals[0], std::min(evals[1],evals[2]));
		scalar_vec.push_back(min_eval);
		//os << std::endl;

                ++eig_iter;
        }
//	os.close();
	std::cout << "Exiting KDRecon::HarrisPlessey::append_evecs" << std::endl;
}

const double KDRecon::HarrisPlessey::ERF_CUTOFF = 5.0;
const double KDRecon::HarrisPlessey::TINY_NUM = 1.0e-20;
