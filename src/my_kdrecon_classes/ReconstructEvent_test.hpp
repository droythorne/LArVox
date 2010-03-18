#ifndef KDRECON_RECONSTRUCTEVENTTEST_HPP
#define KDRECON_RECONSTRUCTEVENTTEST_HPP

#include <boost/shared_ptr.hpp>

#include "Event.hpp"
#include "Exception.hpp"
#include "Tree.hpp"

namespace KDRecon {

class ReconstructEvent_test {
	public:
		ReconstructEvent_test(const unsigned long nLocalPoints)
		: currentEvent(), currentSegment(nLocalPoints), currentIndex(0), currentPoint() {};
		void operator()(KDRecon::EventPtr event);
	private:
		// Hold a weak pointer to the current event
		// so we don't cause a leak if operator()
		// returns before reset() is called
		// but we can still access the event from
		// other member functions
		KDRecon::EventObserver currentEvent;
		KDRecon::Segment currentSegment;
		long currentIndex;
		KDRecon::HitPoint currentPoint;
};

} // namespace KDRecon

#endif
