void test_IMT()
{
  // First enable implicit multi-threading globally, so that the implicit parallelisation is on.
  // The parameter of the call specifies the number of threads to use.
  int nthreads = 4;
  ROOT::EnableImplicitMT(nthreads);

  // Open the file containing the tree
  TFile *file = TFile::Open("http://root.cern.ch/files/h1/dstarmb.root");
  // Get the tree
  TTree *tree = nullptr;
  file->GetObject<TTree>("h42", tree);

  TStopwatch t;
  // Read the branches in parallel.
  // Note that the interface does not change, the parallelisation is internal
  cout << tree -> GetEntries() << endl;
  for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
    if(i == 0) t.Start();
    if(i == tree -> GetEntries() -1){
      t.Stop();
      t.Print();
    }
    if(i != 0 && i%5000 == 0) cout << i << endl;
    tree->GetEntry(i);  // parallel read
  }
  cout << "!" << endl;

  // // IMT parallelisation can be disabled for a specific tree
  // tree->SetImplicitMT(false);
  // // If now GetEntry is invoked on the tree, the reading is sequential
  // cout << tree -> GetEntries() << endl;
  // for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
  //   if(i == 0) t.Start();
  //   if(i == tree -> GetEntries() -1){
  //     t.Stop();
  //     t.Print();
  //   }
  //   if(i != 0 && i%5000 == 0) cout << i << endl;
  //   tree->GetEntry(i);  // sequential read
  // }
  // cout << "!!" << endl;

  // // Parallel reading can be re-enabled
  // tree->SetImplicitMT(true);

  // // IMT can be also disabled globally.
  // // As a result, no tree will run GetEntry in parallel
  // ROOT::DisableImplicitMT();

  // // This is still sequential: the global flag is disabled, even if the
  // // flag for this particular tree is enabled
  // cout << tree -> GetEntries() << endl;
  // for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
  //   if(i == 0) t.Start();
  //   if(i == tree -> GetEntries() -1){
  //     t.Stop();
  //     t.Print();
  //   }
  //   if(i != 0 && i%5000 == 0) cout << i << endl;
  //   tree->GetEntry(i);  // sequential read
  // }
  // cout << "!!!" << endl;
}
