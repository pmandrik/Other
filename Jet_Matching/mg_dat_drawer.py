from ROOT import TH1D, TCanvas, THStack
from copy import copy

list_to_save = []
def save_root(something):
  list_to_save.append ( something )
  return something

def read_histograms( lines ):
  def parce_meta( line ):
    dic = {}
    dic["nbins"] = line.split(" ")[1]
    dic["hname"] = line.split(" ")[2][1:]
    dic["sample"] = line.split(" ")[6][-2:-1]
    return dic

  hists = []
  new_hist_meta = None
  new_hist_points = []
  for line in lines.split("\n"):
    if "<histogram>" in line:
      new_hist_meta = parce_meta( line )
      continue
    if not new_hist_meta : continue
    if "<\histogram>" in line:
      new_hist_meta["points"] = new_hist_points
      hists += [ copy( new_hist_meta ) ]
      new_hist_meta = None
      new_hist_points = []
      continue
    new_hist_points += [ line.split() ]

  def make_hist( meta ):
    nbins = int( meta["nbins"] )
    xmin = float( meta["points"][0][0] )
    xmax = float( meta["points"][-1][1] )
    hist = save_root( TH1D( meta["hname"] + "_" + meta["sample"], meta["hname"] + "_" + meta["sample"], nbins, xmin, xmax ) )
    for i in xrange(1, nbins+1):
      hist.SetBinContent( i, float( meta["points"][i-1][2] ) )
    pass
    hist.Print()
    return hist

  for i, meta in enumerate( hists ):
    hist = make_hist( meta )
    meta["hist"] = hist
    hists[i] = meta

  return hists

def draw_plots_djr( name, xqcut, hnames = ["log10d01"] ):
  input_file = open( name, "r" )
  
  lines = input_file.read()
  hists = read_histograms( lines )

  for name in hnames:
    canv = save_root( TCanvas(xqcut+name, xqcut+name, 640, 480) )
    color_x = 2
    hsum = None
    for i, hist in enumerate( hists ):
      if name in hist["hist"].GetName():

        hist["hist"].SetLineColor( color_x );
        hist["hist"].SetLineWidth( 2 );

        if color_x == 2:
          hist["hist"].Draw()
          hsum = hist["hist"].Clone()
        else :
          hist["hist"].Draw("same")
          hsum.Add( hist["hist"] );
        
        color_x += 1
    hsum.Draw("same")
    hsum.SetLineColor(1)
    save_root( hsum )

#draw_plots_djr( "tag_1_djrs.dat", "xqcut=30" )
#draw_plots_djr( "xcut20/tag_1_djrs.dat", "xqcut=20" )
#draw_plots_djr( "xcut25/tag_1_djrs.dat", "xqcut=25" )

#draw_plots_djr( "tag_1_pts.dat", "xqcut=30", ["pt1"] )
#draw_plots_djr( "xcut20/tag_1_pts.dat", "xqcut=20", ["pt1"] )
#draw_plots_djr( "xcut25/tag_1_pts.dat", "xqcut=25", ["pt1"] )

draw_plots_djr( "tag_1_djrs_fnal.dat", "xqcut=30" )
draw_plots_djr( "tag_1_pts_fnal.dat", "xqcut=30", ["pt1"] )





