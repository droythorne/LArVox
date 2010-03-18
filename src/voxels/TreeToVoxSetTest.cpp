#include "./TreeToVoxSet.hpp"
#include "voxels/Voxel.hpp"
#include "voxels/VoxelSet.hpp"
#include "voxels/VoxelSetTransforms.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <iterator>
#include <vector>
#include <boost/math/special_functions/erf.hpp>

static const double ERF_CUTOFF = 4.0;
int write_to_file(LArVox::VoxelSet * vset_ptr, std::string filename) {

	LArVox::VoxelSet::index<LArVox::lex_xyz>::type& index = vset_ptr->get<LArVox::lex_xyz>();
        LArVox::VoxelSet::index<LArVox::lex_xyz>::type::iterator vox_iter = index.begin();
        std::ofstream os;
        os.open(filename.data(), std::ios::app);
        os << "# " << filename << " - voxel coords (x, y, z), evals and e_vecs" << std::endl;
        while(vox_iter != index.end()) {
                os << **vox_iter; 
	
                ++vox_iter;
        }
        os.close();
	return 0;
}
std::vector<double> populate_kernel(int kernel_size, double std_dev, double voxel_size) {

//Insist on an odd size
        assert((kernel_size / 2)*2 != kernel_size);
        std::vector<double> kernel(kernel_size, 0.0);
        double erf_max = (0.5 * voxel_size * kernel_size) / std_dev;
        assert(erf_max < ERF_CUTOFF);
        double normalisation = 1.0 / boost::math::erf<double>(erf_max);
        kernel[kernel_size/2] = normalisation * boost::math::erf<double>(erf_max / kernel_size);
        for(int i = 1; i <= kernel_size/2; i++) {
                kernel[kernel_size/2 + i] = kernel[kernel_size/2 - i] = 0.5*normalisation*(boost::math::erf<double>((2*i+1)* erf_max / kernel_size) - boost::math::erf<double>((2*i-1)* erf_max / kernel_size));
        }
        return kernel;
}

int main(int argc, char** argv) 
{

	std::string file_name("/storage/epp1/phraar/Documents/Programs/Voxels/events/genie-770MeVQELCC.root");
	std::string tree_name("genie_qel_run");
	std::string output_name("default.dat");
	std::string output_name2("default2.dat");
	std::string output_name3("gradients.dat");
/*	if(argc >= 1) {
		file_name.assign(argv[1]);
		if(argc >= 2) { 
			tree_name.assign(argv[2]);
			if(argc >= 3) output_name.assign(argv[3]);
		}
	}	
*/	std::string branch_name("hits");

	LArVox::TreeToVoxSet t2vox(file_name, tree_name, branch_name);
	LArVox::VoxelSet * vset_ptr = t2vox.getNextEvent();
	LArVox::VoxelSet_out_by<LArVox::lex_xyz>(*vset_ptr);
	
	LArVox::VoxelSet v_set_gradients(*vset_ptr);
	write_to_file(vset_ptr, output_name);
	std::vector<double> kernel = populate_kernel(3, 0.5, 1);
	LArVox::VoxelSet3DConvConcrete* three_d_conv = new LArVox::VoxelSet3DConvConcrete(kernel);
	(*three_d_conv)(*vset_ptr);
	write_to_file(vset_ptr, output_name2);
	LArVox::VoxelSetDerivativeSymmDiff* test_grad = new LArVox::VoxelSetDerivativeSymmDiff();
	(*test_grad)(*vset_ptr, v_set_gradients);
	write_to_file(&v_set_gradients, output_name3);

	return 0;
}
