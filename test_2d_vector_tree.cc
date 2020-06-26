#include<iostream>
#include<fstream>
#include<cmath>
#include "stdlib.h"
using namespace std;

#include<map>
#include<vector>
#include<set>

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THStack.h"
#include "TF1.h"
#include "TLine.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"

#include "TRandom3.h"
#include "TGaxis.h"
#include "TStyle.h"

#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"
#include "TROOT.h"

void test_2d_vector_tree()
{
  TString roostr = "";

  cout<<endl<<" Hello World"<<endl<<endl;
  
  /////////////////////////////////////////// Write

  int run_no;
  vector<int>vc_p_id;
  vector< vector<double> >vc_vc_vtx_x;

  TFile *outfile = new TFile("out.root", "recreate");
  TTree *tree_test = new TTree("tree_test", "tree_test");

  tree_test->Branch("run_no", &run_no, "run_no/I" );
  tree_test->Branch("vc_p_id",  &vc_p_id) ;
  tree_test->Branch("vc_vc_vtx_x",  &vc_vc_vtx_x) ;

  ///

  for(int k=1; k<=2; k++) {
    vc_p_id.clear();
    vc_vc_vtx_x.clear();
    
    run_no = k;  
    for(int i=1; i<=2; i++) {
      vector<double>vc_vtx_x;
      vc_p_id.emplace_back( i );    
      for(int j=1; j<=2; j++) {
	vc_vtx_x.emplace_back( run_no *100 + i*10 + j );
      }
      vc_vc_vtx_x.emplace_back( vc_vtx_x );
    }
    tree_test->Fill();
  }

  tree_test->Write();
  outfile->Close();

  ////////////////////////////////////////////// read

  TFile *inputfile = new TFile("out.root", "read");
  TTree *tree_obj = (TTree*)inputfile->Get("tree_test");
  
  // Declaration of leaf types
  vector<int>     *br_vc_p_id = 0;
  vector<vector<double> > *br_vc_vc_vtx_x = 0;

  // List of branches
  TBranch        *b_run_no;   //!
  TBranch        *b_vc_p_id;   //!
  TBranch        *b_vc_vc_vtx_x;   //!
  
  // Set branch addresses and branch pointers  
  tree_obj->SetBranchAddress("run_no", &run_no, &b_run_no);
  tree_obj->SetBranchAddress("vc_p_id", &br_vc_p_id, &b_vc_p_id);
  tree_obj->SetBranchAddress("vc_vc_vtx_x", &br_vc_vc_vtx_x, &b_vc_vc_vtx_x);

  int entries = tree_obj->GetEntries();

  cout<<endl<<" entries "<<entries<<endl<<endl;

  vector< vector<double> >::iterator it_vc_vc;
  vector< double >::iterator it_vc;
  
  for(int ientry=0; ientry<entries; ientry++) {
    tree_obj->GetEntry(ientry);
    
    int itrack = 0;
    for( it_vc_vc=br_vc_vc_vtx_x->begin(); it_vc_vc!=br_vc_vc_vtx_x->end(); it_vc_vc++ ) {
      itrack++;
      
      int size_vc = it_vc_vc->size();
      for(int isize=0; isize<size_vc; isize++) {

	double val_p_id = br_vc_p_id->at(isize);
	double content = it_vc_vc->at(isize);
	
	cout<<TString::Format(" itrack %2d, run %d, pid %6.1f, content %6.1f", itrack, run_no, al_p_id, content)<<endl;
      }
      
    }
    
  }


  
}
