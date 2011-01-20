
#include "TVoxel.hpp"

//ClassImp(LArVox::TVoxel)
LArVox::TVoxel::TVoxel() : TObject(), vox_coords_()
{}      
LArVox::TVoxel::TVoxel(const TLorentzVector& lvec)
        : TObject(), vox_coords_(lvec)
{} 
LArVox::TVoxel::TVoxel(const TLorentzVector& lvec, const TArrayD& scalars)
	: TObject(), vox_coords_(lvec), scalars_(scalars)
{}
LArVox::TVoxel::~TVoxel()
{}

