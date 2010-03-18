#ifndef VOXEL_VOXEL_HPP
#define VOXEL_VOXEL_HPP
#include <utility>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include "tnt_array2d.h"

namespace LArVox {

class Voxel;
typedef boost::shared_ptr<Voxel> Voxel_ptr;
typedef boost::array<std::pair<double, bool>, 3> grad;
typedef bool (Voxel::*equality)(const Voxel&) const;
typedef short (Voxel::*coord)(void) const;

std::ostream& operator<<(std::ostream& os,const Voxel& v);

class Voxel {
	
	public:
		Voxel(short vox_x, short vox_y, short vox_z); 
		Voxel(boost::array<short, 3> vox_coord);
		Voxel(); 
		friend std::ostream& operator<<(std::ostream& os,const Voxel& v);
		short get_x() const;
		short get_y() const;
		short get_z() const;
		//Lexicographical ordering by (x,y,z)
		bool operator<(const Voxel& v) const; 
		bool xy_equal(const Voxel& v) const;
		bool xz_equal(const Voxel& v) const;
		bool yz_equal(const Voxel& v) const;
		boost::tuple<short, short, short> get_coord_tuple(void) const;
		inline short operator[](size_t const N) const {return vox_coord_[N];} 
	protected:
		virtual std::ostream& println(std::ostream& os) const;	
		boost::array<short,3> vox_coord_;
};

class ScalarVoxel : public Voxel {
	public:
		ScalarVoxel(short vox_x, short vox_y, short vox_z, double vox_weight);
		double get_weight() const;
		void set_weight(double w);
	protected:
		virtual std::ostream& println(std::ostream& os) const ;
	private:
		double vox_weight;
};
class TensorVoxel : public Voxel {
	public:
		TensorVoxel(short vox_x, short vox_y, short vox_z, grad gradient);
		TensorVoxel(short vox_x, short vox_y, short vox_z);
		grad get_gradient(void) const;
		void set_gradient(grad gradient);
	protected:
		virtual std::ostream& println(std::ostream& os) const ;
	private:
		grad gradient_;
};
class ScalarTensorVoxel : public Voxel {
	 public:
                ScalarTensorVoxel(short vox_x, short vox_y, short vox_z, grad gradient);
		ScalarTensorVoxel(short vox_x, short vox_y, short vox_z, double weight);
		ScalarTensorVoxel(short vox_x, short vox_y, short vox_z, double weight, grad gradient);
		ScalarTensorVoxel(short vox_x, short vox_y, short vox_z);
		double get_weight(void) const;
		void set_weight(double weight);
                grad get_gradient(void) const;
                void set_gradient(grad gradient);
        protected:
                virtual std::ostream& println(std::ostream& os) const ;
        private:
                grad gradient_;
		double vox_weight;
};

class StructTensorVoxel : public Voxel {
	public:
		StructTensorVoxel(short vox_x, short vox_y, short vox_z, grad gradient);
		StructTensorVoxel(short vox_x, short vox_y, short vox_z);
		StructTensorVoxel(short vox_x, short vox_y, short vox_z, double vox_weight);
		StructTensorVoxel(const StructTensorVoxel& grad_vox, TNT::Array2D<double> struct_tensor);
		grad get_gradient(void) const;
		void set_gradient(grad gradient);
		double get_weight(void) const;
		void set_weight(double weight);
		TNT::Array2D<double> get_st(void);
		void set_st(TNT::Array2D<double> struct_tensor);
		void append_scalar(double scalar);
		void set_scalars(std::vector<double> scalars);
		const std::vector<double>& get_scalars(void) const;
		std::vector<double>& modify_scalars(void);
	protected:
		virtual std::ostream& println(std::ostream& os) const;
	private:
		grad gradient_;
		TNT::Array2D<double> struct_tensor_;
		double vox_weight_;
		std::vector<double> vox_scalars_;
};
struct VoxelPredicate_lex_xzy {

	bool operator() (const Voxel_ptr v1, const Voxel_ptr v2) const{return v1->get_x() < v2->get_x() || ( (v1->get_x() == v2->get_x()) && ( (v1->get_z() < v2->get_z()) || ((v1->get_z() == v2->get_z()) && (v1->get_y() < v2->get_y())) ) ); }
};

struct VoxelPredicate_lex_xyz {
	bool operator() (const Voxel_ptr v1, const Voxel_ptr v2) const{return v1->get_x() < v2->get_x() || ( (v1->get_x() == v2->get_x()) && ( (v1->get_y() < v2->get_y()) || ((v1->get_y() == v2->get_y()) && (v1->get_z() < v2->get_z())) ) ); }
};

struct VoxelPredicate_lex_yzx {
	bool operator() (const Voxel_ptr v1, const Voxel_ptr v2) const{return v1->get_y() < v2->get_y() || ( (v1->get_y() == v2->get_y()) && ( (v1->get_z() < v2->get_z()) || ((v1->get_z() == v2->get_z()) && (v1->get_x() < v2->get_x())) ) ); }
};

} //namespace LArVox
#endif //VOXEL_VOXEL_HPP
