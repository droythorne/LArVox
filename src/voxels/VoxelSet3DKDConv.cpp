#include "voxels/VoxelSet3DKDConv.hpp"

LArVox::GaussianKernel::GaussianKernel(double std_dev)
: normal_(0.0, std_dev)
{}

double LArVox::GaussianKernel::operator()(double dist)
{
	return boost::math::pdf(normal_, dist);
}
	
