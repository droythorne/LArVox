#ifndef SERIALISE_TVOXEL_HPP_
#define SERIALISE_TVOXEL_HPP_
#include "TArrayD.h"
#include "TLorentzVector.h"
#include "TClass.h"
#include "TObject.h"
#include "Riostream.h"
namespace LArVox {
class TVoxel : public TObject {
        public:
                TVoxel();
                TVoxel(const TLorentzVector& lvec);
		TVoxel(const TLorentzVector& lvec, const TArrayD& scalars); 
                virtual ~TVoxel();
                TLorentzVector vox_coords_;
		TArrayD scalars_;
                ClassDef(TVoxel, 1)
};              
} //namespace LArVox
#endif //SERIALISE_TVOXEL_HPP_
