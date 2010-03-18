import ROOT as rdf
from numpy import *
from enthought.mayavi.mlab import *


class LatteDataSet(object):
    """object wrapping a Latte TFile and outputting data from each branch

    Expected branches:
    ==================
    hits           : TClonesArray of TLorentzVector
    primary_start  : TClonesArray of TVector3
    primary_end    : TClonesArray of TVector3
    secondary_start: TClonesArray of TVector3
    secondary_end  : TClonesArray of TVector3

    The TFile, TTree and TClonesArrays forming the data set are wrapped by
    this class, taking care of all the internals.

    """
    def __init__(self, filename):
        """construct a LatteDataSet instance

        Parameters:
        ===========
        filename: string
            full path to ROOT file to be opened. File must contain a TTree
            named genie_qel_run

        Raises:
        =======
        AttributeError:
            raised if TFile does not contain TTree named genie_qel_run

        """        
        self._tfile = rdf.TFile(filename, "read")
        self._ttree = self._tfile.Get("genie_qel_run")
        if not self._ttree:
            raise AttributeError, "TFile "+filename+" has no object named genie_qel_run"

        #Otherwise, initialize the TClonesArrays
        self._clonesarrays = [] 
        self._hits = self.__make_clonesarray("TLorentzVector",2000,"hits")

        self._primaryStart = self.__make_clonesarray("TVector3",200,"primary_start")
        self._primaryEnd = self.__make_clonesarray("TVector3",200,"primary_end")

        self._secStart = self.__make_clonesarray("TVector3",200,"secondary_start")
        self._secEnd = self.__make_clonesarray("TVector3",200,"secondary_end")



    def __make_clonesarray(self, type, size, branch):
        """private method to construct a TClonesArray and point it at a branch

        Parameters:
        ===========
        type: string
            name of the concrete type stored in the TClonesArray

        size: int
            default length of the TClonesArray

        branch: string
            name of branch in TTree to point clones array at

        The constructed TClonesArray is also stored in a list to ease the
        extra handling of the reset bit needed in PyRoot
        """
        tca = rdf.TClonesArray(type,size)
        tca.ResetBit(rdf.TObject.kMustCleanup)
        self._ttree.SetBranchAddress(branch,tca)
        self._clonesarrays.append(tca)
        return tca


    def entries(self):
        """generator function yielding tuple of TClonesArray for each event

        There are five tuple entries at indices:

        0: Hits -> TClonesArray of TLorentzVector
        1: Primary Start Points -> TClonesArray of TVector3
        2: Primary End Points -> TClonesArray of TVector3
        3: Secondary Start Points -> TClonesArray of TVector3
        4: Secondary End Points -> TClonesArray of TVector3
        """
        for i in range(self._ttree.GetEntries()):
            self._ttree.GetEntry(i)
            for c in self._clonesarrays:
                c.ResetBit(rdf.TObject.kMustCleanup)

            yield (self._hits, self._primaryStart, self._primaryEnd, self._secStart, self._secEnd)



class DataSetRepresentation(object):
    """Present a LatteDataSet object as sequences of internal types

    Converts:
    =========
    hits -> PyTrackGen CellData object
    points -> lists of tuples

    """
    def __init__(self, filename):
        """construct a DataSetRepresentation instance

        Parameters:
        ===========
        filename: string
            full path to TFile to be opened
        
        """
        self._data = LatteDataSet(filename)


    def entries(self):
        """generator function yielding tuple of sequences for each TClonesArray

        Yielded tuple has following entries at indices:

        0: PyTrackGen.CellData object over the hits
        1: Primary Start Points as generator of tuples of (x,y,z) coords
        2: Primary End Points as generator of tuples of (x,y,z) coords
        3: Secondary Start Points as generator of tuples of (x,y,z) coords
        4: Secondary End Points as generator of tuples of (x,y,z) coords

        """
        ds = self._data.entries()

        for e in ds:
            yield (self.__convert_to_cells(e[0]), 
                   self.__convert_to_tuples(e[1]),
                   self.__convert_to_tuples(e[2]),
                   self.__convert_to_tuples(e[3]),
                   self.__convert_to_tuples(e[4]))


    def __convert_to_cells(self, stream):
        """Take a stream of TLorentzVectors and convert them to CellData object
        """
        c = PyTrackGen.CellData.CellData(1.0)

        for v in stream:
            c.setCellValue((int(v.X()), int(v.Y()), int(v.Z())), 0, v.E())

        return c

    def __convert_to_tuples(self,stream):
        """Take a stream of TVector3s and convert them a generator of tuples
        """
        return ((v.X(),v.Y(),v.Z()) for v in stream)





def event_reader(filename):
    """read voxel data from GEANT4/GENIE ROOT output

    Parameters:
    ===========
    filename: string
        ROOT file to be read in. The file must contain a TTree named
        "genie_qel_run".

    Returns:
    ========
    event_reader is a generator function which on each call to next will
    return a generator over the voxels. Each voxel is represented as a
    tuple of (voxel_energy, (x_coord, y_coord, z_coord))

    Example:
    ========
        g = event_reader("my_file.root")

        for event in g:
            for voxel in event:
                print "energy: ",voxel[0]

    """
    f = rdf.TFile(filename,"read")

    t = f.Get("genie_qel_run")

    if t:
        tca = rdf.TClonesArray("TLorentzVector",2000)

        # ResetBit is needed in Python only...
        tca.ResetBit(rdf.TObject.kMustCleanup)
        t.SetBranchAddress("hits",tca)

        for i in xrange(t.GetEntries()):
            t.GetEntry(i)

            # We must also call ResetBit here (don't ask me why, we just do...)
            tca.ResetBit(rdf.TObject.kMustCleanup)

            # Yield a *generator* over the voxels
            yield ((v.E(), (v.X(),v.Y(),v.Z())) for v in tca)



def example_print(filename):
    """Demonstrate simple use of event_reader to print out voxels in events
    """

    g = event_reader(filename)

    for index, event in enumerate(g):
        print "EVENT: ",index

        for voxel in event:
            #Print out the energy deposited and the tuple of coordinates
            print voxel[0], voxel[1]

def example_print_truth(filename):
    """Demonstrate simple use of LatteDataSet to read truth information
    """

    g = LatteDataSet(filename)

    for i,e in enumerate(g.entries()):
        print "EVENT: ",i
        print "\tN(hits): ",e[0].GetEntries()
        print "\tN(psp) : ",e[1].GetEntries()
        print "\tN(pep) : ",e[2].GetEntries()
        print "\tN(ssp) : ",e[3].GetEntries()
        print "\tN(sep) : ",e[4].GetEntries()
def plot_truth(filename) :
  """Demonstrate simple use of LatteDataSet to plot voxels with truth data superimposed
  """
  data = LatteDataSet(filename)
  g = data.entries()
  while(1) :
    e = g.next()
    y_val = e[0].GetEntries()
    print "\tN(hits): ",e[0].GetEntries()
    print e[0][0].X()
    x = []
    y = []
    z = []
    weight = []
    for vox in e[0] :
      x.append(vox.X())
      y.append(vox.Y())
      z.append(vox.Z())
      weight.append(vox.E())
    fig_name = "voxels"

    obj = points3d(x,y,z,weight, colormap = "spectral", figure = figure(fig_name),  scale_mode = 'none', mode = "cube", opacity = 0.5, scale_factor=2 )
    x_psp = []
    y_psp = []
    z_psp = []
    for vox in e[1] :
      x_psp.append(vox.X())
      y_psp.append(vox.Y())
      z_psp.append(vox.Z())
    obj2 = points3d(x_psp, y_psp, z_psp, color= (0,1,1), figure = figure(fig_name), mode = "sphere", opacity = 0.3, scale_factor = 10)
    x_pep = []
    y_pep = []
    z_pep = []
    for vox in e[2] :
      x_pep.append(vox.X())
      y_pep.append(vox.Y())
      z_pep.append(vox.Z())
    obj3 = points3d(x_pep, y_pep, z_pep, color=(0,1,1), figure = figure(fig_name), mode = "sphere", opacity = 0.3, scale_factor = 10)
    x_ssp = []
    y_ssp = []
    z_ssp = []
    for vox in e[3] :
      x_ssp.append(vox.X())
      y_ssp.append(vox.Y())
      z_ssp.append(vox.Z())
    obj4 = points3d(x_ssp, y_ssp, z_ssp, color = (1,0,0.2), figure = figure(fig_name), mode = "sphere", opacity = 0.3, scale_factor = 10)
    yield (obj,obj2,obj3,obj4)

