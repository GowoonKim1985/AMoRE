// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkkwdImyreldItmpdILinux5dO14mIGCC_11_3dIDAQdIlibDAQmIshareddICupGeneralTCB_dict
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
#include "DAQ/CupGeneralTCB.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_CupGeneralTCB(void *p = nullptr);
   static void *newArray_CupGeneralTCB(Long_t size, void *p);
   static void delete_CupGeneralTCB(void *p);
   static void deleteArray_CupGeneralTCB(void *p);
   static void destruct_CupGeneralTCB(void *p);
   static void streamer_CupGeneralTCB(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CupGeneralTCB*)
   {
      ::CupGeneralTCB *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CupGeneralTCB >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CupGeneralTCB", ::CupGeneralTCB::Class_Version(), "DAQ/CupGeneralTCB.hh", 19,
                  typeid(::CupGeneralTCB), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CupGeneralTCB::Dictionary, isa_proxy, 16,
                  sizeof(::CupGeneralTCB) );
      instance.SetNew(&new_CupGeneralTCB);
      instance.SetNewArray(&newArray_CupGeneralTCB);
      instance.SetDelete(&delete_CupGeneralTCB);
      instance.SetDeleteArray(&deleteArray_CupGeneralTCB);
      instance.SetDestructor(&destruct_CupGeneralTCB);
      instance.SetStreamerFunc(&streamer_CupGeneralTCB);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CupGeneralTCB*)
   {
      return GenerateInitInstanceLocal((::CupGeneralTCB*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CupGeneralTCB*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CupGeneralTCB::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CupGeneralTCB::Class_Name()
{
   return "CupGeneralTCB";
}

//______________________________________________________________________________
const char *CupGeneralTCB::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CupGeneralTCB*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CupGeneralTCB::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CupGeneralTCB*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CupGeneralTCB::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CupGeneralTCB*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CupGeneralTCB::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CupGeneralTCB*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CupGeneralTCB::Streamer(TBuffer &R__b)
{
   // Stream an object of class CupGeneralTCB.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CupGeneralTCB(void *p) {
      return  p ? new(p) ::CupGeneralTCB : new ::CupGeneralTCB;
   }
   static void *newArray_CupGeneralTCB(Long_t nElements, void *p) {
      return p ? new(p) ::CupGeneralTCB[nElements] : new ::CupGeneralTCB[nElements];
   }
   // Wrapper around operator delete
   static void delete_CupGeneralTCB(void *p) {
      delete ((::CupGeneralTCB*)p);
   }
   static void deleteArray_CupGeneralTCB(void *p) {
      delete [] ((::CupGeneralTCB*)p);
   }
   static void destruct_CupGeneralTCB(void *p) {
      typedef ::CupGeneralTCB current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CupGeneralTCB(TBuffer &buf, void *obj) {
      ((::CupGeneralTCB*)obj)->::CupGeneralTCB::Streamer(buf);
   }
} // end of namespace ROOT for class ::CupGeneralTCB

namespace {
  void TriggerDictionaryInitialization_libDAQ_CupGeneralTCB_dict_Impl() {
    static const char* headers[] = {
"DAQ/CupGeneralTCB.hh",
nullptr
    };
    static const char* includePaths[] = {
"/home/kkw/myrel/tmp/Linux5.14-GCC_11_3",
"/home/cupsoft/dist_CUP/releases/dev_1.0/tmp/Linux5.14-GCC_11_3",
"/home/kkw/myrel/include",
"/home/cupsoft/dist_CUP/releases/dev_1.0/include",
"/home/cupsoft/Products/ROOT/V6.26.04_2022.10/include",
"/home/cupsoft/Products/HDF5/V1.10.8/include",
"/home/cupsoft/Products/ROOT/V6.26.04_2022.10/include/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libDAQ_CupGeneralTCB_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$DAQ/CupGeneralTCB.hh")))  CupGeneralTCB;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libDAQ_CupGeneralTCB_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "DAQ/CupGeneralTCB.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CupGeneralTCB", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libDAQ_CupGeneralTCB_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libDAQ_CupGeneralTCB_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libDAQ_CupGeneralTCB_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libDAQ_CupGeneralTCB_dict() {
  TriggerDictionaryInitialization_libDAQ_CupGeneralTCB_dict_Impl();
}
