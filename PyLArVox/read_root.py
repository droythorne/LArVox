import ROOT as rdf

def event_reader(filename, treename):
    """read voxel data from GEANT4/GENIE ROOT output

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
    tuple of (voxel_energy, (x_coord, y_coord, z_coord))

    Example:
    ========
        g = event_reader("my_file.root", "my_tree.root")

        for event in g:
            for voxel in event:
                print "energy: ",voxel[0]

    """
    f = rdf.TFile(filename,"read")

    t = f.Get(treename)

    if t:
        t2 = rdf.TTree("t2","Tree2")

        # ResetBit is needed in Python only...
        t2.ResetBit(rdf.TObject.kMustCleanup)
        t.SetBranchAddress(t.GetListOfBranches().First().GetName(),t2)
        for i in xrange(t.GetEntries()):
            t.GetEntry(i)
	    
            # We must also call ResetBit here (don't ask me why, we just do...)
            t2.ResetBit(rdf.TObject.kMustCleanup)
            yield t2

def voxel_reader(t2):
    
  tlorvec = rdf.TLorentzVector()
  tlorvec.ResetBit(rdf.TObject.kMustCleanup)
  t2.SetBranchAddress(t2.GetListOfBranches().First().GetName(),tlorvec)
  for j in xrange(t2.GetEntries()) :
    t2.GetEntry(j)
    tlorvec.ResetBit(rdf.TObject.kMustCleanup)
    # Yield a *generator* over the voxels
    yield ((tlorvec.E(), (tlorvec.X(),tlorvec.Y(),tlorvec.Z()))) 
def scalar_reader(t2):
  tarrayd = rdf.TArrayD()
  tlorvec = rdf.TLorentzVector()
  tlorvec.ResetBit(rdf.TObject.kMustCleanup)
  t2.SetBranchAddress(t2.GetListOfBranches().First().GetName(),tlorvec)
  tarrayd.Reset()
  t2.SetBranchAddress("voxel_scalars_branch", tarrayd)
  
  for j in xrange(t2.GetEntries()) :
    t2.GetEntry(j)
    yield (tlorvec, tarrayd)
def example_print(filename, treename):
    """Demonstrate simple use of event_reader to print out voxels in events
    """

    g = event_reader(filename, treename)
    
    for index, event in enumerate(g):
        print "EVENT: ",index
        g2 = voxel_reader(event)
        for index, voxel in enumerate(g2):
            #Print out the energy deposited and the tuple of coordinates
            print voxel[0]
	
