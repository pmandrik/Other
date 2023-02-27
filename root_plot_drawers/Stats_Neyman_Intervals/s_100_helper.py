
import sys
sys.path.append('/home/i/.macros')

from pROOT import *
from pROOT import memory_wrapper as _

MY_PRIMES = {}

def get_mp(name):
  return MY_PRIMES[name]

class my_prim():
  def __init__(self, name):
    self.name = name
    global MY_PRIMES
    MY_PRIMES[name] = self
  
  def canv(self):
    self.canvas = get_canvas ()
    return self.canvas

  def hist(self, nbins, min, max):
    self.hist = _( TH1D(self.name, self.name, nbins, min, max) )
    return self.hist

  def draw(self, opt=""):
    self.canvas.cd()
    self.hist.Draw(opt);

  def save(self):
    self.hist.Write()

def draw_P(vL, vP):
  avL = array("d", vL)
  avP = array("d", vP)

  gr_P = _( TGraph( len(avP), avL, avP ) )
  return gr_P










