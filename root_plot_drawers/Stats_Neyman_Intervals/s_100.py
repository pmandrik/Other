
from s_100_helper import *
from pROOT import memory_wrapper as _
import math

#-------------------- systematic picture ---------------------------------------
if False : 
  canv = get_canvas ()
  gr = get_graphErrors([-1., 0., 1.], [0.6, 1., 1.2], [], [])

  gr.Draw("ALP");

  gr.SetLineColor(1);
  gr.SetMarkerStyle(20);
  gr.SetMarkerSize(2.);
  gr.SetMarkerColor(root.kBlue)
  gr.SetTitle("");
  gr.SetLineColor(root.kBlue+3)
  gr.SetLineWidth(3)
  gr.SetLineStyle(5)

  color = root.TColor();
  color.SetRGB(86./255., 109./255., 126./255.);

  gr.GetYaxis().SetTitle("R")
  gr.GetYaxis().SetTitleSize(0.08)
  gr.GetYaxis().SetTitleOffset(0.4)
  gr.GetYaxis().SetTitleColor( root.kBlue-1 )
  gr.GetYaxis().SetLabelSize(0.)

  gr.GetXaxis().SetTitle("p_{i}")
  gr.GetXaxis().SetTitleSize(0.00)
  gr.GetXaxis().SetTitleOffset(0.4)
  gr.GetXaxis().SetNdivisions(3, 0, 0)
  gr.GetXaxis().SetLabelSize(0.)
  gr.GetXaxis().SetTitleColor( root.kBlue-1 )

  ar3 = root.TArrow(1.1,0.6, 1.1,1.0, 0.04,"<>");
  ar3.SetLineWidth(3);
  ar3.SetLineColor( root.kRed-3 )
  ar3.Draw();

  ar4 = root.TArrow(1.1,1.0, 1.1,1.2, 0.04,"<>");
  ar4.SetLineWidth(3);
  ar4.SetLineColor( root.kRed-3 )
  ar4.Draw();

  t = root.TLatex();
  t.SetTextAlign(22);
  t.SetTextSize(0.06);
  t.SetTextFont(72);
  t.SetTextColor( root.kBlue-1 );
  t.DrawLatex(-1, 0.5,"p-#sigma_{p}");
  t.DrawLatex( 1, 0.5,"p+#sigma_{p}");
  t.DrawLatex( 0, 0.5,"p");

  t.SetTextColor( root.kRed-3 );
  t.DrawLatex(-1-0.055, 0.63,"R^{-}");
  t.DrawLatex( 0-0.075, 1.03,"R");
  t.DrawLatex( 1-0.105, 1.23,"R^{+}");

  t.SetTextColor( root.kRed-3 );
  t.DrawLatex( 1-0.015, 1.1,"#sigma_{R}^{+}");
  t.DrawLatex( 1-0.015, 0.8,"#sigma_{R}^{-}");

  line = _(root.TLine(-1.2, 1.2, 1.2, 1.2));
  line.SetLineColor(root.kRed-3);
  line.SetLineWidth(3);
  line.SetLineStyle(7)
  line.Draw();

  line2 = _(root.TLine(-1.2, 1., 1.2, 1.));
  line2.SetLineColor(root.kRed-3);
  line2.SetLineWidth(3);
  line2.SetLineStyle(7)
  line2.Draw();

  line = root.TLine(-1.2, 0.6, 1.2, 0.6);
  line.SetLineColor(root.kRed-3);
  line.SetLineWidth(3);
  line.SetLineStyle(7)
  line.Draw();

  canv.Print("unc.pdf")
#-------------------------------------------------------------------------------
if False : 
  N_bins = 50

  names = ["signal", "background", "data"]
  for name in names:
    mp = my_prim(name)
    mp.hist(N_bins, 0, 100)

  #---#

  h_signal     = get_mp("signal").hist
  h_background = get_mp("background").hist
  h_data       = get_mp("data").hist

  #---#

  f_signal     = _( TF1("f_signal","([0]+x^4.8 - x^3.55) * (1. + (x/70.)^3) * exp( -x^0.485 / [1] )* exp( -2. / (102.-x) )",0,100) )
  f_signal.SetParameters(0., 0.50);

  f_background = _( TF1("f_background","([0]+x^1.2 - x^0.1) * (1. - (x/185.)^0.25) * exp( -x^0.31 / [1] )* exp( -1. / (102.-x) )",0,100) )
  f_background.SetParameters(0., 0.45);

  #---#

  N_events = 100000

  W1 = 0.5;
  W2 = 1.;
  WL = 1. + 0.01;

  h_signal.FillRandom("f_signal",         int(N_events * W1));
  h_background.FillRandom("f_background", int(N_events * W2));

  h_data.FillRandom("f_signal", int(N_events * W1 * WL));
  h_data.FillRandom("f_background", int(N_events * W2 * WL));
  
  h_data *= 0.001;
  h_signal *= 0.001;
  h_background *= 0.001;

  h_signal_2 = h_signal + h_background
  h_signal_3 = h_signal + h_background
  for i in xrange(1,N_bins+1):
    err = 0.01*(2.5 + 7.5)*h_signal_3.GetBinContent(i);
    h_signal_3.SetBinError(i, err);

  canv = get_canvas ()

  for mp in [h_data, h_signal_2, h_background]:
    mp.SetStats(False)
    pass


  h_data.SetMarkerStyle(20);

  color = root.gROOT.GetColor(80);
  color.SetRGB(46./255., 149./255., 66./255.);
  color2 = root.gROOT.GetColor(81);
  color2.SetRGB(1.2*86./255., 1.2*189./255., 1.2*126./255.);

  color3 = root.gROOT.GetColor(79);
  color3.SetRGB(89./255., 89./255., 89./255.);
  color4 = root.gROOT.GetColor(78);
  color4.SetRGB(1.2*126./255., 1.2*126./255., 1.2*126./255.);

  h_signal_2.SetLineColor( color.GetNumber() )
  h_signal_2.SetLineWidth( 4 )
  h_signal_2.SetFillColor( color2.GetNumber() )
  h_signal_2.GetYaxis().SetTitle("10^{3} Events");
  h_signal_2.SetTitle(" ");
  h_signal_2.GetYaxis().SetTitleSize(0.09)
  h_signal_2.GetYaxis().SetTitleOffset(0.4)

  h_signal_3.SetFillColor(1);
  h_signal_3.SetFillStyle(3356);

  h_background.SetLineColor( color3.GetNumber() )
  h_background.SetLineWidth( 4 )
  h_background.SetFillColor( color4.GetNumber() )
  #h_background.SetLineColor();

  h_signal_2.Draw("hist same");
  #h_signal_3.Draw("E2 same");
  h_background.Draw("hist same");
  h_data.Draw("P same");

  leg  = root.TLegend(0.5,0.7,0.98,0.9);
  leg.AddEntry(h_data,"data","p");
  leg.AddEntry(h_signal_2,"signal","f");
  leg.AddEntry(h_background,"background","f");
  leg.Draw();

  canv.Print("ex1.pdf")

  h_signal_up =   h_signal * 1.000001
  h_signal_down = h_signal * 1.000001
  for i in xrange(1,N_bins+1):
    err1 = h_signal_up.GetBinContent(i)  *  (1. - 0.01 * (i-N_bins) *(1. + 0.2*math.cos(3.14 * 0.3 * i)))
    err2 = h_signal_down.GetBinContent(i)* (1. + 0.01 * (1+i-N_bins) *(1. + 0.2*math.cos(3.14 * 0.125 * i)))
    h_signal_up.SetBinContent(i, err1);
    h_signal_down.SetBinContent(i, err2);

  for h in [h_signal, h_signal_up, h_signal_down]:
    h.SetLineWidth(4)
    h.SetStats(False)

  color1 = root.gROOT.GetColor(70);
  color1.SetRGB(1.2*86./255., 1.2*189./255., 1.2*126./255.);
  color2 = root.gROOT.GetColor(71);
  color2.SetRGB(1.2*86./255., 1.2*126./255., 1.2*189./255.);
  color3 = root.gROOT.GetColor(72);
  color3.SetRGB(1.2*189./255., 1.2*86./255., 1.2*126./255.);
  h_signal.SetLineColor(70)
  h_signal_up.SetLineColor(71)
  h_signal_down.SetLineColor(72)
  h_signal_up.SetTitle(" ")
  h_signal_up.GetYaxis().SetTitle("10^{3} Events");
  h_signal_up.GetYaxis().SetTitleSize(0.09)
  h_signal_up.GetYaxis().SetTitleOffset(0.4)

  canv = get_canvas ()
  h_signal_up.Draw("hist same")
  h_signal.Draw("hist same")
  h_signal_down.Draw("hist same");

  leg  = root.TLegend(0.5,0.7,0.98,0.9);
  leg.AddEntry(h_signal,     "nominal signal", "l");
  leg.AddEntry(h_signal_up,  "signal shift up",      "l");
  leg.AddEntry(h_signal_down,"signal shift down","l");
  leg.Draw();

  canv.Print("ex2.pdf")

if False :
  N_bins = 100

  names = ["signal"]
  for name in names:
    mp = my_prim(name)
    mp.hist(N_bins, 0, 100)
    mp.hist.SetStats(False)

  #---#

  h     = get_mp("signal").hist
  #---#

  f_signal     = _( TF1("f_signal","exp( -(abs(x-50)/25.)^4 ) * exp( -(abs(x-4))^0.2/2. )",0,100) )
  f_signal.SetParameters(0., 0.50);

  #---#

  N_events = 1000000

  W1 = 0.5;
  W2 = 1.;
  WL = 1. + 0.01;

  h.FillRandom("f_signal",         int(N_events * W1));

  def get_limit(hist, limit):
    print hist.GetIntegral()
    current = 0;
    for i in xrange(1,N_bins+1): current += hist.GetBinContent(i)
    current *= limit;
    for i in xrange(1,N_bins+1):
      current -= hist.GetBinContent(i);
      if current < 0. : return i;

  inter = 0.159
  #inter = 0.025
  lower = get_limit( h, inter     )
  upper = get_limit( h, 1. - inter)
  print lower, upper

  integral = 0;
  for i in xrange(1,N_bins+1): integral += h.GetBinContent(i)
  hl    = h * (100./integral)
  hc    = h * (100./integral)
  hu    = h * (100./integral)
  for i in xrange(1,N_bins+1):
    if(i > lower) : hl.SetBinContent(i, 0);
    else          : hc.SetBinContent(i, 0);

    if(i < upper) : hu.SetBinContent(i, 0);
    else          : hc.SetBinContent(i, 0);
  hc.SetTitle("P(p)")
  canv = get_canvas ()
  #h.Draw("hist");

  color2 = root.gROOT.GetColor(79);
  color2.SetRGB(0.5*189./255., 0.5*86./255., 0.5*126./255.);
  color2 = root.gROOT.GetColor(78);
  color2.SetRGB(1.2*189./255., 1.2*86./255., 1.2*126./255.);
  for h in [hl, hu]:
    h.SetLineColor(79)
    h.SetFillColor(78)
    h.SetLineWidth(3)

  color2 = root.gROOT.GetColor(81);
  color2.SetRGB(0.5*86./255., 0.5*189./255., 0.5*126./255.);
  hc.SetLineColor( 81 )
  color2 = root.gROOT.GetColor(80);
  color2.SetRGB(1.2*86./255., 1.2*189./255., 1.2*126./255.);
  hc.SetFillColor(80)
  hc.SetLineWidth(3)

  hc.Draw("hist same");
  hl.Draw("hist same");
  hu.Draw("hist same");

  hc.GetYaxis().SetTitle("P(p), %");
  hc.GetXaxis().SetTitle("p");

  t = root.TLatex();
  t.SetTextAlign(22);
  t.SetTextSize(0.08);
  t.SetTextFont(72);
  t.SetTextColor( 81 );
  t.DrawLatex(50, 1.6,"68.2%");

  t.SetTextColor( 79 );
  t.DrawLatex(15, 1.6,"15.9%");

  t.SetTextColor( 79 );
  t.DrawLatex(85, 1.6,"15.9%");

  canv.Print("di.pdf")
  pass

if True:
  poi = root.TMath.Poisson
  from math import sqrt

  val_x = []
  val_y = []
  for j in xrange(0, 1000):
    print "--------------- ", j
    N = j
    dN = pow(float(N), 0.5)
    lf = int( max(1, N - dN) )
    uf = int( N + dN          )

    yes, no = 0, 0
    for i in xrange(lf, uf+1):
      val = poi(N, i)
      yes += val
    print yes
    val_x += [ N            ]
    val_y += [ yes          ]

  canv_P = get_canvas ()
  gr_P = draw_P(val_x, val_y)

  gr_P.SetLineColor(1);
  gr_P.SetMarkerStyle(20);
  gr_P.SetMarkerSize(0.2);
  gr_P.SetMarkerColor(root.kBlue);
  gr_P.SetTitle("50 experiments");

  gr_P.GetYaxis().SetTitle("P( #mu #in [N - #sqrt{N}, N + #sqrt{N}] )")
  gr_P.GetXaxis().SetTitle("N")

  gr_P.Draw("ALP");

if False:
  f_1 = _( TF1("f_1","(x + 0.15 * (1. + x*cos(3.14 * 3.6 * x)) * cos(3.14 * 1.5 * x))*(x-0.1)",0.1,1) )
  f_2 = _( TF1("f_1","((x+0.3) + 0.15 * (1. + x*cos(3.14 * 3.6 * x)) * cos(3.14 * 1.5 * x))*(x+0.0)",0,1) )

  lenght  = 100
  grshade = _( TGraph(2*lenght) );
  for i in xrange( lenght ):
    step = 1. / 100.
    grshade.SetPoint(i,        step*i,            f_1.Eval( step*i )  )
    grshade.SetPoint(lenght+i, step*(lenght-i-1), f_2.Eval(step*(lenght-i-1)) )

  grshade.SetFillStyle(3350);
  #grshade.GetYaxis().SetRangeUser(0.05,0.15);
  grshade.GetYaxis().SetTitle("p, parameter")
  grshade.GetXaxis().SetTitle("x, data")
  grshade.SetTitle("1D Neyman construction, probability level #alpha");

  f_1.SetLineColor(root.kBlue + 4);
  f_2.SetLineColor(root.kBlue + 4);
  grshade.SetFillColor(root.kBlue - 9);

  grshade.GetXaxis().SetTitleSize(0.05)
  grshade.GetYaxis().SetTitleSize(0.05)

  canv_P = get_canvas ()
  grshade.Draw("same fA");
  grshade.GetYaxis().SetRangeUser(0,0.8);
  f_2.Draw("same P")
  f_1.Draw("same P")

  xp = 0.7
  p1 = f_1.Eval( xp )
  p2 = f_2.Eval( xp )
  xn = f_2.GetX(p1)
  
  line = _(root.TLine(0, p1, xn, p1));
  line.SetLineColor(root.kRed-7);
  line.SetLineWidth(3);
  line.SetLineStyle(7)
  line.Draw();

  line = _(root.TLine(0, p2, xp, p2));
  line.SetLineColor(root.kRed-7);
  line.SetLineWidth(3);
  line.SetLineStyle(7)
  line.Draw();

  line = _(root.TLine(xp, 0, xp, max(p1, p2)));
  line.SetLineColor(root.kRed-7);
  line.SetLineWidth(3);
  line.SetLineStyle(7)
  line.Draw();

  line = _(root.TLine(xp, min(p1, p2), xp, max(p1, p2)));
  line.SetLineColor(root.kBlue);
  line.SetLineWidth(5);
  line.SetLineStyle(7)
  line.Draw();

  line = _(root.TLine(xn, min(p1, p2), xn, 0));
  line.SetLineColor(root.kRed-7);
  line.SetLineWidth(3);
  line.SetLineStyle(7)
  line.Draw();

  line = _(root.TLine(xn, min(p1, p2), xp, min(p1, p2)));
  line.SetLineColor(root.kBlue);
  line.SetLineWidth(5);
  line.SetLineStyle(7)
  line.Draw();
  
  canv_P.Print("drei.pdf");

#-------------------------------------------------------------------------------
raw_input('Exit : press any key ... ')












