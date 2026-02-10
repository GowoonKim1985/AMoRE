// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_4dIMuonObjsdIlibMuonObjsmIshareddIWCPmt_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "MuonObjs/WCPmt.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_WCPmt(void *p = nullptr);
   static void *newArray_WCPmt(Long_t size, void *p);
   static void delete_WCPmt(void *p);
   static void deleteArray_WCPmt(void *p);
   static void destruct_WCPmt(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::WCPmt*)
   {
      ::WCPmt *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::WCPmt >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("WCPmt", ::WCPmt::Class_Version(), "MuonObjs/WCPmt.hh", 6,
                  typeid(::WCPmt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::WCPmt::Dictionary, isa_proxy, 4,
                  sizeof(::WCPmt) );
      instance.SetNew(&new_WCPmt);
      instance.SetNewArray(&newArray_WCPmt);
      instance.SetDelete(&delete_WCPmt);
      instance.SetDeleteArray(&deleteArray_WCPmt);
      instance.SetDestructor(&destruct_WCPmt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::WCPmt*)
   {
      return GenerateInitInstanceLocal((::WCPmt*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::WCPmt*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr WCPmt::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WCPmt::Class_Name()
{
   return "WCPmt";
}

//______________________________________________________________________________
const char *WCPmt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WCPmt*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WCPmt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WCPmt*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WCPmt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WCPmt*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WCPmt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WCPmt*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void WCPmt::Streamer(TBuffer &R__b)
{
   // Stream an object of class WCPmt.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(WCPmt::Class(),this);
   } else {
      R__b.WriteClassBuffer(WCPmt::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_WCPmt(void *p) {
      return  p ? new(p) ::WCPmt : new ::WCPmt;
   }
   static void *newArray_WCPmt(Long_t nElements, void *p) {
      return p ? new(p) ::WCPmt[nElements] : new ::WCPmt[nElements];
   }
   // Wrapper around operator delete
   static void delete_WCPmt(void *p) {
      delete ((::WCPmt*)p);
   }
   static void deleteArray_WCPmt(void *p) {
      delete [] ((::WCPmt*)p);
   }
   static void destruct_WCPmt(void *p) {
      typedef ::WCPmt current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::WCPmt

namespace {
  void TriggerDictionaryInitialization_libMuonObjs_WCPmt_dict_Impl() {
    static const char* headers[] = {
"MuonObjs/WCPmt.hh",
nullptr
    };
    static const char* includePaths[] = {
"/home/kmseo/Works/muon_3.1.2/tmp/Linux5.14-GCC_11_4",
"/home/cupsoft/dist_CUP/releases/3.1.2/tmp/Linux5.14-GCC_11_4",
"/home/kmseo/Works/muon_3.1.2/include",
"/home/cupsoft/dist_CUP/releases/3.1.2/include",
"/home/cupsoft/Products/ROOT/V6.26.04_2022.10/include",
"/home/cupsoft/Products/ROOT/V6.26.04_2022.10/include/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libMuonObjs_WCPmt_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$MuonObjs/WCPmt.hh")))  WCPmt;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libMuonObjs_WCPmt_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "MuonObjs/WCPmt.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"WCPmt", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libMuonObjs_WCPmt_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libMuonObjs_WCPmt_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libMuonObjs_WCPmt_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libMuonObjs_WCPmt_dict() {
  TriggerDictionaryInitialization_libMuonObjs_WCPmt_dict_Impl();
}
