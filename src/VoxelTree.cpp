#include <cmath>
#include "VoxelTree.hpp"

std::ostream& LArVox::operator<<(std::ostream& os,const Voxel_ptr_node& vpn)
{
        return vpn.println(os);
}

LArVox::Voxel_ptr_node::Voxel_ptr_node(const Voxel_ptr& vp)
: vox_ptr_(vp)
{}
LArVox::Voxel_ptr_node::Voxel_ptr_node(const Voxel_ptr_node & vpn)
: vox_ptr_(vpn.vox_ptr_)
{}
LArVox::Voxel_ptr_node::~Voxel_ptr_node()
{}
double LArVox::Voxel_ptr_node::distance_to(Voxel_ptr_node const& x) const
{
     double dist = 0;
     for (int i = 0; i != 3; ++i)
        dist += ((*this)[i]-x[i])*((*this)[i]-x[i]);
     return std::sqrt(dist);
}
LArVox::Voxel_ptr LArVox::Voxel_ptr_node::get_voxel_ptr(void) const
{
	return vox_ptr_;
}	
std::ostream& LArVox::Voxel_ptr_node::println(std::ostream& os) const 
{ 
      os<<*(this->vox_ptr_);
        return os;
}


