#include <iostream>
#include <algorithm>
#include "voxels/Voxel.hpp"
#include "tnt_array2d_utils.h"

std::ostream& LArVox::operator<<(std::ostream& os,const Voxel& v)
{
	return v.println(os);
}

LArVox::Voxel::Voxel(short vox_x, short vox_y, short vox_z) 
{
	vox_coord_[0] = vox_x;
	vox_coord_[1] = vox_y;
	vox_coord_[2] = vox_z;
}
LArVox::Voxel::Voxel(boost::array<short, 3> vox_coord)
{
	vox_coord_[0] = vox_coord[0];
	vox_coord_[1] = vox_coord[1];
	vox_coord_[2] = vox_coord[2];
}
LArVox::Voxel::Voxel()
{
	vox_coord_[0] = 0.;
        vox_coord_[1] = 0.;
        vox_coord_[2] = 0.;
}
short LArVox::Voxel::get_x() const {return vox_coord_[0];}
short LArVox::Voxel::get_y() const {return vox_coord_[1];}
short LArVox::Voxel::get_z() const {return vox_coord_[2];}

//Lexicographical ordering by (x,y,z)
bool LArVox::Voxel::operator<(const Voxel& v) const {return vox_coord_[0] < v.get_x() || ( (vox_coord_[0] == v.get_x()) && ( (vox_coord_[1] < v.get_y()) || ((vox_coord_[1] == v.get_y()) && (vox_coord_[2] < v.get_z())) ) ); }
bool LArVox::Voxel::xy_equal(const Voxel& v) const {return (v.get_x()==vox_coord_[0])&&(v.get_y()==vox_coord_[1]);}
bool LArVox::Voxel::xz_equal(const Voxel& v) const {return (v.get_x()==vox_coord_[0])&&(v.get_z()==vox_coord_[2]);}
bool LArVox::Voxel::yz_equal(const Voxel& v) const {return (v.get_y()==vox_coord_[1])&&(v.get_z()==vox_coord_[2]);}

boost::tuple<short, short, short> LArVox::Voxel::get_coord_tuple(void) const {return boost::make_tuple(vox_coord_[0], vox_coord_[1], vox_coord_[2]);}
std::ostream& LArVox::Voxel::println(std::ostream& os) const {
	os<<this->vox_coord_[0]<<" "<<this->vox_coord_[1]<<" "<<this->vox_coord_[2]<<std::endl;
	return os;
}
LArVox::ScalarVoxel::ScalarVoxel(short vox_x, short vox_y, short vox_z, double vox_weight)
: Voxel(vox_x, vox_y, vox_z), vox_weight(vox_weight)
{}
double LArVox::ScalarVoxel::get_weight() const {return vox_weight;}
void LArVox::ScalarVoxel::set_weight(double w) {vox_weight = w;}


std::ostream& LArVox::ScalarVoxel::println(std::ostream& os) const {
        os<<this->vox_coord_[0]<<" "<<this->vox_coord_[1]<<" "<<this->vox_coord_[2]<<" "<<this->vox_weight<<std::endl;
        return os;
}
LArVox::TensorVoxel::TensorVoxel(short vox_x, short vox_y, short vox_z, LArVox::grad gradient) 
: Voxel(vox_x, vox_y, vox_z), gradient_(gradient)	
{}
LArVox::TensorVoxel::TensorVoxel(short vox_x, short vox_y, short vox_z) 
: Voxel(vox_x, vox_y, vox_z)
{
	gradient_[0] = std::pair<double, bool>(0.0, false);
	gradient_[1] = std::pair<double, bool>(0.0, false);
	gradient_[2] = std::pair<double, bool>(0.0, false);
}
	
LArVox::grad LArVox::TensorVoxel::get_gradient() const {return gradient_;}
void LArVox::TensorVoxel::set_gradient(grad gradient){
	gradient_[0] = gradient[0];
	gradient_[1] = gradient[1];
	gradient_[2] = gradient[2];
}
std::ostream& LArVox::TensorVoxel::println(std::ostream& os) const {
        os << this->vox_coord_[0] << " " << this->vox_coord_[1] << " " << this->vox_coord_[2];
	for(int i=0; i<3; i++){
		os << " " << gradient_[i].first << " " << gradient_[i].second;
	}
	os << std::endl; 
        return os;
}
LArVox::ScalarTensorVoxel::ScalarTensorVoxel(short vox_x, short vox_y, short vox_z, LArVox::grad gradient) 
: Voxel(vox_x, vox_y, vox_z), gradient_(gradient), vox_weight(-1.0)	
{}
LArVox::ScalarTensorVoxel::ScalarTensorVoxel(short vox_x, short vox_y, short vox_z, double weight) 
: Voxel(vox_x, vox_y, vox_z), vox_weight(weight)	
{
	gradient_[0] = std::pair<double, bool>(0.0, false);
        gradient_[1] = std::pair<double, bool>(0.0, false);
        gradient_[2] = std::pair<double, bool>(0.0, false);
}
LArVox::ScalarTensorVoxel::ScalarTensorVoxel(short vox_x, short vox_y, short vox_z) 
: Voxel(vox_x, vox_y, vox_z), vox_weight(-1.0)
{
	gradient_[0] = std::pair<double, bool>(0.0, false);
	gradient_[1] = std::pair<double, bool>(0.0, false);
	gradient_[2] = std::pair<double, bool>(0.0, false);
}
double LArVox::ScalarTensorVoxel::get_weight() const {return vox_weight;}
void LArVox::ScalarTensorVoxel::set_weight(double w) {vox_weight = w;}
LArVox::grad LArVox::ScalarTensorVoxel::get_gradient() const {return gradient_;}	
void LArVox::ScalarTensorVoxel::set_gradient(grad gradient){
        gradient_[0] = gradient[0];
        gradient_[1] = gradient[1];
        gradient_[2] = gradient[2];
}
std::ostream& LArVox::ScalarTensorVoxel::println(std::ostream& os) const {
        os << this->vox_coord_[0] << " " << this->vox_coord_[1] << " " << this->vox_coord_[2];
	os << " " << this->vox_weight;
        for(int i=0; i<3; i++){
                os << " " << gradient_[i].first << " " << gradient_[i].second;
        }
        os << std::endl;
        return os;
}

LArVox::StructTensorVoxel::StructTensorVoxel(short vox_x, short vox_y, short vox_z, LArVox::grad gradient) 
: Voxel(vox_x, vox_y, vox_z), gradient_(gradient), struct_tensor_(), vox_weight_(-1.0), vox_scalars_()
{}
LArVox::StructTensorVoxel::StructTensorVoxel(short vox_x, short vox_y, short vox_z)
: Voxel(vox_x, vox_y, vox_z), struct_tensor_(), vox_weight_(-1.0), vox_scalars_()
{
        gradient_[0] = std::pair<double, bool>(0.0, false);
        gradient_[1] = std::pair<double, bool>(0.0, false);
        gradient_[2] = std::pair<double, bool>(0.0, false);
}
LArVox::StructTensorVoxel::StructTensorVoxel(short vox_x, short vox_y, short vox_z, double vox_weight)
: Voxel(vox_x, vox_y, vox_z), struct_tensor_(), vox_weight_(vox_weight), vox_scalars_()
{
        gradient_[0] = std::pair<double, bool>(0.0, false);
        gradient_[1] = std::pair<double, bool>(0.0, false);
        gradient_[2] = std::pair<double, bool>(0.0, false);
}
LArVox::StructTensorVoxel::StructTensorVoxel(const StructTensorVoxel& grad_vox, TNT::Array2D<double> struct_tensor)
: Voxel(grad_vox.get_x(), grad_vox.get_y(), grad_vox.get_z()), struct_tensor_(struct_tensor), vox_weight_(grad_vox.get_weight()), vox_scalars_()
{
	LArVox::grad temp_gradient = grad_vox.get_gradient();	
        gradient_[0] = temp_gradient[0];
        gradient_[1] = temp_gradient[1];
        gradient_[2] = temp_gradient[2];
}
LArVox::grad LArVox::StructTensorVoxel::get_gradient(void) const {return gradient_;}
void LArVox::StructTensorVoxel::set_gradient(LArVox::grad gradient){ 
	gradient_[0] = gradient[0];
	gradient_[1] = gradient[1];
	gradient_[2] = gradient[2];
}
double LArVox::StructTensorVoxel::get_weight(void) const {return vox_weight_;}
void LArVox::StructTensorVoxel::set_weight(double weight) {vox_weight_ = weight;}
TNT::Array2D<double> LArVox::StructTensorVoxel::get_st(void) {return struct_tensor_;}
void LArVox::StructTensorVoxel::set_st(TNT::Array2D<double> st) {struct_tensor_ = st;}
void LArVox::StructTensorVoxel::append_scalar(double scalar) {vox_scalars_.push_back(scalar);}
void LArVox::StructTensorVoxel::set_scalars(std::vector<double> scalars) {vox_scalars_ = scalars;}
const std::vector<double>& LArVox::StructTensorVoxel::get_scalars(void) const {return vox_scalars_;}
 std::vector<double>& LArVox::StructTensorVoxel::modify_scalars(void) {return vox_scalars_;}
std::ostream& LArVox::StructTensorVoxel::println(std::ostream& os) const {
        os << this->vox_coord_[0] << " " << this->vox_coord_[1] << " " << this->vox_coord_[2] << " " << this->vox_weight_;
	for(int i=0; i<3; i++){
		os << " " << gradient_[i].first << " " << gradient_[i].second; }
	os << " " << struct_tensor_;
	std::vector<double>::const_iterator it = vox_scalars_.begin();
	while (it != vox_scalars_.end()) {
		os << " " << *it;
		++it;
	}
	os << std::endl; 
        return os;
}

