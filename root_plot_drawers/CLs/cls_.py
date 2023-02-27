

import sys
sys.path.append('/home/i/.macros')

from pROOT import *
from pROOT import memory_wrapper as _
from array import array
import math

def get_shade(gr1, gr2):
  N = gr1.GetN()
  gr = TGraph(2*N)
  for i in xrange(N):
    x, l, r = root.Double(0), root.Double(0), root.Double(0)
    gr1.GetPoint(i, x, l)
    gr2.GetPoint(i, x, r)
    gr.SetPoint(i,   x, l);
    gr.SetPoint(2*N-i-1, x, r);
  return gr

def get_cls(h1, h2, x):
  cl_b  = h1.Integral(0, x);
  cl_sb = h2.Integral(0, x);
  #print cl_b, cl_sb
  return cl_sb/(cl_b + 0.00001)

def find_cls_limit(f_b, f_s, obs, l_val):
  step = 0.001
  val = 40
  for i in xrange( int((40 - 16)/step) ):
    val -= step * i
    f_s.SetParameter(0, val)
    lim = get_cls(f_b, f_s, obs)
    if lim > l_val : break
  return val
  #f_s = _( TF1("f_b","TMath::Poisson(x, par[0])", 0, 100) )

def set_gt_nice(gr):
    gr.SetTitle(" ")
    gr.SetTitle(" ")
    gr.SetMarkerColor(8)
    gr.SetMarkerStyle(21)
    gr.SetMarkerSize(0.8)
    gr.SetLineWidth(5)
    gr.SetLineColor(root.kGreen+2)
    gr.GetXaxis().SetTitle("N_{observed}");
    gr.GetYaxis().SetTitle("CL_{s}");
    gr.GetXaxis().SetTitleSize(0.035 + 0.015)
    gr.GetYaxis().SetTitleSize(0.035 + 0.015)
    gr.GetXaxis().SetTitleOffset(0.75)
    gr.GetYaxis().SetTitleOffset(0.75)

def get_limits(f, l, r):
  val = 0.0455 * 0.5
  #val = 0.3173 * 0.5
  val *= f.Integral(l, r)
  step = 0.01

  up, down = l, r
  s_sup, s_down = 0, 0
  for i in xrange( int((r-l)/step) ):
    up = l + step*i
    s_sup += f.Eval( up ) * step
    if s_sup > val : break
  for i in xrange( int((r-l)/step) ):
    down = r - step*i
    s_down += f.Eval( down ) * step
    if s_down > val : break
  return up, down

########################################################################################
########################################################################################
########### CLs
h_b = _( TH1D("h_b", "h_b", 50, 0, 50) )
h_s = _( TH1D("h_s", "h_s", 50, 0, 50) )

f_b = _( TF1("f_b","TMath::PoissonI(x, 16)", 0, 100) )
f_bd = _( TF1("f_b","TMath::Poisson(x, 16)", 0, 100) )
f_s = _( TF1("f_s","TMath::PoissonI(x, 28)", 0, 100) )

int_N = 2000000
h_b.FillRandom("f_b", int_N);
h_s.FillRandom("f_s", int_N);

h_b *= 1./float(int_N)
h_s *= 1./float(int_N)

obs_x = 20

###########
h_b.SetLineWidth(2)
h_s.SetLineWidth(2)

h_b.SetLineColor(root.kRed)
h_s.SetLineColor(root.kBlue)

h_b_part =  _( h_b.Clone() )
h_b_part.GetXaxis().SetRange(0, obs_x)
h_b_part.SetFillColor(root.kRed)
h_b_part.SetFillStyle(3354)

h_b_part_2 =  _( h_b.Clone() )
h_b_part_2.GetXaxis().SetRange(obs_x+1, 100)
h_b_part_2.SetLineColor(root.kBlack)
h_b_part_2.SetFillColor(root.kBlack)
h_b_part_2.SetFillStyle(3409)

h_s_part =  _( h_s.Clone() )
h_s_part.GetXaxis().SetRange(0, obs_x)
h_s_part.SetFillColor(root.kBlue)
h_s_part.SetFillStyle(3409)

h_b.SetTitle(" ")
h_b.SetStats(False)
h_b.GetYaxis().SetTitle("P(N|hypothesis)")
h_b.GetXaxis().SetTitle("N")
h_b.GetYaxis().SetTitleOffset(1.2)

###########

if True:
  canv = get_canvas()
  h_b.Draw("same")
  h_b_part.Draw("same")
  h_b_part_2.Draw("same")
  h_s.Draw("same")
  h_s_part.Draw("same")

  line = _( root.TLine(obs_x,0,obs_x,0.105) )
  line.Draw()
  line.SetLineColor(root.kGreen)
  line.SetLineWidth(4)

  leg  =  _( root.TLegend(0.48,0.74,0.99,0.99) );
  leg.AddEntry(h_b,"Expected for background",  "l");
  leg.AddEntry(h_s,"Expected for background + signal","l");
  leg.AddEntry(h_b_part,"CL_{b}","f");
  leg.AddEntry(h_b_part_2,"p-value","f");
  leg.AddEntry(h_s_part,"CL_{s+b}","f");
  leg.AddEntry(line, "Observed","l");
  leg.Draw();

  canv.Print("CLs_1_pvalue.pdf")

########################################################################################
########################################################################################
########### CLs on N_observed
if False : 
  gr_cls = _( root.TGraph(50) )
  gr_clsb = _( root.TGraph(50) )
  gr_clb = _( root.TGraph(50) )
  for obs_x in xrange(50):
    gr_cls.SetPoint(obs_x, obs_x, get_cls(h_b, h_s, obs_x) );
    gr_clsb.SetPoint(obs_x, obs_x, h_s.Integral(0, obs_x) );
    gr_clb.SetPoint(obs_x, obs_x, h_b.Integral(0, obs_x) );

  canv = get_canvas()
  #canv.SetLogy()
  for gr in [gr_cls, gr_clsb, gr_clb]:
    gr.SetTitle(" ")
    gr.SetTitle(" ")
    gr.SetMarkerColor(8)
    gr.SetMarkerStyle(21)
    gr.SetMarkerSize(0.8)
    gr.SetLineWidth(5)
    gr.SetLineColor(root.kGreen+2)
    gr.GetXaxis().SetTitle("N_{observed}");
    gr.GetYaxis().SetTitle("CL_{s}");
    gr.GetXaxis().SetTitleSize(0.035 + 0.015)
    gr.GetYaxis().SetTitleSize(0.035 + 0.015)
    gr.GetXaxis().SetTitleOffset(0.75)
    gr.GetYaxis().SetTitleOffset(0.75)

  gr_clsb.SetMarkerColor(root.kBlue)
  gr_clsb.SetLineColor(38)

  gr_clb.SetMarkerColor(root.kRed)
  gr_clb.SetLineColor(root.kRed-9)

  gr_cls.Draw();
  #gr_clsb.Draw("same lP");
  #gr_clb.Draw("same lP");
  leg  =  _( root.TLegend(0.15,0.80,0.6,0.99) );
  leg.AddEntry(0, "<N_{b}> = 16", "");
  leg.AddEntry(0, "<N_{s+b}> = 28", "");
  #leg.AddEntry(gr_clb, "CL_{b}", "l");
  #leg.AddEntry(gr_clsb, "CL_{s+b}", "l");
  #leg.AddEntry(gr_cls,  "CL_{s}", "l");
  leg.Draw();

  canv.Print("CLs_2.pdf")

########################################################################################
########################################################################################
########### CLs on N_s+b
if False : 
  obs_x = 20

  gr_cls2 = _( root.TGraph(40-16) )
  for i in xrange(16, 40):
    h_s = _( TH1D("h_s" + str(i), "h_s" + str(i), 50, 0, 50) )
    f_s = _( TF1("f_s" + str(i),"TMath::PoissonI(x,"+str(i)+")", 0, 100) )

    h_s.FillRandom("f_s" + str(i), int_N);
    h_s *= 1./float(int_N)

    gr_cls2.SetPoint(i-16, i, get_cls(h_b, h_s, obs_x) );

  canv = get_canvas()
  canv.SetLogy()
  gr_cls2.SetTitle(" ")
  gr_cls2.SetMarkerColor(root.kBlue)
  gr_cls2.SetMarkerStyle(21)
  gr_cls2.SetMarkerSize(0.8)
  gr_cls2.SetLineWidth(5)
  gr_cls2.SetLineColor(38)
  gr_cls2.GetXaxis().SetTitle("<N_{s+b}>");
  gr_cls2.GetYaxis().SetTitle("CL_{s}");
  gr_cls2.GetXaxis().SetTitleSize(0.035 + 0.015)
  gr_cls2.GetYaxis().SetTitleSize(0.035 + 0.015)
  gr_cls2.GetXaxis().SetTitleOffset(0.75)
  gr_cls2.GetYaxis().SetTitleOffset(0.75)

  gr_cls2.Draw();

  leg  =  _( root.TLegend(0.48,0.80,0.99,0.99) );
  leg.AddEntry(0, "<N_{b}> = 16", "");
  leg.AddEntry(0, "N_{observed} = 20", "");
  leg.Draw();

  line = _( root.TLine(14,0.05,41,0.05) )
  line.Draw()
  line.SetLineColor(root.kMagenta)
  line.SetLineWidth(3)
  line.SetLineStyle(9);

  canv.Print("CLs_3.pdf")

########################################################################################
########################################################################################
########### CLs on N_s+b expected

if False : 
  gr_clb = _( root.TGraph(40-16) )
  gr_clsb = _( root.TGraph(40-16) )
  gr_cls2 = _( root.TGraph(40-16) )
  for i in xrange(16, 40):
    h_s = _( TH1D("h_s1" + str(i), "h_s1" + str(i), 50, 0, 50) )
    f_s = _( TF1("f_s1" + str(i),"TMath::PoissonI(x,"+str(i)+")", 0, 100) )

    h_s.FillRandom("f_s1" + str(i), int_N);
    h_s *= 1./float(int_N)

    gr_clb.SetPoint(i-16, i, get_cls(h_b, h_s, 16) );
    gr_clsb.SetPoint(i-16, i, get_cls(h_b, h_s, i) );
    gr_cls2.SetPoint(i-16, i, get_cls(h_b, h_s, obs_x) );

  canv = get_canvas()
  canv.SetLogy()

  set_gt_nice(gr_clb)
  set_gt_nice(gr_clsb)
  set_gt_nice(gr_cls2)

  gr_clb.Draw();
  gr_clb.SetLineColor(root.kRed-9)
  gr_clb.SetMarkerColor(root.kRed)
  gr_clb.GetXaxis().SetTitle("<N_{s+b}>");

  gr_cls2.Draw("same lP");
  gr_cls2.SetLineColor(38)
  gr_cls2.SetMarkerColor(root.kBlue)

  gr_clsb.Draw("same lP");
  gr_clsb.SetLineColor(root.kOrange-9)
  gr_clsb.SetMarkerColor(root.kOrange)

  leg  =  _( root.TLegend(0.11,0.11,0.58,0.33) );
  leg.AddEntry(gr_clb,  "Expected under H_{b}", "lp");
  leg.AddEntry(gr_clsb, "Expected under H_{s+b}", "lp");
  leg.AddEntry(gr_cls2, "N_{observed} = 20", "lp");
  leg.Draw();

  line = _( root.TLine(14,0.05,41,0.05) )
  line.Draw()
  line.SetLineColor(root.kMagenta)
  line.SetLineWidth(3)
  line.SetLineStyle(9);

  canv.Print("CLs_4.pdf")

########################################################################################
########################################################################################
########### CLs on N_s+b expected countur

#print get_limits(f_bd, 0., 100.)

if False : 
  gr_clb = _( root.TGraph(40-16) )
  gr_clb_1sigma_u = _( root.TGraph(40-16) )
  gr_clb_1sigma_d = _( root.TGraph(40-16) )
  gr_clb_2sigma_u = _( root.TGraph(40-16) )
  gr_clb_2sigma_d = _( root.TGraph(40-16) )

  sigma_b = 4
  sigma2_b = 8

  gr_clsb = _( root.TGraph(40-16) )
  gr_cls2 = _( root.TGraph(40-16) )
  for i in xrange(16, 40):
    f_s = _( TF1("f_s1" + str(i),"TMath::PoissonI(x,"+str(i)+")", 0, 100) )

    gr_clb.SetPoint(i-16, i, get_cls(f_b, f_s, 16) );
    gr_clb_1sigma_u.SetPoint(i-16, i, get_cls(f_b, f_s, 16-sigma_b) );
    gr_clb_1sigma_d.SetPoint(i-16, i, get_cls(f_b, f_s, 16+sigma_b) );
    gr_clb_2sigma_u.SetPoint(i-16, i, get_cls(f_b, f_s, 16-sigma2_b) );
    gr_clb_2sigma_d.SetPoint(i-16, i, get_cls(f_b, f_s, 16+sigma2_b) );

    gr_clsb.SetPoint(i-16, i, get_cls(f_b, f_s, i) );
    gr_cls2.SetPoint(i-16, i, get_cls(f_b, f_s, obs_x) );

  gr_shade_sigma  = get_shade(gr_clb_1sigma_u, gr_clb_1sigma_d)
  gr_shade_2sigma = get_shade(gr_clb_2sigma_u, gr_clb_2sigma_d)

  canv = get_canvas()
  canv.SetLogy()

  set_gt_nice(gr_clb)
  set_gt_nice(gr_clsb)
  set_gt_nice(gr_cls2)

  gr_clb.Draw();
  gr_clb.SetLineColor(root.kRed-9)
  gr_clb.SetMarkerColor(root.kRed)
  gr_clb.GetXaxis().SetTitle("<N_{s+b}>");

  grs  = [gr_clb_1sigma_u, gr_clb_1sigma_d, gr_clb_2sigma_u, gr_clb_2sigma_d]
  for gr in grs :
    gr.Draw("same lP");
    gr.SetLineColor(root.kRed)

  gr_shade_2sigma.SetFillColor(root.kSpring+10)
  gr_shade_2sigma.Draw("f same")
  gr_shade_2sigma.SetFillStyle(3001)

  gr_shade_sigma.SetFillColor(root.kGreen+1)
  gr_shade_sigma.SetFillStyle(3001)
  gr_shade_sigma.Draw("f same")

  gr_cls2.Draw("same lP");
  gr_cls2.SetLineColor(38)
  gr_cls2.SetMarkerColor(root.kBlue)

  leg  =  _( root.TLegend(0.58,0.80,0.98,0.98) );
  leg.AddEntry(gr_clb,  "Expected under H_{b}", "lp");
  leg.AddEntry(gr_shade_sigma,  "1#sigma for expected", "f");
  leg.AddEntry(gr_shade_2sigma,  "2#sigma for expected", "f");
  leg.AddEntry(gr_cls2, "N_{observed} = 20", "lp");
  leg.Draw();

  line = _( root.TLine(14,0.05,41,0.05) )
  line.Draw()
  line.SetLineColor(root.kMagenta)
  line.SetLineWidth(3)
  line.SetLineStyle(9);

  def draw_limit_line( lb, color ):
    line = _( root.TLine(lb,0.0,lb,0.05) )
    line.Draw()
    line.SetLineColor(color)
    line.SetLineWidth(4)
    line.SetLineStyle(2);

  f_s = _( TF1("f_sss","TMath::PoissonI(x, [0])", 0, 100) )
  lb = find_cls_limit(f_b, f_s, 16-sigma_b, 0.05)
  draw_limit_line(lb, root.kGreen+1)
  t = _( root.TText(lb-1,0.0003, '%.1f' % lb ) );
  t.SetTextColor( root.kGreen-1 )
  t.Draw()

  lb = find_cls_limit(f_b, f_s, 16+sigma_b, 0.05)
  draw_limit_line(lb, root.kGreen+1)
  t = _( root.TText(lb-1,0.0003, '%.1f' % lb ) );
  t.SetTextColor( root.kGreen-1 )
  t.Draw()

  lb = find_cls_limit(f_b, f_s, 16-sigma2_b, 0.05)
  draw_limit_line(lb, root.kSpring+4)
  t = _( root.TText(lb-1,0.00003, '%.1f' % lb ) );
  t.SetTextColor( root.kSpring-7 )
  t.Draw()

  lb = find_cls_limit(f_b, f_s, 16+sigma2_b, 0.05)
  draw_limit_line(lb, root.kSpring+4)
  t = _( root.TText(lb-1,0.00003, '%.1f' % lb ) );
  t.SetTextColor( root.kSpring-7 )
  t.Draw()

  print find_cls_limit(f_b, f_s, 16, 0.05)

  canv.Print("CLs_5.pdf")

########################################################################################
########################################################################################
########### Jeremys limits

if True:
  N_expected = 3.19
  N_observed = 1.

  f_b = _( TF1("f_jb","TMath::Poisson(x, [0])", 0, 100) )
  f_b.SetParameter(0, N_expected)

  f_s = _( TF1("f_js","TMath::Poisson(x, [0])", 0, 100) )

  lexp = find_cls_limit(f_b, f_s, N_expected, 0.05)
  lobs = find_cls_limit(f_b, f_s, N_observed, 0.05)

  print lexp, lobs
  print 0.11 / (lexp-N_expected) * (lobs-N_expected), 0.07
  coff = 0.11 / (lexp-N_expected)

raw_input('Exit : press any key ... ')



