

from numpy import *
from enthought.mayavi.mlab import * 
from numpy import random
def main(s, i) :
  x = []
  y = []
  z = []
  weight = []
  f = open(s, 'r')
  count = 0
  skip_st = 1;
  clf() 
  while f:
    line = f.readline();
    if len(line) == 0 : break
    if line[0] == '#' : continue
    a = array([float(s) for s in line.split()])
    x.append(a[0])
    y.append(a[1])
    z.append(a[2])
    weight.append(a[i])
    count = count + 1
  
  obj4 =points3d(x,y,z, weight, figure = gcf(), mask_points = 5, colormap = "spectral", scale_mode = 'none', mode = "cube", scale_factor=1.0)

  return obj4 
def plotgrad(s, skip) :
  x = []
  y = []
  z = []
  gx = []
  gy = []
  gz = []
  f = open(s, 'r')
  count = 0
  while f:
    line = f.readline();
    if len(line) == 0 : break
    if line[0] == '#' : continue
    if count % skip == 0 :
      a = array([float(s) for s in line.split()])
      x.append(a[0])
      y.append(a[1])
      z.append(a[2])
      gx.append(a[4])
      gy.append(a[6])
      gz.append(a[8])
    count = count + 1
  obj1 = quiver3d(x,y,z,gx,gy,gz,figure = gcf(), scale_mode = 'vector', scale_factor = 1.0, mask_points = None, line_width = 3)
  return obj

def test_numpy():
  x1 = [1,2,3]
  x2 = [2,3,4]
  x3 = 3.0 * x1[1]
  print x3

