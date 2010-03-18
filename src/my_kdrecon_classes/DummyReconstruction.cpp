#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

#include "Configuration.hpp"
#include "Event.hpp"
#include "Exception.hpp"
#include "HitPoint.hpp"
#include "HitInfo.hpp" 
#include "HitMap.hpp"
#include "DummyReconstruction.hpp"
#include "SeedFinder.hpp"
#include "Utility.hpp"

KDRecon::DummyReconstruction::DummyReconstruction()
 : currentEvent()
{}
KDRecon::DummyReconstruction::DummyReconstruction(const unsigned long nLocalPoints)
	: currentEvent(), currentSegment(nLocalPoints)
{}
void KDRecon::DummyReconstruction::operator()(KDRecon::EventPtr event)
{
	// ---Set internal event pointer to point to current event---
	this->currentEvent = event;

	// ---Output information about the event being processed---
	std::cerr << "=[Event " << event->getNumber() << "]=\n";
	
	//Populate a list with true line segments
	const std::list<KDRecon::Truth>& truth = event->getTruth();
        std::list<KDRecon::Truth>::const_iterator truth_iter = truth.begin();
        const std::list<KDRecon::Truth>::const_iterator true_end = truth.end();
        for(; truth_iter != true_end; ++truth_iter)
        {
              	
        }

	// Get a reference to the event's hitmap
	KDRecon::HitMap& hitmap = event->hitMap();

	// Output size of hitmap
	std::cerr << "Found " << hitmap.size() << " hits.\n";

	// ---Open ROOT file---
	boost::shared_ptr<TFile> output_file;
	if ( KDRecon::Configuration::Instance().writeHistograms() )
	{
		boost::shared_ptr<TFile> tmp(new TFile(KDRecon::Configuration::Instance().histogramsFilename().c_str(), "UPDATE"));
		output_file = tmp;
	}
	// Iterate over hit map to deal with each point in turn
	KDRecon::HitMap::const_iterator iter = hitmap.begin();
	KDRecon::HitMap::const_iterator end = hitmap.end();
	for(; iter != end; ++iter)
	{
		int x = static_cast<int>(floor(iter->first.getX()));
		int y = static_cast<int>(floor(iter->first.getY()));
		int z = static_cast<int>(floor(iter->first.getZ()));
		double w = iter->second.getWeight();
		std::cout << "Hit point = " << x << ", " << y << ", " << z << ", " << w << std::endl;
		//octVoxels->set(x,y,z,w);
	}


	// ---Close ROOT File---
	if ( KDRecon::Configuration::Instance().writeHistograms() )
	{
		output_file->Close();
	}

	// ---Reset currentEvent pointer before returning---
	this->currentEvent.reset();
}
