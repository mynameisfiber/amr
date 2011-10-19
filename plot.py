#!/usr/bin/python

import numpy as np
import pylab as py

def plot_cell(data,c):
  py.scatter(data[0], data[1])
  s = 1
  if c == 0:
    s = 5
  for i in range(2,data.shape[0],5):
    py.plot([data[i], data[i+2]], [data[i+1], data[i+3]],linewidth=s)

def plot_mesh(data):
  for cellidx in data.shape[0]:
    plot_cell(data[cellidx,:])

def plot_mesh_from_file(filename):
  maxloc = [0,0]
  minloc = [0,0]
  for c, line in enumerate(file(filename, "r").xreadlines()):
    data = np.array(line.split(), dtype=np.double)
    plot_cell(data,c)
    maxloc[0] = max(maxloc[0], data[0])
    maxloc[1] = max(maxloc[1], data[1])
    minloc[0] = min(minloc[0], data[0])
    minloc[1] = min(minloc[1], data[1])
  return maxloc, minloc

import sys
maxloc, minloc = plot_mesh_from_file(sys.argv[1])
py.xlim(minloc[0], maxloc[0])
py.ylim(minloc[1], maxloc[1])
#py.show()
py.savefig("testplot.png")
  
