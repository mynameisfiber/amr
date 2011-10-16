#!/usr/bin/python

import numpy as np
import pylab as py
py.ion()

points  = np.loadtxt("test.points.dat"   , dtype = np.double)
vpoints = np.loadtxt("test.v.points.dat" , dtype = np.double)
vedges  = np.loadtxt("test.v.edges.dat"  , dtype = np.double)

py.figure(1)
py.clf()
for i in range(vedges.shape[0]):
  if vedges[i,1] == -1.0:
    start  = vpoints[vedges[i,0]]
    end    = start + vedges[i,2:4] / np.sqrt((vedges[i,2:4]**2).sum())
  else:
    start  = vpoints[vedges[i,0]]
    end    = vpoints[vedges[i,1]]
  py.plot([start[0], end[0]], [start[1],end[1]])

py.scatter(points[:,0], points[:,1])
py.scatter(vpoints[:,0], vpoints[:,1], c="y", alpha=0.5)

py.xlim(points[:,0].min(), points[:,0].max())
py.ylim(points[:,1].min(), points[:,1].max())
py.show()
