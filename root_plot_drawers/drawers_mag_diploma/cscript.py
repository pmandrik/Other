
import ROOT
from ROOT import *

import sys

def main() :
  to_copy = []
  gROOT.Reset();
  c1 = TCanvas("trigger","trigger",1200,0,2*800,2*200);
  c1.Range(0,0,1.,1.);

  p1 = TPaveText(0.01,0.05,0.32,0.95);
  p1.SetFillColor(kBlue-5);
  p111 = TPaveText(0.05,0.72,0.28,0.10);
  p11 = TPaveText(0.05,0.75,0.28,0.90);

  p2 = TPaveText(0.35,0.05,0.65,0.95);
  p2.SetFillColor(kBlue-8);
  p222 = TPaveText(0.35 + 0.04,0.72,0.65 - 0.04,0.10);
  p22 = TPaveText(0.35 + 0.04,0.75,0.65 - 0.04,0.90);

  p3 = TPaveText(0.68,0.05,0.99,0.95);
  p3.SetFillColor(kBlue-10);
  p333 = TPaveText(0.68 + 0.04,0.72,0.99 - 0.04,0.10);
  p33 = TPaveText(0.68 + 0.04,0.75,0.99 - 0.04,0.90);
  for p in [p1, p111, p11, p2, p222, p22, p3, p333, p33] : p.Draw();


  ar1 = TArrow();
  ar1.SetLineWidth(1);
  ar1.SetLineColor(1);
  ar1.SetFillStyle(1001);
  ar1.SetFillColor(1);

  for i in xrange(3):
    py = 0.2 + 0.3 * i
    ar1.SetFillColor(kAzure+4);
    ar1.DrawArrow( 0.32, py, 0.35, py, 0.042, "|>" );
    ar1.SetFillColor(kAzure+5);
    ar1.DrawArrow( 0.65, py, 0.68, py, 0.04, "|>" );

  # L1 text
  l1 = TText(0.33/2., 0.77, "Level 1");
  l1.SetTextAlign(21);
  l1.SetTextSize(0.15);
  
  ts = ["muons, jets", "electrons / photons", "#tau-leptons", " E_{Tmiss}, E_{Ttotal}"]
  for i, text in enumerate(ts) :
    t = TLatex(0.33/2., 0.62 - 0.1 * i, text);
    t.SetTextAlign(21);
    t.SetTextSize(0.08);
    t.Draw()
    to_copy  += [t]

  # L2 text
  l2 = TText(0.33 + 0.33/2., 0.77, "Level 2");
  l2.SetTextAlign(21);
  l2.SetTextSize(0.15);
  ts = ["Regions of Interest"]
  for i, text in enumerate(ts) :
    t = TLatex(0.33 + 0.33/2., 0.45 - 0.1 * i, text);
    t.SetTextAlign(21);
    t.SetTextSize(0.08);
    t.Draw()
    to_copy  += [t]

  # L3 text
  l3 = TText(0.67 + 0.33/2., 0.77, "Event Filter");
  l3.SetTextAlign(21);
  l3.SetTextSize(0.15);
  ts = ["Full event information"]
  for i, text in enumerate(ts) :
    t = TLatex(0.67 + 0.33/2., 0.45 - 0.1 * i, text);
    t.SetTextAlign(21);
    t.SetTextSize(0.08);
    t.Draw()
    to_copy  += [t]

  for l in [l1, l2, l3] : l.Draw();

  for p, text in zip([0.33/2., 0.33 + 0.33/2., 0.67 + 0.33/2.], ["~75 kHz", "~3.5 kHz", "~200 Hz"]):
    t = TText(p, 0.15, text);
    t.SetTextAlign(21);
    t.SetTextSize(0.125);
    t.Draw();
    to_copy += [t]

  c1.cd();

  T = sys.stdin.readline()

if __name__ == '__main__': main()

