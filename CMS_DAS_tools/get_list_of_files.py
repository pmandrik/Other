
from cmssw_das_client import *

def prind_files(dataset):
	answer = get_data("file dataset="+dataset)
	for fdata in answer["data"]:
#	print fdata["file"][0]
		print fdata["file"][0]["name"]

dataset_line = """/SingleMuon/CMSSW_10_6_0_pre3-105X_dataRun2_v8_RelVal_sigMu2017F-v2/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre3-105X_dataRun2_v8_RelVal_sigMu2017E-v1/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre3-105X_dataRun2_v8_RelVal_sigMu2017D-v1/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre3-105X_dataRun2_v8_RelVal_sigMu2017C-v1/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre3-105X_dataRun2_v8_RelVal_sigMu2017B-v1/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre4-106X_dataRun2_tracker2017_v1_RelVal_sigMu2017F-v1/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre4-106X_dataRun2_tracker2017_v1_RelVal_sigMu2017E-v1/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre4-106X_dataRun2_tracker2017_v1_RelVal_sigMu2017D-v1/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre4-106X_dataRun2_tracker2017_v1_RelVal_sigMu2017C-v1/MINIAOD
/SingleMuon/CMSSW_10_6_0_pre4-106X_dataRun2_tracker2017_v1_RelVal_sigMu2017B-v1/MINIAOD"""

datasets = dataset_line.split("\n") 

for dataset in datasets: 
#	print dataset
	prind_files( dataset )
	print "---"
