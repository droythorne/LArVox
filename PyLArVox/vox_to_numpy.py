

from numpy import *
from enthought.mayavi.mlab import * 
from numpy import random
import read_root
import ROOT as rdf
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
	
  while f:
    line = f.readline();
    if len(line) == 0 : break
    if line[0] == '#' : continue
    a = array([float(s) for s in line.split()])
    x.append(a[0])
    y.append(a[1])
    z.append(a[2])
    weight.append(a[3])
    if count % skip == 0 :
      wsf= (a[4]+a[5]+a[6])/3.0
      x_vec.append(a[0])
      y_vec.append(a[1])
      z_vec.append(a[2])
      e1.append(a[4])
      e2.append(a[5])
      e3.append(a[6])
      u1.append(a[4]*a[7]/wsf)
      u2.append(a[4]*a[8]/wsf)
      u3.append(a[4]*a[9]/wsf)
      v1.append(a[5]*a[10]/wsf)
      v2.append(a[5]*a[11]/wsf)
      v3.append(a[5]*a[12]/wsf)
      w1.append(a[6]*a[13]/wsf)
      w2.append(a[6]*a[14]/wsf)
      w3.append(a[6]*a[15]/wsf)
      hp.append((a[4]*a[5]*a[6])/(a[4]+a[5]+a[6]))
      hp_thresh.append((a[4]*a[5]*a[6])/((0.33*(a[4]+a[5]+a[6]))**3))

    count = count + 1
  obj1 = quiver3d(x_vec,y_vec,z_vec,u1,u2,u3, figure = figure('evecs_quiver'), scale_mode = 'none', scale_factor = 1.0, mask_points = None, line_width = 3)
  obj2 = quiver3d(x_vec,y_vec,z_vec,v1,v2,v3, figure = gcf(), mask_points = None , scale_mode = 'none', scale_factor = 1.0, line_width = 3)
  obj3 = quiver3d(x_vec,y_vec,z_vec,w1,w2,w3, figure = gcf(), mask_points = None, scale_mode = 'none', scale_factor = 1.0, line_width = 3)
  obj4 =points3d(x_vec,y_vec,z_vec, hp, figure = figure('hp_response'), colormap = "cool", scale_mode = 'none', mode = "sphere", scale_factor=0.7)
  obj5 = points3d(x_vec,y_vec,z_vec, hp_thresh, figure = figure('hp_thresh_response'), colormap = "cool", scale_mode = 'none', mode = "sphere", scale_factor=0.7)
  return obj4 
def plot_scalvox_from_generator(g, scal_vec_index, fig = figure(), skip = 'none', alpha = 0.0, beta = 0.0) :
  x = []
  y = []
  z = []
  weight = []
  for index, voxel in enumerate(g) :
    x.append(voxel[0][0])
    y.append(voxel[0][1])
    z.append(voxel[0][2])
    weight.append(voxel[1][scal_vec_index])
    fig_name = "scalar " + str(scal_vec_index)
  obj = points3d(x,y,z, weight, figure = fig, mask_points = skip, colormap = "cool", scale_mode = 'none', mode = "cube", scale_factor=20)
  return obj 

def plot_weights_from_generator(g, fig = figure(), skip = None) :
  x = []
  y = []
  z = []
  weight = []
  for index, voxel in enumerate(g) :
    x.append(voxel[0][0])
    y.append(voxel[0][1])
    z.append(voxel[0][2])    
    weight.append(voxel[0][3])
   
  obj = points3d(x,y,z, weight, figure = fig, colormap = "cool", scale_mode = 'none', mask_points = skip, mode = "cube", scale_factor=20)
  return obj

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
  fig_name = "lorvec"
  obj = points3d(x,y,z, weight, colormap = "spectral", figure = fig, scale_mode = 'none', mask_points = mask, mode = "sphere", opacity = 0.3, scale_factor= 50)
  return obj

def plot_feature_voxels(voxels_file, features_file, tree_name, fig = figure(), scalar_val = 0, mask = None) :
  g_feat = read_root.event_reader(features_file, tree_name)
  g_vox = read_root.event_reader(voxels_file, tree_name)
  while(1) :
    figure(fig.name) 

    t_feat = g_feat.next() 
    t_feat.ResetBit(rdf.TObject.kMustCleanup)
    t_vox = g_vox.next()
    t_vox.ResetBit(rdf.TObject.kMustCleanup)
    g_feat_read = read_root.voxel_reader(t_feat)
    g_vox_read = read_root.scalar_reader(t_vox)
    if(scalar_val != 0) :
      obj1 = plot_scalvox_from_generator(g_vox_read, scalar_val, fig, mask)
    else :
      obj1 = plot_weights_from_generator(g_vox_read, fig, mask)
    obj2 = plot_lorvec_from_generator(g_feat_read, figure(gcf()))
    yield (obj1, obj2)    
def calc_hp(e1,e2,e3) :
  evals = [abs(e1),abs(e2),abs(e3)]
  evals.sort()
  lambda_ = evals[2]
  
def test_numpy():
  x1 = [1,2,3]
  x2 = [2,3,4]
  x3 = 3.0 * x1[1]
  print x3

