#ifndef VOXEL_VOXELSETTRANSFORMS_HPP
#define VOXEL_VOXELSETTRANSFORMS_HPP

#include <string>
#include <vector>
#include "voxels/VoxelSet.hpp"
#include "tnt_array2d.h"
namespace LArVox {

enum Scan {WRITE_ALL, WRITE_SELECTION}; 
enum Axis {X_AXIS = 0, Y_AXIS, Z_AXIS};
class VoxelSetTransform {

	public :
		VoxelSetTransform();
		VoxelSetTransform(const std::string& transform_name);
		virtual ~VoxelSetTransform();
		virtual VoxelSet& operator()(VoxelSet&) = 0;

	protected :

		const std::string transform_name_;
};
class VoxelSetConv : public VoxelSetTransform {

	public :
		VoxelSetConv(const std::vector<double>& kernel);
		VoxelSetConv(const std::string& transform_name, const std::vector<double>& kernel);
		virtual ~VoxelSetConv();
		virtual VoxelSet& operator()(VoxelSet&) = 0;

	protected :
		
		struct change_weight {
		       		change_weight(const double new_weight) 
				: new_weight(new_weight) 
				{}
				void operator() (Voxel_ptr& v);
			private :
        			double new_weight;
		};
		const std::vector<double>& kernel_;	
};
class VoxelSetConvYConcrete : public VoxelSetConv {

	public :
		VoxelSetConvYConcrete(const std::vector<double>& kernel);
                VoxelSetConvYConcrete(const std::string& transform_name, const std::vector<double>& kernel);
                virtual ~VoxelSetConvYConcrete();
                virtual VoxelSet& operator()(VoxelSet& v_set);

};
class VoxelSetConvZConcrete : public VoxelSetConv {

	public :
		VoxelSetConvZConcrete(const std::vector<double>& kernel);
                VoxelSetConvZConcrete(const std::string& transform_name, const std::vector<double>& kernel);
                virtual ~VoxelSetConvZConcrete();
                virtual VoxelSet& operator()(VoxelSet& v_set);

};
class VoxelSetConvXConcrete : public VoxelSetConv {

	public :
		VoxelSetConvXConcrete(const std::vector<double>& kernel);
                VoxelSetConvXConcrete(const std::string& transform_name, const std::vector<double>& kernel);
                virtual ~VoxelSetConvXConcrete();
                virtual VoxelSet& operator()(VoxelSet& v_set);

};
class VoxelSet3DConvConcrete : public VoxelSetConv {

	public :
		VoxelSet3DConvConcrete(const std::vector<double>& kernel);
		VoxelSet3DConvConcrete(const std::string& transform_name, const std::vector<double>& kernel);
		virtual ~VoxelSet3DConvConcrete();
		virtual VoxelSet& operator()(VoxelSet& v_set);

	private :
		std::vector<VoxelSetConv* > convolutions_;
		void initialise_convolutions();
};
class VoxelSet3DConvWindow : public VoxelSetConv {
	public:
		VoxelSet3DConvWindow(const std::vector<double>& kernel);
		VoxelSet3DConvWindow(const std::string& transform_name, const std::vector<double>& kernel);
                virtual ~VoxelSet3DConvWindow();
                virtual VoxelSet& operator()(VoxelSet& v_set);

        private :
                void initialise_convolution();
		int x_conv_window(VoxelSet::index<comp_yzx>::type::iterator x_iter, VoxelSet& vox_set, TNT::Array2D<double>& st);
		int y_conv_window(VoxelSet::index<comp_xzy>::type::iterator y_iter, VoxelSet& vox_set, TNT::Array2D<double>& st);
		int z_conv_window(VoxelSet::index<comp_xyz>::type::iterator z_iter, VoxelSet& vox_set, TNT::Array2D<double>& st);
		TNT::Array2D<double>& scalar_mult(double scalar, TNT::Array2D<double>& matrix);
		TNT::Array2D<double> temp_struct_tensor; //unused at present (remove?)
		
};
class VoxelSetDerivativeInterface : public VoxelSetTransform {

	public :
		VoxelSetDerivativeInterface();
		VoxelSetDerivativeInterface(const std::string& transform_name);
		virtual ~VoxelSetDerivativeInterface();
		virtual VoxelSet& operator()(VoxelSet& v_set) = 0;
		virtual VoxelSet& operator()(VoxelSet& v_set_in, VoxelSet& v_set_out) = 0;
		
	protected :
	 
		struct change_gradient{
                                change_gradient(const grad new_gradient)
                                : new_gradient_(new_gradient)
                                {}
                                void operator() (Voxel_ptr& v);
                        private :
                                grad new_gradient_;
                };

};
class VoxelSetDerivativeSymmDiff : public VoxelSetDerivativeInterface {
	public :
                VoxelSetDerivativeSymmDiff();
                VoxelSetDerivativeSymmDiff(const std::string& transform_name);
                virtual ~VoxelSetDerivativeSymmDiff();
                virtual VoxelSet& operator()(VoxelSet& v_set);
                virtual VoxelSet& operator()(VoxelSet& v_set_in_scalar, VoxelSet& v_set_out_tensor);
	private:
		
			//v_set_out_tensor.insert(Voxel_ptr(new TensorVoxel((*vox_iter_traverse)->get_x(), (*vox_iter_traverse)->get_y(), (*vox_iter_traverse)->get_z, temp_grad)))
		
		template<typename Tag> std::pair<double, bool> calc_partial(equality eq, coord c, typename VoxelSet::index<Tag>::type::const_iterator vox_iter_in, typename VoxelSet::index<Tag>::type& lex_index) {
			// Responsibility of the caller to not pass VoxelSet::index<Tag>::type X.begin() to this function - no bound checking carried out when computing and using --vox_iter_in.
			double partial;
			std::pair<double, bool> temp_pair(0.0, false);
		        typename VoxelSet::index<Tag>::type::const_iterator vox_iter_prv, vox_iter_nxt, vox_end = lex_index.end(), vox_begin = lex_index.begin();
			if(vox_iter_in != vox_end && vox_iter_in != vox_begin) { 
				vox_iter_nxt = vox_iter_in; vox_iter_prv = vox_iter_in;
				++vox_iter_nxt; --vox_iter_prv;
				if(((vox_iter_prv->get())->*eq)(**vox_iter_nxt) && (((vox_iter_prv->get())->*c)() + 2) == ((vox_iter_nxt->get())->*c)() ) {
					partial = 0.5*(boost::static_pointer_cast<StructTensorVoxel, Voxel>(*vox_iter_nxt)->get_weight() - 
						  	boost::static_pointer_cast<StructTensorVoxel, Voxel>(*vox_iter_prv)->get_weight());
					temp_pair.first = partial;
					temp_pair.second = true;
				}
			}
			
			return temp_pair;
		}
};
} //namespace LArVox
#endif //VOXEL_VOXELSETTRANSFORMS_HPP
