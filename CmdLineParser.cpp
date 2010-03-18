#include "./CmdLineParser.hpp" 
#include <tclap/CmdLine.h>

LArVox::Options::Options() : parsed(false) 
{}
std::ostream& LArVox::operator<<(std::ostream& os, const Options & opt) {

	os << opt.parsed << " , " << opt.skipEvents << " , " <<  opt.maxEvents << " , " << opt.kernel_size << " , " << opt.voxel_size << " , " << opt.scale_t << " , " << opt.gamma << std::endl;
	os << opt.out_to_root << " , " << opt.root_filename_in << " , " << opt.root_filename_out << std::endl;
	os <<  opt.root_treename << " , " << opt.root_branchname << std::endl;

	return os;
}
LArVox::Options LArVox::CmdLineParser::parse(int argc, char** argv) {
	LArVox::Options opt;
	opt.parsed = false;
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
		return opt;

	}
	catch (TCLAP::ArgException &e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; 
		return opt;	
	}


}

