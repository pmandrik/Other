
import ROOT
from ROOT import *

import sys

def main() :
  to_copy = []
  gROOT.Reset();
  c1 = TCanvas("architecture","architecture" ,20,10,800,500);
  c1.Range(0,0,1.,1.);

  p1 = TPaveText(0.01,0.05,0.30,0.95);
  p1.SetFillColor(39);
  p1.SetBorderSize(1);
  p1.Draw();

  pro2 = TText(0.27/2. + 0.02, 0.90, "Rootcore packages");
  pro2.SetTextAlign(21);
  pro2.SetTextSize(0.018 * 2.5);
  pro2.Draw();

  root_core_packages = ["RootCoreCLHEP", "Asg_CMake", "Asg_FastJet", "InDetTrackCorr", "GoodRunsLists"]
  for i, name in enumerate( root_core_packages ) :
    p1dict = TPaveText(0.02, 0.80 - 0.07 * i, 0.29, 0.87 - 0.07 * i);
    p1dict.SetTextSize(0.018 * 2);
    p1dict.AddText( name );
    p1dict.Draw();
    to_copy.append(p1dict);

  p11 = TPaveText(0.01,0.05,0.30,0.50);
  p11.SetFillColor(37);
  p11.SetBorderSize(1);
  p11.Draw();

  pro22 = TText(0.27/2. + 0.02, 0.45, "Other packages");
  pro22.SetTextAlign(21);
  pro22.SetTextSize(0.018 * 2.5);
  pro22.Draw();

  other_packages = ["Track Extrapolator", "InDetTrackDecisionTool", "JetTrackZClusterTool", "JetFastJetFinderTool"]
  for i, name in enumerate( other_packages ) :
    p1dict = TPaveText(0.02, 0.35 - 0.07 * i, 0.29, 0.42 - 0.07 * i);
    p1dict.SetTextSize(0.018 * 2);
    p1dict.AddText( name );
    p1dict.Draw();
    to_copy.append(p1dict);

  p2 = TPaveText(0.35,0.05,0.95,0.95);
  p2.SetFillColor(36);
  p2.SetBorderSize(1);
  p2.Draw();

  ar1 = TArrow();
  ar1.SetLineWidth(6);
  ar1.SetLineColor(1);
  ar1.SetFillStyle(1001);
  ar1.SetFillColor(1);

  for i in xrange(3) :
    px = (0.87 - 0.42)/2. + 0.42 + 0.05 * (i - 1)
    ar1.DrawArrow( px, 0.81,px, 0.80,0.012,"|>");
    ar1.DrawArrow( px, 0.73,px, 0.72,0.012,"|>");
    ar1.DrawArrow( px, 0.28,px, 0.27,0.012,"|>");

  for i in xrange(2) :
    px = 0.90 - 0.45 * i;
    px2 = px - 0.05;
    ar1.DrawArrow(0.30,px,0.35,px,0.012,"|>");
    ar1.DrawArrow(0.30,px2,0.35,px2,0.012,"<|");

  ar1.DrawArrow(0.41,0.755,0.42, 0.755,0.012,"|>");
  ar1.DrawArrow(0.41,0.225,0.42, 0.225,0.012,"|>");
  for i in xrange(3) :
    py = (0.79 - 0.20) / 2. + 0.20 + 0.05 * (i - 1)
    ar1.DrawArrow(0.41,py,0.42, py,0.012,"|>");
  
  # main framework
  proM = TText(0.5 + 0.15, 0.90, "Framework core");
  proM.SetTextAlign(21);
  proM.SetTextSize(0.018 * 2.5);
  proM.Draw();

  roottree = TPaveText(0.42, 0.87, 0.89, 0.81);
  roottree.SetTextSize(0.018 * 2);
  roottree.AddText( "D3PD ROOT tree skeleton analysis class" );
  roottree.Draw();

  D3PDShell = TPaveText(0.42, 0.87 - 0.08, 0.89, 0.81 - 0.08);
  D3PDShell.SetTextSize(0.018 * 2);
  D3PDShell.AddText( "D3PD Shell" );
  D3PDShell.Draw();

  Event = TPaveText(0.42, 0.87 - 0.08*2, 0.89, 0.28);
  Event.SetTextSize(0.018 * 2);
  Event.Draw();

  evt = TText(0.457, 0.50, "Event Information");
  evt.SetTextAlign(21);
  evt.SetTextAngle(90);
  evt.SetTextSize(0.018 * 2.5);
  evt.Draw();

  event_info = ["Common Event Info", "Trigger Decision Info", "Tracks Containers", "Jets Containers", "Vertexes Container", "CaloClusters Container"]
  for i, name in enumerate( event_info ) :
    p1dict = TPaveText(0.48, 0.69 - 0.06 * i - 0.013, 0.87, 0.63 - 0.06 * i - 0.013);
    p1dict.SetTextSize(0.018 * 2);
    p1dict.AddText( name );
    p1dict.SetFillColor(25);
    p1dict.Draw();
    to_copy.append(p1dict);

  analys = TPaveText(0.42, 0.26, 0.89, 0.20);
  analys.SetTextSize(0.018 * 2);
  analys.AddText( "User Analysis Loop" );
  analys.Draw();

  params = TPaveText(0.37, 0.79, 0.41, 0.20);
  params.SetTextSize(0.018 * 2);
  text = params.AddText( "Parameters" );
  text.SetTextAngle(90);
  params.Draw();

  c1.cd();

  T = sys.stdin.readline()

if __name__ == '__main__': main()

