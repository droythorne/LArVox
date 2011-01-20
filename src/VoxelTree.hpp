#ifndef VOXEL_VOXELTREE_HPP_
#define VOXEL_VOXELTREE_HPP_

#include <functional>
#include <iostream>
#include <kdtree++/kdtree.hpp>
#include "Voxel.hpp"
namespace LArVox{
class Voxel_ptr_node;
std::ostream& operator<<(std::ostream& os,const Voxel_ptr_node& vpn);

class Voxel_ptr_node {

	public:
		Voxel_ptr_node(const Voxel_ptr& vp);
		Voxel_ptr_node(const Voxel_ptr_node & vpn);
		~Voxel_ptr_node();

		double distance_to(Voxel_ptr_node const& x) const;

		Voxel_ptr get_voxel_ptr(void) const ;
		short operator[](size_t const N) const;

		friend std::ostream& operator<<(std::ostream& os,const Voxel_ptr_node& vpn);
	protected:
		virtual std::ostream& println(std::ostream& os) const;
	private:
		Voxel_ptr vox_ptr_;
};

inline short Voxel_ptr_node::operator[](size_t const N) const {return (*vox_ptr_)[N];} 
//Checks for equality of shared pointers
inline bool operator==(Voxel_ptr_node const& x, Voxel_ptr_node const& y) {return x==y;} 
inline short vpn_accessor(Voxel_ptr_node vpn, size_t k) {return vpn[k];}

typedef KDTree::KDTree<3, Voxel_ptr_node, std::pointer_to_binary_function<Voxel_ptr_node, size_t, short>  > v_tree;
} //namespace LArVox

#endif //VOXEL_VOXELTREE_HPP_

