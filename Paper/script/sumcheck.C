void sumcheck()
{
  TFile *f = TFile::Open("./psmd_000330_test.root");  // 원본 파일
  //  TFile *f = TFile::Open("./pwcoin_all_bot.root");  // 원본 파일  
  //  TFile *f = TFile::Open("./psmd_all_bot.root");  // 원본 파일
  TTree *t = (TTree*)f->Get("psmd");   // 트리 이름에 맞게 수정
//  TTree *t = (TTree*)f->Get("pbot");   // 트리 이름에 맞게 수정
//    TTree *t = (TTree*)f->Get("coin");   // 트리 이름에 맞게 수정

  Long64_t n = t->GetEntries();
//  Long64_t start = 38720;
//  Long64_t end   = 42515;

 
//  TFile *fout = new TFile("psmd_000330.root", "RECREATE");  
  //  TTree *coin = t->CloneTree(0);  // 구조만 복사
  int qsum1, qsum2, ievt1, ievt2, it1, it2;
  t->GetEntry(0);
  /*
  qsum1=t->GetLeaf("pqsum")->GetValue(0);
  ievt1=t->GetLeaf("pevtnum")->GetValue(0);
  it1=t->GetLeaf("ptrgtime")->GetValue(0);
  */
  
  qsum1=t->GetLeaf("qsum")->GetValue(0);
  ievt1=t->GetLeaf("evtnum")->GetValue(0);
  it1=t->GetLeaf("trgtime")->GetValue(0);
  
  
  int qsum0;
  int flag=0;
     for (Long64_t i = 1; i < n; i++) {
  //    for (Long64_t i = 1; i < 1000000; i++) {
    t->GetEntry(i);

    /*      
	  qsum2=t->GetLeaf("pqsum")->GetValue(0);
    ievt2=t->GetLeaf("pevtnum")->GetValue(0);
    it2=t->GetLeaf("ptrgtime")->GetValue(0);
    */
    
    qsum2=t->GetLeaf("qsum")->GetValue(0);
    ievt2=t->GetLeaf("evtnum")->GetValue(0);
    it2=t->GetLeaf("trgtime")->GetValue(0);
    
    // cout<<"qsum1 "<<qsum1<<" / qsum2 "<<qsum2<<endl;
    if((qsum1==qsum2)&&(ievt1==ievt2)&&(it1==it2)){
      flag=flag+1;
      if(flag==1){
	//		cout<<""<<endl;
		//		cout<<"i entry "<<i-1<<" : evtnum "<<ievt1<<", qsum "<<qsum1<<endl;
		cout<<"(i >= "<<i<<" && ";
      }
    }
    else{
      if(flag>0){
	//		cout<<"f entry "<<i-1<<" : evtnum "<<ievt1<<", qsum "<<qsum1<<endl;}
      	cout<<"i <= "<<i-1<<") ||"<<endl;}
      flag=0;
    }

    qsum1=qsum2;
    ievt1=ievt2;
    it1=it2;
  }
}
