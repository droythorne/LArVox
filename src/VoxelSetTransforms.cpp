#include <cstdio> 
#include <iterator>
#include <list>
#include <cmath>
#include <iostream>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "VoxelSetTransforms.hpp"
#include "tnt_126/tnt_array2d_utils.h"

using LArVox::VoxelSet;

LArVox::VoxelSetTransform::VoxelSetTransform()
: transform_name_("default") 
{}
LArVox::VoxelSetTransform::VoxelSetTransform(const std::string& transform_name)
: transform_name_(transform_name) 
{}
LArVox::VoxelSetTransform::~VoxelSetTransform()
{}
LArVox::VoxelSetConv::VoxelSetConv(const std::vector<double>& kernel)
: VoxelSetTransform(), kernel_(kernel)
{}
LArVox::VoxelSetConv::VoxelSetConv(const std::string& transform_name, const std::vector<double>& kernel)
:  VoxelSetTransform(transform_name), kernel_(kernel)
{}
LArVox::VoxelSetConv::~VoxelSetConv()
{}
void LArVox::VoxelSetConv::change_weight::operator()(Voxel_ptr& v) {
	boost::static_pointer_cast<StructTensorVoxel, Voxel>(v)->set_weight(new_weight);
}

LArVox::VoxelSetConvZConcrete::VoxelSetConvZConcrete(const std::vector<double>& kernel)
: VoxelSetConv(kernel)
{}
LArVox::VoxelSetConvZConcrete::VoxelSetConvZConcrete(const std::string& transform_name, const std::vector<double>& kernel)
:  VoxelSetConv(transform_name, kernel)
{}
LArVox::VoxelSetConvZConcrete::~VoxelSetConvZConcrete()
{}
VoxelSet& LArVox::VoxelSetConvZConcrete::operator()(VoxelSet& in_v_set) {
	std::cout << "performing ConvZ" << std::endl;
	int i;
        int coord_interval;
        double weight_temp;
        bool replace_flag;
        int K_SIZE = kernel_.size();
        VoxelSet::index<lex_xyz>::type& lex_index = in_v_set.get<lex_xyz>();
        VoxelSet::index<lex_xyz>::type::iterator vox_iter1 = lex_index.begin();
        VoxelSet::index<lex_xyz>::type::iterator vox_iter2 = vox_iter1;
        VoxelSet::index<lex_xyz>::type::iterator current_vox_iter;
        std::list<boost::tuple<short, double, VoxelSet::index<lex_xyz>::type::iterator> > k_index;
        std::list<boost::tuple<short, double, VoxelSet::index<lex_xyz>::type::iterator> >::iterator k_index_iter;

	 while(vox_iter1 != lex_index.end()) {
		++vox_iter2;
		coord_interval = (vox_iter2 == lex_index.end()) ? K_SIZE : ( ((*vox_iter1)->xy_equal(**vox_iter2)) ? std::min((*vox_iter2)->get_z() - (*vox_iter1)->get_z(), K_SIZE) : K_SIZE );
		k_index.push_front(boost::make_tuple(0, boost::static_pointer_cast<StructTensorVoxel, Voxel>(*vox_iter1)->get_weight(), vox_iter1));
                for(i=0; i<coord_interval; i++) {
			k_index_iter = k_index.begin();
	                weight_temp = 0;
                        replace_flag = false;
                        while(k_index_iter != k_index.end() && ( (k_index_iter->get<0>() + i) < K_SIZE) ) {
                                weight_temp += kernel_[k_index_iter->get<0>() + i] * (k_index_iter->get<1>());
                                if(k_index_iter->get<0>() + i == K_SIZE/2) {
					replace_flag = true;
                                        current_vox_iter = k_index_iter->get<2>();
                                }
                                k_index_iter++;
                        }

			if(replace_flag) lex_index.modify(current_vox_iter, change_weight(weight_temp));
        	        else { 
			        if (i > K_SIZE/2) {
                        	                lex_index.insert(Voxel_ptr(new StructTensorVoxel((*vox_iter1)->get_x(), (*vox_iter1)->get_y(), (*vox_iter1)->get_z() + 1, weight_temp)));
                                	        ++vox_iter1;
	                        }
        	                else {
                	        	lex_index.insert(Voxel_ptr(new StructTensorVoxel((*vox_iter1)->get_x(), (*vox_iter1)->get_y(), (*vox_iter1)->get_z() - (K_SIZE/2) + i, weight_temp)));
                        	}
			}
		}
                
		k_index_iter = k_index.begin();
                while (k_index_iter != k_index.end()) {
                        if(k_index_iter->get<0>() + coord_interval < K_SIZE) {
                                k_index_iter->get<0>() += coord_interval;
                                ++k_index_iter;
                        }
                        else {
                                k_index_iter = k_index.erase(k_index_iter);
                        }
                }

                ++vox_iter1;
        }

        return in_v_set;
}

LArVox::VoxelSetConvYConcrete::VoxelSetConvYConcrete(const std::vector<double>& kernel)
: VoxelSetConv(kernel)
{}
LArVox::VoxelSetConvYConcrete::VoxelSetConvYConcrete(const std::string& transform_name, const std::vector<double>& kernel)
:  VoxelSetConv(transform_name, kernel)
{}
LArVox::VoxelSetConvYConcrete::~VoxelSetConvYConcrete()
{}
VoxelSet& LArVox::VoxelSetConvYConcrete::operator()(VoxelSet& in_v_set) {
	
	std::cout << "Perfoming ConvY" << std::endl;
	int i;
        int coord_interval;
        double weight_temp;
        bool replace_flag;
        int K_SIZE = kernel_.size();
        VoxelSet::index<lex_xzy>::type& lex_index = in_v_set.get<lex_xzy>();
        VoxelSet::index<lex_xzy>::type::iterator vox_iter1 = lex_index.begin();
        VoxelSet::index<lex_xzy>::type::iterator vox_iter2 = vox_iter1;
        VoxelSet::index<lex_xzy>::type::iterator current_vox_iter;
        std::list<boost::tuple<short, double, VoxelSet::index<lex_xzy>::type::iterator> > k_index;
        std::list<boost::tuple<short, double, VoxelSet::index<lex_xzy>::type::iterator> >::iterator k_index_iter;

	 while(vox_iter1 != lex_index.end()) {
		++vox_iter2;
		coord_interval = (vox_iter2 == lex_index.end()) ? K_SIZE : ( ((*vox_iter1)->xz_equal(**vox_iter2)) ? std::min((*vox_iter2)->get_y() - (*vox_iter1)->get_y(), K_SIZE) : K_SIZE );
		k_index.push_front(boost::make_tuple(0, boost::static_pointer_cast<StructTensorVoxel, Voxel>(*vox_iter1)->get_weight(), vox_iter1));
                for(i=0; i<coord_interval; i++) {
			k_index_iter = k_index.begin();
	                weight_temp = 0;
                        replace_flag = false;
                        while(k_index_iter != k_index.end() && ( (k_index_iter->get<0>() + i) < K_SIZE) ) {
                                weight_temp += kernel_[k_index_iter->get<0>() + i] * (k_index_iter->get<1>());
                                if(k_index_iter->get<0>() + i == K_SIZE/2) {
					replace_flag = true;
                                        current_vox_iter = k_index_iter->get<2>();
                                }
                                k_index_iter++;
                        }

			if(replace_flag) lex_index.modify(current_vox_iter, change_weight(weight_temp));
        	        else { 
			        if (i > K_SIZE/2) {
                        	                lex_index.insert(Voxel_ptr(new StructTensorVoxel((*vox_iter1)->get_x(), (*vox_iter1)->get_y() + 1, (*vox_iter1)->get_z(), weight_temp)));
                                	        ++vox_iter1;
	                        }
        	                else {
                	        	lex_index.insert(Voxel_ptr(new StructTensorVoxel((*vox_iter1)->get_x(), (*vox_iter1)->get_y() - (K_SIZE/2) + i, (*vox_iter1)->get_z(), weight_temp)));
                        	}
			}
		}
                
		k_index_iter = k_index.begin();
                while (k_index_iter != k_index.end()) {
                        if(k_index_iter->get<0>() + coord_interval < K_SIZE) {
                                k_index_iter->get<0>() += coord_interval;
                                ++k_index_iter;
                        }
                        else {
                                k_index_iter = k_index.erase(k_index_iter);
                        }
                }

                ++vox_iter1;
        }

        return in_v_set;
}



LArVox::VoxelSetConvXConcrete::VoxelSetConvXConcrete(const std::vector<double>& kernel)
: VoxelSetConv(kernel)
{}
LArVox::VoxelSetConvXConcrete::VoxelSetConvXConcrete(const std::string& transform_name, const std::vector<double>& kernel)
:  VoxelSetConv(transform_name, kernel)
{}
LArVox::VoxelSetConvXConcrete::~VoxelSetConvXConcrete()
{}
VoxelSet& LArVox::VoxelSetConvXConcrete::operator()(VoxelSet& in_v_set) {

	std::cout << "Performing ConvX" << std::endl;	
	int i;
        int coord_interval;
        double weight_temp;
        bool replace_flag;
        int K_SIZE = kernel_.size();
        VoxelSet::index<lex_yzx>::type& lex_index = in_v_set.get<lex_yzx>();
        VoxelSet::index<lex_yzx>::type::iterator vox_iter1 = lex_index.begin();
        VoxelSet::index<lex_yzx>::type::iterator vox_iter2 = vox_iter1;
        VoxelSet::index<lex_yzx>::type::iterator current_vox_iter;
        std::list<boost::tuple<short, double, VoxelSet::index<lex_yzx>::type::iterator> > k_index;
        std::list<boost::tuple<short, double, VoxelSet::index<lex_yzx>::type::iterator> >::iterator k_index_iter;

	 while(vox_iter1 != lex_index.end()) {
		++vox_iter2;
		coord_interval = (vox_iter2 == lex_index.end()) ? K_SIZE : ( ((*vox_iter1)->yz_equal(**vox_iter2)) ? std::min((*vox_iter2)->get_x() - (*vox_iter1)->get_x(), K_SIZE) : K_SIZE );
		k_index.push_front(boost::make_tuple(0, boost::static_pointer_cast<StructTensorVoxel, Voxel>(*vox_iter1)->get_weight(), vox_iter1));
                for(i=0; i<coord_interval; i++) {
			k_index_iter = k_index.begin();
	                weight_temp = 0;
                        replace_flag = false;
                        while(k_index_iter != k_index.end() && ( (k_index_iter->get<0>() + i) < K_SIZE) ) {
                                weight_temp += kernel_[k_index_iter->get<0>() + i] * (k_index_iter->get<1>());
                                if(k_index_iter->get<0>() + i == K_SIZE/2) {
					replace_flag = true;
                                        current_vox_iter = k_index_iter->get<2>();
                                }
                                k_index_iter++;
                        }

			if(replace_flag) lex_index.modify(current_vox_iter, change_weight(weight_temp));
        	        else { 
			        if (i > K_SIZE/2) {
                        	                lex_index.insert(Voxel_ptr(new StructTensorVoxel((*vox_iter1)->get_x() + 1, (*vox_iter1)->get_y(), (*vox_iter1)->get_z(), weight_temp)));
                                	        ++vox_iter1;
	                        }
        	                else {
                	        	lex_index.insert(Voxel_ptr(new StructTensorVoxel((*vox_iter1)->get_x() - (K_SIZE/2) + i, (*vox_iter1)->get_y(), (*vox_iter1)->get_z(), weight_temp)));
                        	}
			}
		}
                
		k_index_iter = k_index.begin();
                while (k_index_iter != k_index.end()) {
                        if(k_index_iter->get<0>() + coord_interval < K_SIZE) {
                                k_index_iter->get<0>() += coord_interval;
                                ++k_index_iter;
                        }
                        else {
                                k_index_iter = k_index.erase(k_index_iter);
                        }
                }

                ++vox_iter1;
        }

        return in_v_set;
}

LArVox::VoxelSet3DConvConcrete::VoxelSet3DConvConcrete(const std::vector<double>& kernel)
: VoxelSetConv(kernel)
{
	initialise_convolutions();
}
LArVox::VoxelSet3DConvConcrete::VoxelSet3DConvConcrete(const std::string& transform_name, const std::vector<double>& kernel)
:  VoxelSetConv(transform_name, kernel)
{
	initialise_convolutions();
}
LArVox::VoxelSet3DConvConcrete::~VoxelSet3DConvConcrete()
{
	std::cout << "In LArVox::VoxelSet3DConvConcrete::~VoxelSet3DConvConcrete()" << std::endl;
	std::vector<VoxelSetConv* >::iterator conv_it = convolutions_.begin();
	while(conv_it != convolutions_.end()) {
		delete *conv_it;
		std::cout << "delete *conv_it" << std::endl;
		++conv_it;
	}
}
VoxelSet& LArVox::VoxelSet3DConvConcrete::operator()(VoxelSet& in_v_set) {
	
	std::vector<VoxelSetConv* >::iterator conv_it = convolutions_.begin();
	while(conv_it != convolutions_.end()) {
		(**conv_it)(in_v_set);
		++conv_it;
	}
	return in_v_set;
}
void LArVox::VoxelSet3DConvConcrete::initialise_convolutions(void) {
	convolutions_.push_back(new VoxelSetConvZConcrete(kernel_));
	convolutions_.push_back(new VoxelSetConvYConcrete(kernel_));
	convolutions_.push_back(new VoxelSetConvXConcrete(kernel_));
}
LArVox::VoxelSet3DConvWindow::VoxelSet3DConvWindow(const std::vector<double>& kernel)
: VoxelSetConv(kernel), temp_struct_tensor()
{
        initialise_convolution();
}
LArVox::VoxelSet3DConvWindow::VoxelSet3DConvWindow(const std::string& transform_name, const std::vector<double>& kernel)
:  VoxelSetConv(transform_name, kernel), temp_struct_tensor()
{
        initialise_convolution();
}
LArVox::VoxelSet3DConvWindow::~VoxelSet3DConvWindow()
{}
VoxelSet& LArVox::VoxelSet3DConvWindow::operator() (VoxelSet& v_set) {

	VoxelSet::index<comp_xyz>::type& lex_index = v_set.get<comp_xyz>();
	VoxelSet::index<comp_xyz>::type::iterator iter = lex_index.begin();
	while(iter != lex_index.end()) {
		TNT::Array2D<double> temp_st(3,3,0.0);
		std::cout << "operator() loop " << (**iter);
		int z_conv_flag = z_conv_window(iter, v_set, temp_st);
	//	std::cout << "z_conv_flag " << z_conv_flag << std::endl << temp_st;
		Voxel_ptr st_vox = Voxel_ptr(new StructTensorVoxel(*(boost::dynamic_pointer_cast<StructTensorVoxel, Voxel>(*iter)), temp_st));
		lex_index.replace(iter,st_vox);
		++iter;
	//make allocation of temp_st to v_set here
	}	
	std::cout << "Completed VoxelSet3DConvWindow::operator()" << std::endl;
	return v_set;
}
int LArVox::VoxelSet3DConvWindow::x_conv_window(VoxelSet::index<comp_yzx>::type::iterator x_iter, VoxelSet& vox_set, TNT::Array2D<double>& st) {
	
	boost::tuple<short,short,short> vox_tuple = (*x_iter)->get_coord_tuple();
	VoxelSet::index<comp_yzx>::type& lex_index = vox_set.get<comp_yzx>();
	double kernel_element, xy_elt, xz_elt, yz_elt;
	grad temp_grad;
	short vox_x_coord = vox_tuple.get<0>();
	boost::tuple<short,short,short> upp_bnd(vox_tuple.get<1>(), vox_tuple.get<2>(), vox_x_coord + kernel_.size()/2);
	boost::tuple<short,short,short> low_bnd(vox_tuple.get<1>(), vox_tuple.get<2>(), vox_x_coord - kernel_.size()/2);
	VoxelSet::index<comp_yzx>::type::iterator iter_low = lex_index.lower_bound(low_bnd); 	
	VoxelSet::index<comp_yzx>::type::iterator iter_upp = lex_index.upper_bound(upp_bnd); 	
	
	while(iter_low != iter_upp) {
		kernel_element = kernel_[(*iter_low)->get_x() - vox_x_coord + kernel_.size()/2];
		temp_grad = boost::static_pointer_cast<StructTensorVoxel, Voxel>(*iter_low)->get_gradient();
		std::cout << "in x_conv_wind loop" << std::endl;
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
		
		//std::cout << st;
		++iter_low;
	}
	return 0;
}

int LArVox::VoxelSet3DConvWindow::y_conv_window(VoxelSet::index<comp_xzy>::type::iterator y_iter, VoxelSet& vox_set, TNT::Array2D<double>& st) {
	std::cout << "in y conv window" << std::endl;
	boost::tuple<short,short,short> vox_tuple = (*y_iter)->get_coord_tuple();
	VoxelSet::index<comp_xzy>::type& lex_index = vox_set.get<comp_xzy>();
	double kernel_element;
	short vox_y_coord = vox_tuple.get<1>();
	boost::tuple<short,short,short> upp_bnd(vox_tuple.get<0>(), vox_tuple.get<2>(), vox_y_coord + kernel_.size()/2);
	boost::tuple<short,short,short> low_bnd(vox_tuple.get<0>(), vox_tuple.get<2>(), vox_y_coord - kernel_.size()/2);
	VoxelSet::index<comp_xzy>::type::iterator iter_low = lex_index.lower_bound(low_bnd); 	
	VoxelSet::index<comp_xzy>::type::iterator iter_upp = lex_index.upper_bound(upp_bnd); 	
	
	if(iter_upp == lex_index.end()) {
		std::cout << "iter_upp is end" << std::endl;	
	}
	else {	
		std::cout << "iter_upp " <<**iter_upp << std::endl;
	}
	if(iter_low == lex_index.end()) {
		std::cout << " iter_low is end" << std::endl;
	}
	else {
		std::cout << "iter_low " <<**iter_low << std::endl;
	}

	VoxelSet::index<comp_yzx>::type::iterator x_iter;
	while(iter_low != iter_upp) {
		std::cout << "iter_low " << **iter_low << std::endl;	
		std::cout << "in y_conv" << std::endl;
		kernel_element = kernel_[(*iter_low)->get_y() - vox_y_coord + kernel_.size()/2];
		x_iter = vox_set.project<comp_yzx>(iter_low);
		TNT::Array2D<double> st_temp(3,3,0.0);
		int x_conv_flag = x_conv_window(x_iter, vox_set, st_temp);
		std::cout << "x_c_f " << x_conv_flag << std::endl;
		if (x_conv_flag == -1) return -1; 


		st += scalar_mult(kernel_element, st_temp);
		++iter_low;
	}
	return 0;
}

int LArVox::VoxelSet3DConvWindow::z_conv_window(VoxelSet::index<comp_xyz>::type::iterator z_iter, VoxelSet& vox_set, TNT::Array2D<double>& st) {
	std::cout << "z_conv_window1" << std::endl;
	boost::tuple<short,short,short> vox_tuple = (*z_iter)->get_coord_tuple();
	std::cout << "z_conv_window2" << std::endl;
	VoxelSet::index<comp_xyz>::type& lex_index = vox_set.get<comp_xyz>();
	std::cout << "z_conv_window3" << std::endl;
	double kernel_element;
	short vox_z_coord = vox_tuple.get<2>();
	boost::tuple<short,short,short> upp_bnd(vox_tuple.get<0>(), vox_tuple.get<1>(), vox_z_coord + kernel_.size()/2);
	boost::tuple<short,short,short> low_bnd(vox_tuple.get<0>(), vox_tuple.get<1>(), vox_z_coord - kernel_.size()/2);
	std::cout << "z_conv_window4" << std::endl;
	VoxelSet::index<comp_xyz>::type::iterator iter_low = lex_index.lower_bound(low_bnd); 	
	VoxelSet::index<comp_xyz>::type::iterator iter_upp = lex_index.upper_bound(upp_bnd); 	
	if(iter_upp == lex_index.end()) std::cout << "iter_upp == lex_index.end()" << std::endl;
	VoxelSet::index<comp_xzy>::type::iterator y_iter;
	while(iter_low != iter_upp){
		kernel_element = kernel_[(*iter_low)->get_z() - vox_z_coord + kernel_.size()/2];
		y_iter = vox_set.project<comp_xzy>(iter_low);
		std::cout << "y_iter " <<**y_iter << std::endl;
		TNT::Array2D<double> st_temp(3,3,0.0);
		int y_conv_flag = y_conv_window(y_iter, vox_set, st_temp);
		std::cout << st_temp;
		if (y_conv_flag == -1) return -1; 
		st += scalar_mult(kernel_element, st_temp);
		std::cout << "y_c_flag " <<y_conv_flag << std::endl << st;
		++iter_low;
	}
	return 0;
}
TNT::Array2D<double>& LArVox::VoxelSet3DConvWindow::scalar_mult(double scalar, TNT::Array2D<double>& matrix) {
	for(int i=0; i<matrix.dim1(); i++) {
		for(int j=0; j<matrix.dim2(); j++) {
			matrix[i][j] *= scalar;
		}
	}

	return matrix;
}

void LArVox::VoxelSet3DConvWindow::initialise_convolution(void) 
{}
LArVox::VoxelSetDerivativeInterface::VoxelSetDerivativeInterface(const std::string& transform_name) 
: VoxelSetTransform(transform_name)
{}
LArVox::VoxelSetDerivativeInterface::VoxelSetDerivativeInterface()
: VoxelSetTransform()
{}
LArVox::VoxelSetDerivativeInterface::~VoxelSetDerivativeInterface()
{}
void LArVox::VoxelSetDerivativeInterface::change_gradient::operator()(Voxel_ptr& v) {
	boost::static_pointer_cast<StructTensorVoxel, Voxel>(v)->set_gradient(new_gradient_);
}
LArVox::VoxelSetDerivativeSymmDiff::VoxelSetDerivativeSymmDiff(const std::string& transform_name)
: VoxelSetDerivativeInterface(transform_name)
{}
LArVox::VoxelSetDerivativeSymmDiff::VoxelSetDerivativeSymmDiff()
: VoxelSetDerivativeInterface()
{}
LArVox::VoxelSetDerivativeSymmDiff::~VoxelSetDerivativeSymmDiff()
{}
VoxelSet& LArVox::VoxelSetDerivativeSymmDiff::operator()(VoxelSet& v_set_in_st) {
	
	return v_set_in_st;
}
VoxelSet& LArVox::VoxelSetDerivativeSymmDiff::operator()(VoxelSet& v_set_in_scalar, VoxelSet& v_set_out_tensor) {
	VoxelSet::index<hash>::type& hash_index_in = v_set_in_scalar.get<hash>();
	
	VoxelSet::index<lex_xyz>::type& xyz_index_out = v_set_out_tensor.get<lex_xyz>();
	VoxelSet::index<lex_xyz>::type::iterator xyz_iter_out = xyz_index_out.begin();
	
	VoxelSet::index<lex_xyz>::type& xyz_index_in = v_set_in_scalar.get<lex_xyz>();
	VoxelSet::index<lex_xzy>::type& xzy_index_in = v_set_in_scalar.get<lex_xzy>();
	VoxelSet::index<lex_yzx>::type& yzx_index_in = v_set_in_scalar.get<lex_yzx>();

        VoxelSet::index<lex_xyz>::type::const_iterator xyz_iter_in;
        VoxelSet::index<lex_xzy>::type::const_iterator xzy_iter_in;
        VoxelSet::index<lex_yzx>::type::const_iterator yzx_iter_in;
	
	while(xyz_iter_out != xyz_index_out.end() ) {
		VoxelSet::index<hash>::type::const_iterator hash_iter_in = hash_index_in.find((*xyz_iter_out)->get_coord_tuple());
		xyz_iter_in = v_set_in_scalar.project<lex_xyz>(hash_iter_in);
		std::pair<double, bool> partial_z = calc_partial<lex_xyz>(&Voxel::xy_equal, &Voxel::get_z, xyz_iter_in, xyz_index_in);
		xzy_iter_in = v_set_in_scalar.project<lex_xzy>(hash_iter_in);
                std::pair<double, bool> partial_y = calc_partial<lex_xzy>(&Voxel::xz_equal, &Voxel::get_y, xzy_iter_in, xzy_index_in);
		yzx_iter_in = v_set_in_scalar.project<lex_yzx>(hash_iter_in);
                std::pair<double, bool> partial_x = calc_partial<lex_yzx>(&Voxel::yz_equal, &Voxel::get_x, yzx_iter_in, yzx_index_in);

		grad gradient_temp;
		gradient_temp[0] = partial_x;
		gradient_temp[1] = partial_y;
		gradient_temp[2] = partial_z;
		
		xyz_index_out.modify(xyz_iter_out, change_gradient(gradient_temp)); 
		++xyz_iter_out;
	}

	return v_set_out_tensor;
//	lex_index.insert(Voxel_ptr(new ScalarVoxel((*vox_iter1)->get_x() + 1, (*vox_iter1)->get_y(), (*vox_iter1)->get_z(), weight_temp)));
}

