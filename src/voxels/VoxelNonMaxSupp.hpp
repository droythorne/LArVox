#ifndef V0XELS_VOXELNONMAXSUPP_HPP_
#define V0XELS_VOXELNONMAXSUPP_HPP_

#include <set>
#include <vector>
#include "TLorentzVector.h"
#include "voxels/Voxel.hpp"
#include "voxels/VoxelSet.hpp"
#include "voxels/VoxelTree.hpp"

namespace LArVox {
class ScalarAccessor {
	public:
		virtual double operator()(const Voxel_ptr& v_ptr) const = 0;
};
class ForsterAccessor : public ScalarAccessor {
	public:
		double operator()(const Voxel_ptr& v_ptr) const;
	private:
		static const int INDEX;
};
class ForsterThresholdAccessor : public ScalarAccessor {
	public:
                double operator()(const Voxel_ptr& v_ptr) const;
        private:
                static const int INDEX;
};
class HPMinEigValAccessor : public ScalarAccessor {
	public:
                double operator()(const Voxel_ptr& v_ptr) const;
        private:
                static const int INDEX;
};
class ScalarComparator {
	public:
		ScalarComparator(ScalarAccessor& sa);
		
		bool operator()(const Voxel_ptr& v1, const Voxel_ptr& v2); 
	private:
		ScalarAccessor* sa_;
};
class VoxelNonMaxSuppInterface {
	public:

		virtual void SetThreshold(double thresh) = 0;
		virtual void SetWindowSize(double win_size) = 0;
		virtual std::vector<TLorentzVector> operator()(VoxelSet& v_set) = 0;
};
class CrudeVoxelNonMaxSupp : public VoxelNonMaxSuppInterface {
	public:
		CrudeVoxelNonMaxSupp(ScalarAccessor& ac);
		~CrudeVoxelNonMaxSupp();
                virtual void SetThreshold(double thresh);
                virtual void SetWindowSize(double win_size);
                virtual std::vector<TLorentzVector> operator()(VoxelSet& v_set);

	private:

		double window_size_;
		double threshold_;
		v_tree *vt_;
		ScalarAccessor* sa_;
		ScalarComparator scal_comp_;
		std::set<Voxel_ptr, ScalarComparator> *v_std_set_;
		
		TLorentzVector vox_ptr_to_lorvec(const Voxel_ptr& vox_ptr);	
};
} // namespace LArVox
#endif //VOXELS_NONMAXSUPP_HPP_
