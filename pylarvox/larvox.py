import ROOT as rdf
rdf.gSystem.Load("/home/droythorne/local/lib64/liblarvox.so")
from ROOT import LArVox
from numpy import *
from enthought.mayavi.mlab import *
def event_reader(filename, treename):
    """read voxel data from LArVox output

    Parameters:
    ===========
    filename: string
        ROOT file to be read in.
    treename: string
        ROOT TTree to be read in from filename

    Returns:
    ========
    event_reader is a generator function which on each call to next will
    return a generator over the voxels. Each voxel is represented as a
    LArVox::TVoxel object. Branch address is hard-set to "voxel_data_branch"
    for now
    Example: TODO - complete this!
    ========
        g = event_reader("my_file.root", "my_tree.root")

        for event in g:
    """
    f = rdf.TFile(filename,"read")

    t = f.Get(treename)

    if t:
	tca = rdf.TClonesArray("LArVox::TVoxel", 2000)
	tca.ResetBit(rdf.TObject.kMustCleanup)
	
#       t.SetBranchAddress(t.GetListOfBranches().First().GetName(),t2)
	t.SetBranchAddress("voxel_data_branch", tca) 
        for i in xrange(t.GetEntries()):
            t.GetEntry(i)
	    
            # We must also call ResetBit here (don't ask me why, we just do...)
            tca.ResetBit(rdf.TObject.kMustCleanup)
            yield tca

def voxel_reader(tca):
  num_voxels = tca.GetEntries()	  
  for j in xrange(tca.GetEntries()) :
    #print tca[j].vox_coords_.E()
    yield (tca[j].vox_coords_.E(), (tca[j].vox_coords_.X(), tca[j].vox_coords_.Y(), tca[j].vox_coords_.Z()))

def plot_lorvec_from_generator(g, fig = figure(), mask = None) :
  x = []
  y = []
  z = []
  weight = []
  for index, voxel in enumerate(g) :
    x.append(voxel[1][0])
    y.append(voxel[1][1])
    z.append(voxel[1][2])
    weight.append(voxel[0])
  obj = points3d(x,y,z, weight, colormap = "spectral", figure = fig, scale_mode = 'none', mask_points = mask, mode = "sphere", opacity = 0.3, scale_factor= 50)
  return obj

def plot_feature_voxels(voxels_file, tree_name, fig = figure(), scalar_val = 0, mask = None) :
  g_vox = event_reader(voxels_file, tree_name)
  figure(fig.name)
  while(1) :
    t_vox = g_vox.next() 
    t_vox.ResetBit(rdf.TObject.kMustCleanup)
    g_vox_read = voxel_reader(t_vox)
    obj1 = plot_lorvec_from_generator(g_vox_read, figure(gcf()))
    yield obj1    
def example_print(filename, treename):
    """Demonstrate simple use of event_reader to print out voxels in events
    """

    g = event_reader(filename, treename)
    
    for index, event in enumerate(g):
        print "EVENT: ",index
        v = voxel_reader(event)
        for index, v in enumerate(v):
           #Print out the energy deposited and the tuple of coordinates
           print v[0], v[1][0], v[1][1], v[1][2]
	
