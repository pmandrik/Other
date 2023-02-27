
import ROOT
from ROOT import *

import sys

from math import cos, sin

def main() :
  to_copy = []
  gROOT.Reset();
  c1 = TCanvas("c0","c0",1200,0,600,600);
  c1.Range(0,0,1.,1.);

  el2 = TEllipse(0.5, 0.5, 0.15)
  el2.SetFillStyle(1001);
  el2.SetPhimax(180)
  el2.SetPhimin(63)
  el2.SetLineColor(4)
  el2.Draw()

  el1 = TEllipse(0.5, 0.5, 0.07)
  el1.SetFillStyle(1001);
  el1.SetPhimax(180)
  el1.SetLineColor(2)
  el1.SetPhimin(30)
  el1.Draw()

  ar1 = TArrow();
  ar1.SetLineWidth(2);
  ar1.SetLineColor(1);
  ar1.SetFillStyle(1001);
  ar1.SetFillColor(1);

  ar1.DrawArrow( 0.1, 0.5, 0.9, 0.5, 0.012, "<|" );
  ar1.DrawArrow( 0.5, 0.5, 0.5, 0.9, 0.012, "|>" );
  ar1.DrawArrow( 0.5, 0.5, 0.30, 0.30, 0.012, "|>" );

  ar2 = TArrow();
  ar2.SetLineWidth(2);
  ar2.SetLineColor(2);
  ar2.SetFillStyle(1001);
  ar2.SetFillColor(2);

  ar2.DrawArrow( 0.5, 0.5, 0.75, 0.65, 0.012, "|>" );

  ar3 = TArrow();
  ar3.SetLineWidth(2);
  ar3.SetLineColor(4);
  ar3.SetFillStyle(1001);
  ar3.SetFillColor(4);

  ar3.DrawArrow( 0.5, 0.5, 0.65, 0.80, 0.012, "|>" );


  #---text
  text = TText(0.1, 0.52, "z");
  text.SetTextAlign(21);
  text.SetTextSize(0.04);
  text.Draw()

  texta = TText(0.9, 0.46, "beam");
  texta.SetTextAlign(21);
  texta.SetTextSize(0.04);
  texta.Draw()

  textb = TText(0.1, 0.46, "beam");
  textb.SetTextAlign(21);
  textb.SetTextSize(0.04);
  textb.Draw()

  text2 = TText(0.52, 0.9, "y");
  text2.SetTextAlign(21);
  text2.SetTextSize(0.04);
  text2.Draw()

  text3 = TText(0.28, 0.28, "x");
  text3.SetTextAlign(21);
  text3.SetTextSize(0.04);
  text3.Draw()


  text4 = TText(0.65, 0.82, "jet");
  text4.SetTextAlign(21);
  text4.SetTextSize(0.025);
  text4.SetTextColor(4);
  text4.Draw()

  text5 = TText(0.75, 0.67, "track");
  text5.SetTextAlign(21);
  text5.SetTextSize(0.025);
  text5.SetTextColor(2);
  text5.Draw()

  text41 = TLatex(0.53, 0.7, "#eta_{jet}");
  text41.SetTextAlign(21);
  text41.SetTextSize(0.03);
  text41.SetTextColor(4);
  text41.Draw()

  text51 = TLatex(0.58, 0.58, "#eta_{track}");
  text51.SetTextAlign(21);
  text51.SetTextSize(0.03);
  text51.SetTextColor(2);
  text51.Draw()

  c1.cd();

  #--------------------------------------#

  c2 = TCanvas("c1","c1",600,0,600,600);
  c2.Range(0,0,1.,1.);

  el3 = TEllipse(0.5, 0.5, 0.15)
  el3.SetFillStyle(1001);
  el3.SetPhimax(63)
  el3.SetPhimin(31)
  el3.SetLineColor(4)
  el3.Draw()

  ar1 = TArrow();
  ar1.SetLineWidth(2);
  ar1.SetLineColor(1);
  ar1.SetFillStyle(1001);
  ar1.SetFillColor(1);

  ar1.DrawArrow( 0.1, 0.5, 0.9, 0.5, 0.012, "<|" );
  ar1.DrawArrow( 0.5, 0.5, 0.5, 0.9, 0.012, "|>" );
  ar1.DrawArrow( 0.5, 0.5, 0.30, 0.30, 0.012, "|>" );

  ar2 = TArrow();
  ar2.SetLineWidth(2);
  ar2.SetLineColor(2);
  ar2.SetFillStyle(1001);
  ar2.SetFillColor(2);

  ar2.DrawArrow( 0.5, 0.5, 0.75, 0.65, 0.012, "|>" );

  ar3 = TArrow();
  ar3.SetLineWidth(2);
  ar3.SetLineColor(4);
  ar3.SetFillStyle(1001);
  ar3.SetFillColor(4);

  ar3.DrawArrow( 0.5, 0.5, 0.65, 0.80, 0.012, "|>" );

  #---text
  text.Draw()
  texta.Draw()
  textb.Draw()
  text2.Draw()
  text3.Draw()
  text4.Draw()
  text5.Draw()

  text42 = TLatex(0.63, 0.63, "#eta_{TJ}");
  text42.SetTextAlign(21);
  text42.SetTextSize(0.03);
  text42.SetTextColor(4);
  text42.Draw()

  #--------------------------------------#

  c3 = TCanvas("c2","c2",1200,0,600,600);
  c3.Range(0,0,1.,1.);

  wel3 = TEllipse(0.5, 0.5, 0.35)
  wel3.SetFillStyle(1001);
  wel3.SetLineColor(1)
  wel3.SetLineWidth(2)
  wel3.Draw()

  eel2 = TEllipse(0.5, 0.5, 0.15)
  eel2.SetFillStyle(1001);
  eel2.SetPhimax(156)
  eel2.SetPhimin(0)
  eel2.SetLineColor(4)
  eel2.Draw()

  eel1 = TEllipse(0.5, 0.5, 0.07)
  eel1.SetFillStyle(1001);
  eel1.SetPhimax(45)
  eel1.SetLineColor(2)
  eel1.SetPhimin(0)
  eel1.Draw()
  
  ar1 = TArrow();
  ar1.SetLineWidth(2);
  ar1.SetLineColor(1);
  ar1.SetFillStyle(1001);
  ar1.SetFillColor(1);

  ar1.DrawArrow( 0.1, 0.5, 0.9, 0.5, 0.012, "|>" );
  ar1.DrawArrow( 0.5, 0.1, 0.5, 0.9, 0.012, "|>" );

  ar2 = TArrow();
  ar2.SetLineWidth(2);
  ar2.SetLineColor(2);
  ar2.SetFillStyle(1001);
  ar2.SetFillColor(2);

  angle = 45
  ar2.DrawArrow( 0.5, 0.5, 0.5 + 0.35 * cos(3.14 * angle / 180.), 0.5 + 0.35 * sin(3.14 * angle / 180.), 0.012, "|>" );

  ar3 = TArrow();
  ar3.SetLineWidth(2);
  ar3.SetLineColor(4);
  ar3.SetFillStyle(1001);
  ar3.SetFillColor(4);

  angle = 156
  ar3.DrawArrow( 0.5, 0.5, 0.5 + 0.35 * cos(3.14 * angle / 180.), 0.5 + 0.35 * sin(3.14 * angle / 180.), 0.012, "|>" );

  wel4 = TEllipse(0.5, 0.5, 0.015)
  wel4.SetFillStyle(1001);
  wel4.SetLineColor(1)
  wel4.SetLineWidth(2)
  wel4.Draw()

  wel5 = TEllipse(0.5, 0.5, 0.0025)
  wel5.SetFillStyle(1001);
  wel5.SetLineColor(1)
  wel5.SetLineWidth(3)
  wel5.Draw()

  #---text
  qtext = TText(0.48, 0.45, "z");
  qtext.SetTextAlign(21);
  qtext.SetTextSize(0.04);
  qtext.Draw()

  qtexta = TText(0.56, 0.45, "beam");
  qtexta.SetTextAlign(21);
  qtexta.SetTextSize(0.04);
  qtexta.Draw()

  qtext2 = TText(0.52, 0.9, "y");
  qtext2.SetTextAlign(21);
  qtext2.SetTextSize(0.04);
  qtext2.Draw()

  qtext3 = TText(0.9, 0.52, "x");
  qtext3.SetTextAlign(21);
  qtext3.SetTextSize(0.04);
  qtext3.Draw()

  angle = 156
  qtext4 = TText(0.5 + 0.37 * cos(3.14 * angle / 180.), 0.5 + 0.37 * sin(3.14 * angle / 180.), "jet");
  qtext4.SetTextAlign(21);
  qtext4.SetTextSize(0.025);
  qtext4.SetTextColor(4);
  qtext4.Draw()

  angle = 45
  qtext5 = TText(0.5 + 0.37 * cos(3.14 * angle / 180.), 0.5 + 0.37 * sin(3.14 * angle / 180.), "track");
  qtext5.SetTextAlign(21);
  qtext5.SetTextSize(0.025);
  qtext5.SetTextColor(2);
  qtext5.Draw()

  qtext41 = TLatex(0.40, 0.66, "#varphi_{jet}");
  qtext41.SetTextAlign(21);
  qtext41.SetTextSize(0.03);
  qtext41.SetTextColor(4);
  qtext41.Draw()

  qtext51 = TLatex(0.61, 0.525, "#varphi_{track}");
  qtext51.SetTextAlign(21);
  qtext51.SetTextSize(0.03);
  qtext51.SetTextColor(2);
  qtext51.Draw()

  #--------------------------------------#

  c4 = TCanvas("c3","c3",1200,0,600,600);
  c4.Range(0,0,1.,1.);

  ewel3 = TEllipse(0.5, 0.5, 0.35)
  ewel3.SetFillStyle(1001);
  ewel3.SetLineColor(1)
  ewel3.SetLineWidth(2)
  ewel3.Draw()

  qeel1 = TEllipse(0.5, 0.5, 0.07)
  qeel1.SetFillStyle(1001);
  qeel1.SetPhimax(45)
  qeel1.SetLineColor(2)
  qeel1.SetPhimin(0)
  qeel1.Draw()

  ar1.DrawArrow( 0.1, 0.5, 0.9, 0.5, 0.012, "|>" );
  ar1.DrawArrow( 0.5, 0.1, 0.5, 0.9, 0.012, "|>" );

  ar2 = TArrow();
  ar2.SetLineWidth(2);
  ar2.SetLineColor(2);
  ar2.SetFillStyle(1001);
  ar2.SetFillColor(2);

  angle = 45
  ar2.DrawArrow( 0.5, 0.5, 0.5 + 0.35 * cos(3.14 * angle / 180.), 0.5 + 0.35 * sin(3.14 * angle / 180.), 0.012, "|>" );

  qwel4 = TEllipse(0.5, 0.5, 0.015)
  qwel4.SetFillStyle(1001);
  qwel4.SetLineColor(1)
  qwel4.SetLineWidth(2)
  qwel4.Draw()

  qwel5 = TEllipse(0.5, 0.5, 0.0025)
  qwel5.SetFillStyle(1001);
  qwel5.SetLineColor(1)
  qwel5.SetLineWidth(3)
  qwel5.Draw()

  #---text
  wqtext = TText(0.48, 0.45, "z");
  wqtext.SetTextAlign(21);
  wqtext.SetTextSize(0.04);
  wqtext.Draw()

  wqtexta = TText(0.56, 0.45, "jet");
  wqtexta.SetTextAlign(21);
  wqtexta.SetTextSize(0.04);
  wqtexta.Draw()

  wqtext2 = TText(0.52, 0.9, "y");
  wqtext2.SetTextAlign(21);
  wqtext2.SetTextSize(0.04);
  wqtext2.Draw()

  wqtext3 = TText(0.9, 0.52, "x");
  wqtext3.SetTextAlign(21);
  wqtext3.SetTextSize(0.04);
  wqtext3.Draw()

  wqtext33 = TText(0.9, 0.46, "beam");
  wqtext33.SetTextAlign(21);
  wqtext33.SetTextSize(0.04);
  wqtext33.Draw()

  angle = 45
  wqtext5 = TText(0.5 + 0.37 * cos(3.14 * angle / 180.), 0.5 + 0.37 * sin(3.14 * angle / 180.), "track");
  wqtext5.SetTextAlign(21);
  wqtext5.SetTextSize(0.025);
  wqtext5.SetTextColor(2);
  wqtext5.Draw()

  wqtext51 = TLatex(0.61, 0.525, "#varphi_{TJ}");
  wqtext51.SetTextAlign(21);
  wqtext51.SetTextSize(0.03);
  wqtext51.SetTextColor(2);
  wqtext51.Draw()


  T = sys.stdin.readline()

if __name__ == '__main__': main()

