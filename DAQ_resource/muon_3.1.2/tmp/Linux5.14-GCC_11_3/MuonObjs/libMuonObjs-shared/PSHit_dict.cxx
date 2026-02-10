// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_3dIMuonObjsdIlibMuonObjsmIshareddIPSHit_dict
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
#include "MuonObjs/PSHit.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_PSHit(void *p = nullptr);
   static void *newArray_PSHit(Long_t size, void *p);
   static void delete_PSHit(void *p);
   static void deleteArray_PSHit(void *p);
   static void destruct_PSHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PSHit*)
   {
      ::PSHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PSHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PSHit", ::PSHit::Class_Version(), "MuonObjs/PSHit.hh", 6,
                  typeid(::PSHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PSHit::Dictionary, isa_proxy, 4,
                  sizeof(::PSHit) );
      instance.SetNew(&new_PSHit);
      instance.SetNewArray(&newArray_PSHit);
      instance.SetDelete(&delete_PSHit);
      instance.SetDeleteArray(&deleteArray_PSHit);
      instance.SetDestructor(&destruct_PSHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PSHit*)
   {
      return GenerateInitInstanceLocal((::PSHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PSHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr PSHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PSHit::Class_Name()
{
   return "PSHit";
}

//______________________________________________________________________________
const char *PSHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PSHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PSHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PSHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PSHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PSHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PSHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PSHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void PSHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class PSHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PSHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(PSHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PSHit(void *p) {
      return  p ? new(p) ::PSHit : new ::PSHit;
   }
   static void *newArray_PSHit(Long_t nElements, void *p) {
      return p ? new(p) ::PSHit[nElements] : new ::PSHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_PSHit(void *p) {
      delete ((::PSHit*)p);
   }
   static void deleteArray_PSHit(void *p) {
      delete [] ((::PSHit*)p);
   }
   static void destruct_PSHit(void *p) {
      typedef ::PSHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PSHit

namespace {
  void TriggerDictionaryInitialization_libMuonObjs_PSHit_dict_Impl() {
    static const char* headers[] = {
"MuonObjs/PSHit.hh",
nullptr
    };
    static const char* includePaths[] = {
"/home/kmseo/Works/muon_3.1.2/tmp/Linux5.14-GCC_11_3",
"/home/cupsoft/dist_CUP/releases/3.1.2/tmp/Linux5.14-GCC_11_3",
"/home/kmseo/Works/muon_3.1.2/include",
"/home/cupsoft/dist_CUP/releases/3.1.2/include",
"/home/cupsoft/Products/ROOT/V6.26.04_2022.10/include",
"/home/cupsoft/Products/ROOT/V6.26.04_2022.10/include/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libMuonObjs_PSHit_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$MuonObjs/PSHit.hh")))  PSHit;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libMuonObjs_PSHit_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "MuonObjs/PSHit.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"PSHit", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libMuonObjs_PSHit_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libMuonObjs_PSHit_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libMuonObjs_PSHit_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libMuonObjs_PSHit_dict() {
  TriggerDictionaryInitialization_libMuonObjs_PSHit_dict_Impl();
}
