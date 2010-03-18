#include <iostream>
#include <sstream>

#include <boost/shared_ptr.hpp>

#include "TFile.h"
#include "TH1.h"
#include "TSpectrum.h"

#include "Configuration.hpp"
#include "Counter.hpp"
#include "Event.hpp"
#include "Exception.hpp"
#include "my_kdrecon_classes/ReconstructEvent_test.hpp"
#include "SeedFinder.hpp"
#include "Tree.hpp"

#include "EventClass/C1Long.hpp"
#include "EventClass/C1Gap.hpp"

void KDRecon::ReconstructEvent_test::operator()(KDRecon::EventPtr event)
{
/*	// ---Set internal event pointer to point to current event---
	this->currentEvent = event;

	// ---Output information about the event being processed---
	std::cerr << "=[Event " << event->getNumber() << "]=\n";

	// Get a reference to the event's hitmap
	KDRecon::HitMap& hitmap = event->hitMap();

	// Output size of hitmap
	std::cerr << "Found " << hitmap.size() << " hits.\n";

	// Output hitmap
	//std::cout << hitmap << std::endl;
// ---Build kdtree---
	KDRecon::Tree tree(event);
	
	// ---Open ROOT file---
	boost::shared_ptr<TFile> output_file;
	if ( KDRecon::Configuration::Instance().writeHistograms() )
	{
		boost::shared_ptr<TFile> tmp(new TFile(KDRecon::Configuration::Instance().histogramsFilename().c_str(), "UPDATE"));
		output_file = tmp;
	}

	unsigned long segmentCounter(0);
	unsigned long nTooShort(0);
	bool foundTwoSegments(false);
	while ( true )
	{
		// ---Clear the segment---
		currentSegment.clear();

		// ---Increment Segment Counter---
		++segmentCounter;

		// ---Make Histogram---
		std::ostringstream ost;
		ost << "event" << event->getNumber() << "_segment" << segmentCounter;
		const std::string histname = ost.str();
		const std::string histname_x = histname + "_x";
		const std::string histname_y = histname + "_y";
		const std::string histname_z = histname + "_z";
		TH1F xHistogram(histname_x.c_str(), histname_x.c_str(), 100, -1, 1);
		TH1F yHistogram(histname_y.c_str(), histname_y.c_str(), 100, -1, 1);
		TH1F zHistogram(histname_z.c_str(), histname_z.c_str(), 100, -1, 1);

		// ---Pick seed point---
		KDRecon::SeedFinder seedFinder(event);
		KDRecon::HitPoint seed;
		try {
			seed = seedFinder.getSeed();
		}
		catch (KDRecon::Exception::EndOfEvent& e)
		{
			break; // Finished event
		}
		hitmap[seed].markUnavailable();
		currentSegment.addPoint(seed);
		currentIndex = tree.indexFromPoint(seed);

		// Pick (nLocalPoints - 1) more points
		unsigned long nLocal(currentSegment.nLocalPoints());
		for(unsigned long i = 1; i < nLocal; ++i)
		{
			if ( this->nextPoint(tree) == false )
			{
				++nTooShort;
				//std::cerr << "Track too short.\n";
				continue;
			}
		}

		// variables to hold results of segment fits
		std::vector<float> lastLocalCosines(3);
		//float lastLocalQuality(0.0);
		
		// Fit segment
		currentSegment.fitLocal();
		lastLocalCosines = currentSegment.getLocalCosines();
		//lastLocalQuality = currentSegment.getLocalQuality();
		
		xHistogram.Fill(lastLocalCosines[0]);
		yHistogram.Fill(lastLocalCosines[1]);
		zHistogram.Fill(lastLocalCosines[2]);

		// Now loop over picking and fitting
		unsigned long stepCount(0);
		bool useSegment(true);
		while ( true )
		{
			++stepCount;
			// Pick a point
			if ( this->nextPoint(tree) == false )
			{
				if ( currentSegment.size() < nLocal )
				{
					useSegment = false;
				}
				break; // End of segment 
			}
			// Perform fits
			currentSegment.fitLocal();
			lastLocalCosines = currentSegment.getLocalCosines();
			//lastLocalQuality = currentSegment.getLocalQuality();
			xHistogram.Fill(lastLocalCosines[0]);
			yHistogram.Fill(lastLocalCosines[1]);
			zHistogram.Fill(lastLocalCosines[2]);

			// Now check to see if two peaks exist
			if ( NumPeaks(xHistogram) >= 2 )
			{
				std::cerr << "Found two peaks in x.\n";
				foundTwoSegments = true;
				break;
			}
			else if ( NumPeaks(yHistogram) >= 2 )
			{
				std::cerr << "Found two peaks in y.\n";
				foundTwoSegments = true;
				break;
			}
			else if ( NumPeaks(zHistogram) >= 2 )
			{
				std::cerr << "Found two peaks in z.\n";
				foundTwoSegments = true;
				break;
			}
		} // end: while true [Segment]

		if ( useSegment )
		{
			event->addSegment(currentSegment);
						// ---Write histograms---
			if ( KDRecon::Configuration::Instance().writeHistograms() )
			{
				xHistogram.Write();
				yHistogram.Write();
				zHistogram.Write();
				output_file->Write();
			}
		}
	} // end: while true [Event] 
	
	if ( foundTwoSegments )
		KDRecon::Counter::Instance().increment("Two peaks");

	// output segment fits here
	std::list<KDRecon::Segment>::iterator iter = event->getSegments().begin();
	std::list<KDRecon::Segment>::iterator end = event->getSegments().end();
	for(; iter != end; ++iter)
	{
		iter->fitLine();
		std::cerr << "----Segment Fit---- [" << (iter->getPoints()).size() << " points]\n";
		std::cerr << "Direction cosines: " << iter->getCosines() << "\n";
		std::cerr << "Start point: " << iter->getStart() << "\n";
		std::cerr << "End point: " << iter->getEnd() << "\n";
		std::cerr << "Quality of fit: " << iter->getQuality() << "\n";
		KDRecon::EventClass::C1Long<> m(*iter);
		if ( m.matches() )
			std::cerr << "Matched C1Long\n";
	}
	std::cerr << "Also found " << nTooShort << " track candidates which were too short.\n";

	KDRecon::EventClass::C1Gap<> m(event->getSegments());
	if ( m.matches() )
	{
		std::cerr << "C1Gap: Found gap of size " << m.gapSize() << "\n";
	}
	
	// ---Close ROOT File---
	if ( KDRecon::Configuration::Instance().writeHistograms() )
	{
		output_file->Close();
	}
*/
	// ---Reset currentEvent pointer before returning---
	this->currentEvent.reset();
}
/*
void KDRecon::ReconstructEvent::filterNeighbours(KDRecon::Tree::result_vector& results, KDRecon::Tree& tree, long current_idx)
{
	KDRecon::EventPtr cur_evt(currentEvent);
	KDRecon::HitMap& hitmap = cur_evt->hitMap();
	KDRecon::Tree::result_vector allowed;
	allowed.reserve(results.size());
	KDRecon::Tree::result_vector::iterator end = results.end();
	for(KDRecon::Tree::result_vector::iterator iter = results.begin(); iter != end; ++iter)
	{
		// Strip self
		if ( iter->idx == current_idx )
			continue;

		// Strip points marked used
		KDRecon::HitPoint pt = tree.pointFromIndex(iter->idx);
		if ( hitmap[pt].isAvailable() )
		{
			// Add point to vector of allowed points
			allowed.push_back(*iter);
		}
		else continue;
	}
	results = allowed; // Copy temporary back over original result vector
}


bool KDRecon::ReconstructEvent::nextPoint(KDRecon::Tree& tree)
{
	// Obtain shared_ptr to current event
	KDRecon::EventPtr event(currentEvent);
	// Create vector to store results
	KDRecon::Tree::result_vector results;
	// Find nearest 5 neighbours around current point
	tree.findAllInRadius(tree.pointFromIndex(currentIndex), 5, results);
	// filter out allocated neighbours and self
	this->filterNeighbours(results, tree, currentIndex);
	if ( results.size() == 0 )
	{
		return false; // if no more points available
	}
	// Update current point information
	currentIndex = results[0].idx;
	currentPoint = tree.pointFromIndex(currentIndex);
	// Add current point to current segment
	currentSegment.addPoint(currentPoint);
	// Mark unavailable in hitmap
	KDRecon::HitMap& hitmap = event->hitMap();
	hitmap[currentPoint].markUnavailable();
	return true; // A point was available
}
*/
