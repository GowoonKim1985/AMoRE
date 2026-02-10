#include "DAQConfig/IADCTConf.hh"
#include "DAQ/CupDAQManager.hh"

void CupDAQManager::TF_WriteData()
{
  fWriteState = READY;

  if (!ThreadWait(&fStatus, &fDoExit)) {
    return;
  }

  if (!OpenNewOutputFile()) {
    //fStatus = kERROR;
    RUNSTATE::SetError(fStatus);
    return;
  }

  bool isSMode = fADCType == ADC::SADCT || fADCType == ADC::SADCS;
  if (fADCType == ADC::IADCT) {
    auto * adc = (AbsADC *)fCont[0];
    auto * conf = (IADCTConf*)adc->GetConfig();
    if (conf->MODE() == 0) 
      isSMode = true;
  }

  const char * adcmode = isSMode ? "SADC mode" : "FADC mode";
  fLog->Info("CupDAQManager::TF_WriteData", "writing output data started as %s", adcmode);

  StartBenchmark("WriteData");
  if (isSMode) {
    switch (fOutputFileFormat)
    {
    case OUTPUT::ROOT:
      WriteSADC_MOD_ROOT();      
      break;
    case OUTPUT::HDF5:
      WriteSADC_MOD_HDF5();      
      break;
    case OUTPUT::GZIP:
      WriteSADC_MOD_GZIP();      
      break;            
    default:
      break;
    }
  }
  else {
    switch (fOutputFileFormat)
    {
    case OUTPUT::ROOT:
      WriteFADC_MOD_ROOT();      
      break;
    case OUTPUT::HDF5:
      WriteFADC_MOD_HDF5();      
      break;
    case OUTPUT::GZIP:
      WriteFADC_MOD_GZIP();      
      break;            
    default:
      break;
    }    
  }
  StopBenchmark("WriteData");

  if (fROOTFile && fROOTFile->IsOpen()) {
    fROOTFile->cd();
    fROOTTree->Write();
    fTotalWrittenDataSize += fROOTFile->GetEND();
    const char * fname = fROOTFile->GetName();
    fROOTFile->Close();
    fLog->Info("CupDAQManager::TF_WriteData", "output data %s closed", fname);
  }
  if (fHDF5File && fHDF5File->IsOpen()) {
    fTotalWrittenDataSize += fHDF5File->GetFileSize();
    const char * fname = fHDF5File->GetFilename();
    fHDF5File->Close();
    fLog->Info("CupDAQManager::TF_WriteData", "output data %s closed", fname);
  }

  fWriteState = ENDED;
  fLog->Info("CupDAQManager::TF_WriteData", "writing output data ended");
}

bool CupDAQManager::OpenNewOutputFile()
{
  int nfile = fOutputFileList.size();
  if (nfile == 0) {
    TString fname;
    if (fOutputFilename.IsNull()) {
      TString extension;
      switch (fOutputFileFormat)
      {
      case OUTPUT::ROOT: extension = "root";
        break;
      case OUTPUT::HDF5: extension = "h5";
        break;
      case OUTPUT::GZIP: extension = "gz";
        break;      
      default:
        break;
      }
      TString adcname = GetADCName(fADCType);
      TString dirname = gSystem->Getenv("RAWDATA_DIR");
      if (dirname.IsNull()) {
        fLog->Warning("CupDAQManager::OpenNewOutputFile", "RAWDATA_DIR is not set");
        fname = Form("%s_%06d.%s", adcname.Data(), fRunNumber, extension.Data());
      }
      else {
        dirname += Form("/RAW/%06d", fRunNumber);
        int isdir = gSystem->Exec(Form("test -d %s", dirname.Data()));
        if (isdir) {
          gSystem->Exec(Form("mkdir %s", dirname.Data()));
          fLog->Info("CupDAQManager::OpenNewOutputFile", "%s created", dirname.Data());
        }
        fLog->Info("CupDAQManager::OpenNewOutputFile", "%s already exist", dirname.Data());

        fname = Form("%s/%s_%06d.%s", dirname.Data(), adcname.Data(), fRunNumber, extension.Data());
      }
      fOutputFilename = fname;
    }
    else {
      fname = gSystem->BaseName(fOutputFilename);
      if (fname.Contains(".root"))
        fOutputFileFormat = OUTPUT::ROOT;
      else if (fname.Contains(".h5") || fname.Contains(".hdf"))
        fOutputFileFormat = OUTPUT::HDF5;
      else if (fname.Contains(".gz") || fname.Contains(".dat"))
        fOutputFileFormat = OUTPUT::GZIP;
      else {
        fLog->Warning("CupDAQManager::OpenNewOutputFile", "output file format is unclear");
        fLog->Info("CupDAQManager::OpenNewOutputFile", "output file is going to be written by ROOT");    
        fOutputFileFormat = OUTPUT::ROOT;
      }
    }
  }

  long retval = 0;
  const char * filename = Form("%s.%05d", fOutputFilename.Data(), nfile);

  std::unique_lock<std::mutex> lock(fWriteFileMutex);
    
  switch (fOutputFileFormat)
  {
  case OUTPUT::ROOT: retval = OpenNewROOTFile(filename);
    break;
  case OUTPUT::HDF5: retval = OpenNewHDF5File(filename);
    break;
  case OUTPUT::GZIP: retval = OpenNewGZIPFile(filename);
    break;        
  default:
    break;
  }

  lock.unlock();

  if (retval < 0)
    return false;

  fTotalWrittenDataSize += retval;
  fOutputFileList.push_back(filename);

  return true;
}