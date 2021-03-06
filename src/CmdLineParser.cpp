#include <vector>
#include "./CmdLineParser.hpp" 
#include <tclap/CmdLine.h>

LArVox::Options::Options() : parsed(false) 
{}
std::ostream& LArVox::operator<<(std::ostream& os, const Options & opt) {

	os << opt.parsed << " , " << opt.skipEvents << " , " <<  opt.maxEvents << " , " << opt.kernel_size << " , " << opt.voxel_size << " , " << opt.scale_t << " , " << opt.gamma << std::endl;
	os << opt.out_to_root << " , " << opt.root_filename_in << " , " << opt.root_filename_out << std::endl;
	os <<  opt.root_treename << " , " << opt.root_branchname << std::endl;

	std::vector<std::string>::const_iterator feature_names_it = opt.feature_names.begin();
	std::vector<double>::const_iterator thresholds_it = opt.thresholds.begin();
	std::vector<double>::const_iterator windows_it = opt.windows.begin();
	while(feature_names_it != opt.feature_names.end()) os << *feature_names_it << " ";
	os << std::endl;
	while(thresholds_it != opt.thresholds.end()) os << *thresholds_it << " ";
	os << std::endl;
	while(windows_it != opt.windows.end()) os << *windows_it << " ";
	os << std::endl;
 
	return os;
}
LArVox::Options LArVox::CmdLineParser::parse(int argc, char** argv) {
	LArVox::Options opt;
	opt.parsed = false;
	allowed_features_.push_back("forster");
	allowed_features_.push_back("forster_thresh");
	allowed_features_.push_back("min_eval");
	try {
		TCLAP::CmdLine cmd("LArVox_HP - implements 3D feature detection using variant of the Harris-Steven/Plessey algorithm",' ',"0.1");
		TCLAP::UnlabeledValueArg<std::string> root_filename_in_arg("root_filename_in", "Input ROOT filename", true, "default_in.root", "string");
		TCLAP::ValueArg<unsigned long> skipEvents_arg("s", "skip", "Number of events to skip processing", false, 0, "unsigned long");
		TCLAP::ValueArg<unsigned long> maxEvents_arg("m", "max", "Maximum number of events to process", false, 0, "unsigned long");
		TCLAP::ValueArg<int> kernel_size_arg("k", "kernel", "Kernel width (in # voxels)", false, 3, "int");
		TCLAP::ValueArg<double> voxel_size_arg("v", "vox", "Voxel side length (mm)", false, 1, "double");
		TCLAP::ValueArg<double> scale_t_arg("t", "tscale", "HS/Plessey t (scale space) parameter (mm) (REFERENCE?)", false, 0.7, "double");
		TCLAP::ValueArg<double> gamma_arg("g", "gamma", "HS/Plessey gamma (integration scale) parameter", false, 1.4, "double");
		TCLAP::ValueArg<std::string> root_filename_out_arg("R", "root_filename_out", "Output ROOT filename", false, "default_out.root", "string");
		TCLAP::ValueArg<std::string> root_treename_arg("T", "tree", "ROOT TTree name (for input and output)", false, "genie_qel_run", "string");
		TCLAP::ValueArg<std::string> root_branchname_arg("B", "branch", "ROOT TBranch (must contain TClonesArray of TLorentzVector)", false, "hits", "string");
		TCLAP::SwitchArg out_to_root_arg("q", "quiet", "Supress output to ROOT TFile", false); 
		TCLAP::ValuesConstraint<std::string> allowed_features_constraint(allowed_features_);
		TCLAP::MultiArg<std::string> features_arg("F", "feature", "Name of the feature identifying scalar", false, &allowed_features_constraint);  
		TCLAP::MultiArg<double> thresholds_arg("", "thresh", "Feature voxel threshold (i'th command line instance of thresh binds to i'th instance of -F)", false, "double");
		TCLAP::MultiArg<double> windows_arg("", "window", "Feature voxel exclusion window for non-max-supression algorithm (i'th command line instance of thresh binds to i'th instance of -F)", false, "double");
		cmd.add(root_filename_in_arg);
		cmd.add(skipEvents_arg);
		cmd.add(maxEvents_arg);
		cmd.add(kernel_size_arg);
		cmd.add(voxel_size_arg);
		cmd.add(scale_t_arg);
		cmd.add(gamma_arg);
		cmd.add(root_filename_out_arg);
		cmd.add(root_treename_arg);
		cmd.add(root_branchname_arg);
		cmd.add(out_to_root_arg);
		cmd.add(features_arg);
		cmd.add(thresholds_arg);
		cmd.add(windows_arg);
		cmd.parse(argc, argv);

		opt.parsed = true;
		opt.skipEvents = skipEvents_arg.getValue();
		opt.maxEvents = maxEvents_arg.getValue();
		opt.kernel_size = kernel_size_arg.getValue();
		opt.voxel_size = voxel_size_arg.getValue();
		opt.scale_t = scale_t_arg.getValue();
		opt.gamma = gamma_arg.getValue();
		opt.root_filename_out = root_filename_out_arg.getValue();
		opt.root_treename = root_treename_arg.getValue();
		opt.root_branchname = root_branchname_arg.getValue();
		opt.out_to_root = out_to_root_arg.getValue();
		opt.root_filename_in = root_filename_in_arg.getValue();
		opt.feature_names = features_arg.getValue();
		opt.thresholds = thresholds_arg.getValue();
		opt.windows = windows_arg.getValue();
		return opt;

	}
	catch (TCLAP::ArgException &e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
		return opt;	
	}


}
std::vector<std::string>& LArVox::CmdLineParser::getAllowedFeatures() { return allowed_features_;}
LArVox::ScalarFieldAllocator::ScalarFieldAllocator(Options& opt, std::vector<std::string>& allowed_features) {
	unsigned int thresh_size = opt.thresholds.size();
	unsigned int wind_size = opt.windows.size();
	for(int i=0; i<opt.feature_names.size(); i++) {
		std::vector<std::string>::const_iterator all_feat_it = allowed_features.begin();
		int feature_id = 0;
		while(all_feat_it != allowed_features.end()) {
			if(opt.feature_names[i].compare(*all_feat_it) == 0) {
				switch(feature_id) {
					case 0 : 
						{
							LArVox::ForsterAccessor fa;
							scalars_.push_back(new LArVox::CrudeVoxelNonMaxSupp(fa));
						}
						break;
					default : 
						break;
				}
				if(i < thresh_size) scalars_.back()->SetThreshold(opt.thresholds[i]);
				if(i < wind_size) scalars_.back()->SetWindowSize(opt.windows[i]);
				break;
			}
			feature_id++;
			all_feat_it++;
		}
	}
}
