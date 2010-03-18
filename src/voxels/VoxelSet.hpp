#ifndef VOXEL_VOXELSET_HPP
#define VOXEL_VOXELSET_HPP

#include <algorithm>
#include <iostream>
#include <cstdio>
#include <iterator>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <utility>
#include <boost/shared_ptr.hpp>
#include "voxels/Voxel.hpp"
namespace LArVox {

using namespace boost::multi_index;
struct lex_xyz{};
struct lex_xzy{};
struct lex_yzx{};
struct comp_xyz{};
struct comp_xzy{};
struct comp_yzx{};
struct hash{};
typedef multi_index_container<
	Voxel_ptr,
	indexed_by<
		ordered_unique<
			tag<lex_xyz>, identity<Voxel_ptr>, VoxelPredicate_lex_xyz >,
		ordered_unique<
			tag<lex_xzy>, identity<Voxel_ptr>, VoxelPredicate_lex_xzy >,
		ordered_unique<
			tag<lex_yzx>, identity<Voxel_ptr>, VoxelPredicate_lex_yzx >,
		hashed_unique<
			tag<hash>,
			composite_key<
				Voxel,
				const_mem_fun<Voxel, short, &Voxel::get_x>, 
				const_mem_fun<Voxel, short, &Voxel::get_y>,
				const_mem_fun<Voxel, short, &Voxel::get_z>
			>
		>,
		ordered_unique<
			tag<comp_xyz>,
			composite_key<
				Voxel,
				const_mem_fun<Voxel, short, &Voxel::get_x>,
                                const_mem_fun<Voxel, short, &Voxel::get_y>,
                                const_mem_fun<Voxel, short, &Voxel::get_z>
                        >
		>,
		ordered_unique<
                        tag<comp_xzy>,     
                        composite_key<
                                Voxel,
                                const_mem_fun<Voxel, short, &Voxel::get_x>,
                                const_mem_fun<Voxel, short, &Voxel::get_z>,
                		const_mem_fun<Voxel, short, &Voxel::get_y>
                        >
                >,
                ordered_unique<
                        tag<comp_yzx>,
                        composite_key<
                                Voxel,
                                const_mem_fun<Voxel, short, &Voxel::get_y>,
                                const_mem_fun<Voxel, short, &Voxel::get_z>,
                                const_mem_fun<Voxel, short, &Voxel::get_x>
                        >
                >
		
	
	>
> VoxelSet;

/* VoxelSet_out_by currently functions to output only the data members of ScalarVoxel, though 
should make use of the overloaded operator<< of its MultiIndexContainer object.
*/

template<typename Tag, typename MultiIndexContainer>
void VoxelSet_out_by(
	const MultiIndexContainer& s,
	Tag* =0 /* fixes a MSVC++ 6.0 bug with implicit template function parms */
) {
// Obtain a reference to the index tagged by Tag
	const typename boost::multi_index::index<MultiIndexContainer,Tag>::type& i= get<Tag>(s);
	typedef typename MultiIndexContainer::value_type value_type;
//Dump the elements of the index to cout */
	typename boost::multi_index::index<MultiIndexContainer,Tag>::type::iterator it = i.begin();
	while(it != i.end()) {
		std::cout << **it ;
		++it;
	} 

  //Eventually modify this function to refer to overloaded virtual operator<< 
//  	std::copy(i.begin(),i.end(),std::ostream_iterator<value_type>(std::cout));

}
} //namespace LArVox
#endif //VOXEL_VOXELSET_HPP
