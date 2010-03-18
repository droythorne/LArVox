#ifndef KDRECON_DUMMYRECONSTRUCTION2_HPP 
#define KDRECON_DUMMYRECONSTRUCTION2_HPP

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Event.hpp"
#include "Exception.hpp"
#include "Tree.hpp"
// reconstructor whiich doeas absolutely nothing!
namespace KDRecon {
	
class DummyReconstruction2 {
	public:
		DummyReconstruction2();
		DummyReconstruction2(const unsigned long nLocalPoints);
	
		void operator()(KDRecon::EventPtr event);
	private:
		// Hold a weak ptr to the current event
		// so we don't cause a leak if operator()
		// returns before reset() is called
		// but we can still access the event from
		// other member functions
		KDRecon::EventObserver currentEvent;
		unsigned long currentSegment;
};
	
} // namespace KDRecon

#endif
