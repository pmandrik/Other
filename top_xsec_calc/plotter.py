
import ROOT
from ROOT import *

pp_energy = [7, 8, 13, 14, 27, 100]

points_energy = [1, 2, 3, 4, 5, 6]


def get_x_sec(file_name):
  file = open(file_name, "r")
  xsec = 0
  for line in file.read().split("\n"):
    if not "threads:" in line : continue;
    if not "x-section:" in line : continue
  
    subline = line.split(" ")
    xsec = subline[-2]
  # print float(xsec) / 1000.
  return float(xsec) / 1000.
  
def get_ttbar():
  xsecs = []
  for energy in pp_energy:
    file_name = "log_" + str( energy ) + ".txt"
    xsecs += [ get_x_sec(file_name) ]
    
  print "ttbar xsecs", xsecs
  return xsecs

def get_t_ch():
  xsecs = []
  for energy in pp_energy:
    file_name_t    = "log_st_161_" + str( energy ) + ".txt"
    file_name_tbar = "log_st_166_" + str( energy ) + ".txt"
    xsec_t    = get_x_sec(file_name_t)
    xsec_tbar = get_x_sec(file_name_tbar)
    xsecs += [ xsec_t + xsec_tbar ]
  print "t-ch xsecs", xsecs
  return xsecs

def get_s_ch():
  xsecs = []
  for energy in pp_energy:
    file_name_t    = "log_st_alt_171_" + str( energy ) + ".txt"
    file_name_tbar = "log_st_alt_176_" + str( energy ) + ".txt"
    xsec_t    = get_x_sec(file_name_t)
    xsec_tbar = get_x_sec(file_name_tbar)
    xsecs += [ xsec_t + xsec_tbar ]
  print "s-ch xsecs", xsecs
  return xsecs

def get_tW_ch():
  xsecs = []
  for energy in pp_energy:
    file_name    = "log_tW_" + str( energy ) + ".txt"
    xsec    = get_x_sec(file_name)
    xsecs += [ xsec * 2 * 9 ]
  print "s-ch xsecs", xsecs
  return xsecs

def get_graph(xsecs):
  graph = TGraph(len(pp_energy));
  for i, point, energy in zip(xrange(len(xsecs)), xsecs, pp_energy):
    graph.SetPoint(i, energy, point)
  return graph

def tune_graph(graph, N):
  graph.SetLineColor(N)
  graph.SetLineWidth(2) 
  graph.SetMarkerStyle(20) 
  graph.SetMarkerColor(N)
  graph.SetMarkerSize(1.5) 
  return graph

items = []
exp_points_x, exp_points_y, exp_error, exp_style, exp_color, exp_ref = [], [], [], [], [], []
def make_plot():
  global items
  points_tbar = get_ttbar()
  points_tch  = get_t_ch()
  points_sch  = get_s_ch()
  points_tW  = get_tW_ch()

  graph_ttbar = tune_graph(get_graph(points_tbar), 98)
  graph_tch   = tune_graph(get_graph(points_tch),  8)
  graph_sch   = tune_graph(get_graph(points_sch),  61)
  graph_tW    = tune_graph(get_graph(points_tW),   51)

  graph_ttbar.GetHistogram().SetMinimum(0.0001); 
  graph_ttbar.GetHistogram().SetMaximum(points_tbar[-1] * 2);   

  canv = TCanvas("plot", "", 1000, 600)
  #graph_ttbar.GetXaxis().SetRangeUser(0., 150.);
  canv.SetLogy();
  canv.SetLogx();
  
  graph_ttbar.Draw("AL")
  graph_tch.Draw("L")
  graph_sch.Draw("L")
  graph_tW.Draw("L")

  ymin = 1
  ymax = 100000
  graph_ttbar.GetHistogram().SetMinimum( ymin ); 
  graph_ttbar.GetHistogram().SetMaximum( ymax );   
  graph_ttbar.GetXaxis().SetLimits(6,200);
  graph_ttbar.GetXaxis().SetLabelSize(0);

  graph_ttbar.GetYaxis().SetTitle( "#sigma [pb]" )
  graph_ttbar.GetXaxis().SetTitle( "#sqrt{s} [TeV]" )
  graph_ttbar.SetTitle("")

  latex_axis = TLatex()
  latex_axis.SetTextSize(0.035);
  latex_axis.SetTextAlign(21);
  latex_axis.SetTextFont(42);
  latex_axis.SetNDC()

  latex_ref = TLatex()
  latex_ref.SetTextSize(0.030);
  latex_ref.SetTextAlign(12);
  #latex_axis.SetTextFont(42);
  latex_ref.SetNDC()

  posses = [0.135, 0.165, 0.27, 0.305, 0.445, 0.74]
  for pos, energy in zip(posses, pp_energy):
    line = TLine(energy, ymin, energy, ymax);
    line.SetLineColor( 17 );
    line.Draw();
    line.SetLineWidth(3)
    line.SetLineStyle(7)
    items += [line]

    dx = 0
    if energy == 13 : dx = -0.45
    if energy == 14 : dx = 0.45
    latex_axis.DrawLatex(pos, 0.065, str(energy) )

  for name, graph in zip(["t#bar{t}", "t-channel", "s-channel", "tW-channel"], [graph_ttbar, graph_tch, graph_sch, graph_tW]) : 
    latex_process = TLatex()
    latex_process.SetTextSize(0.04);
    latex_process.SetTextFont(42);
    latex_process.SetTextColor( graph.GetLineColor() );
    latex_process.SetTextAlign(12);

    x, y = ROOT.Double(0), ROOT.Double(0)
    print graph.GetPoint( (len(pp_energy)-1), x, y )
    latex_process.DrawLatex( 107, y, name )

  graph_ttbar.Draw("L")
  graph_tch.Draw("L")
  graph_sch.Draw("L")
  graph_tW.Draw("L")
  
  def get_error(list):
    value = 0
    for err in list:
      value += err*err
    return pow(value, 0.5)

  def add_point(x, y, err, style, color, ref):
    global exp_points_x, exp_points_y, exp_error, exp_style, exp_color, exp_ref
    exp_points_x += [x]
    exp_points_y += [y]
    exp_error    += [err]
    exp_style    += [style]
    exp_color    += [color]
    exp_ref      += [ref]

  add_point(7,   173, get_error([2, 8, 6])      , 20, 98, "ATLAS-CONF-2012-134, CMS-PAS-TOP-12-003")
  add_point(8, 241.5, get_error([1.4, 5.7, 6.2]), 21, 98, "ATLAS-CONF-2014-053, CMS-PAS-TOP-14-016")
  add_point(13,  818, get_error([8, 27, 19])    , 22, 98, "ATLAS, PLB 761 (2016) 136")

  add_point(8,  85, get_error([4, 11, 3])       , 23, 8 , "ATLAS-CONF-2013-098, CMS-PAS-TOP-12-002")
  add_point(8,  25.0, get_error([1.4, 4.4, 0.7]), 33, 51, "ATLAS-CONF-2014-052, CMS-PAS-TOP-14-009")

  dy = 0
  for x, y, err, style, color, ref in zip(exp_points_x, exp_points_y, exp_error, exp_style, exp_color, exp_ref):
    m = TMarker(x, y, style);
    m.SetMarkerSize(2.0)
    m.SetMarkerColor( color )
    m.Draw();
    items += [m]

    mx = TMarker(0.500, 0.135 + dy, style);
    mx.SetMarkerSize(1.5)
    mx.SetMarkerColor( color )
    mx.Draw();
    mx.SetNDC()
    latex_ref.DrawLatex(0.52, 0.135 + dy, ref )
    items += [ mx ]
    dy += 0.04

    gr = TGraphErrors(1);
    gr.SetPoint(0, x,y)
    gr.SetPoint(0, x,y)
    gr.SetPointError(0, 0, err)
    #gr.SetLError(
    gr.Draw("P");
    items += [gr]

  canv.RedrawAxis();

  canv.Update()
  canv.Print("xsec_tbar.pdf")
  items += [canv]

make_plot()












