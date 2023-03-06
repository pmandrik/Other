
import ROOT
from ROOT import *

import TemplateMaster as tm

base_class_template="""
#ifndef {% CLASE_NAME %}_hh
#define {% CLASE_NAME %}_hh 1

class {% CLASE_NAME %} {
  public:
    {% CLASE_NAME %}(){
      {% for CHAIN in CHAINS %}
      fChain_{% CHAIN %} = nullptr;
      {% endfor %}
    }

    void Init(){
      {% for CHAIN in CHAINS %}
      if(fChain_{% CHAIN %} == nullptr){
        cout << "{% CLASE_NAME %}.Init(): fChain_{% CHAIN %} == nullptr, return" << endl;
        return;
      };
      {% endfor %}

      // Set branch addresses
      fCurrent = -1;

      {% for MEMBER in MEMBERS %}
      fChain_{% MEMBER.chain %}->SetBranchAddress("{% MEMBER.name %}", &{% MEMBER.name %});
      {% endfor %}

      {% for MEMBER in OBJECTS %}
      fChain_{% MEMBER.chain %}->SetBranchAddress("{% MEMBER.name %}", {% MEMBER.chain %}_{% MEMBER.name %});
      {% endfor %}   

      Notify();
    }

    void Notify(){
      {% for MEMBER in MEMBERS %}
      b_{% MEMBER.name %} = fChain_{% MEMBER.chain %}->GetBranch("{% MEMBER.name %}");
      {% endfor %}

      {% for MEMBER in OBJECTS %}
      b_{% MEMBER.chain %}_{% MEMBER.name %} = fChain_{% MEMBER.chain %}->GetBranch("{% MEMBER.name %}");
      {% endfor %}
    }

    Int_t GetEntry(Long64_t entry){
      // Read contents of entry.
      {% for CHAIN in CHAINS %}
      fChain_{% CHAIN %}->GetEntry(entry);
      {% endfor %}

      return 0;
    }

    {% for CHAIN in CHAINS %}
    TTree *fChain_{% CHAIN %};
    {% endfor %}
    Int_t fCurrent; //!current Tree number in a TChain

     // Declaration of leave types
  {% for MEMBER in MEMBERS %}
    {% MEMBER.type %} {% MEMBER.name %};
  {% endfor %}

  {% for MEMBER in OBJECTS %}
    {% MEMBER.type %} {% MEMBER.chain %}_{% MEMBER.name %}[100];
  {% endfor %}

    // List of branches
  {% for MEMBER in MEMBERS %}
    TBranch *b_{% MEMBER.name %}; //!
  {% endfor %}

  {% for MEMBER in OBJECTS %}
    TBranch *b_{% MEMBER.chain %}_{% MEMBER.name %}; //!
  {% endfor %}
};

#endif
"""

file = ROOT.TFile.Open("qcd.root", "read")
file.cd("ntuple")

gDirectory.ls()

class Member():
  def __init__(self, n, t, c):
    self.name = n
    self.type = t
    self.chain = c
    self.members = []

class parcer():
  def __init__(self):
    self.members = []
    self.objects = []
    self.chains = []

  def write(self):
    dic = {}
    dic["CLASE_NAME"] = "tqgamma_Event";
    dic["MEMBERS"] = self.members
    dic["OBJECTS"] = self.objects
    dic["CHAINS"]  = set( [m.chain for m in (self.members + self.objects)] )

    f = open("tqgamma_flat_ntuple_data.hh", "w")
    f.write( tm.parce_template(base_class_template, dic) )
    f.close()

def is_general(ttree):
  for branch in ttree.GetListOfBranches():
    if branch.GetName() == ttree.GetName() + "_size" : return False
  return True

pars = parcer();
dummy = TClass()
dType = ROOT.Long(0)

for key in gDirectory.GetListOfKeys():
  ttree = key.ReadObj()
  if not ttree.InheritsFrom("TTree") : continue

  # check if it is general information
  if is_general(ttree):
    print "add general ... ", ttree.GetName();
    for branch in ttree.GetListOfBranches():
      pars.members += [ Member( branch.GetName(), branch.GetListOfLeaves().At(0).GetTypeName(), ttree.GetName()) ]
    continue
  

  # generate object class
  # mytree.MakeClass("mycode")
  print "add object ... ", ttree.GetName();
  brs = []
  for branch in ttree.GetListOfBranches():
    if branch.GetName() in brs : continue
    brs += [ branch.GetName() ]
    if branch.GetName() == ttree.GetName() + "_size":
      pars.members += [ Member( branch.GetName(), branch.GetListOfLeaves().At(0).GetTypeName(), ttree.GetName()) ]
    else : 
      pars.objects += [ Member( branch.GetName(), branch.GetListOfLeaves().At(0).GetTypeName(), ttree.GetName()) ]

pars.write()






