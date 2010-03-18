#ifndef KDRECON_HARRISPLESSEY_HPP 
#define KDRECON_HARRISPLESSEY_HPP

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include "voxels/VoxelSet.hpp"

namespace KDRecon {
	
class HarrisPlessey {
	public:
		HarrisPlessey(int K_SIZE);
		void operator()(LArVox::VoxelSet& v_set, double vox_size, double t, double gamma);
		
		
	private:
		// Hold a weak ptr to the current event
		// so we don't cause a leak if operator()
		// returns before reset() is called
		// but we can still access the event from
		// other member functions
		void calc_struct_tensor( LArVox::VoxelSet& v_set, double vox_size, double t, double gamma);
		std::vector<double> populate_kernel(int kernel_size, double std_dev, double voxel_size);
		void append_evecs(LArVox::VoxelSet::index<LArVox::lex_xyz>::type& index);
		int K_SIZE_;
		static const double ERF_CUTOFF;
		static const double TINY_NUM;

};

} // namespace KDRecon

#endif
