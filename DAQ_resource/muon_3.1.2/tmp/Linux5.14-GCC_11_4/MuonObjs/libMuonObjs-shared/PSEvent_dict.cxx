// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_4dIMuonObjsdIlibMuonObjsmIshareddIPSEvent_dict
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
#include "MuonObjs/PSEvent.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_PSEvent(void *p = nullptr);
   static void *newArray_PSEvent(Long_t size, void *p);
   static void delete_PSEvent(void *p);
   static void deleteArray_PSEvent(void *p);
   static void destruct_PSEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PSEvent*)
   {
      ::PSEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PSEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PSEvent", ::PSEvent::Class_Version(), "MuonObjs/PSEvent.hh", 10,
                  typeid(::PSEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PSEvent::Dictionary, isa_proxy, 4,
                  sizeof(::PSEvent) );
      instance.SetNew(&new_PSEvent);
      instance.SetNewArray(&newArray_PSEvent);
      instance.SetDelete(&delete_PSEvent);
      instance.SetDeleteArray(&deleteArray_PSEvent);
      instance.SetDestructor(&destruct_PSEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PSEvent*)
   {
      return GenerateInitInstanceLocal((::PSEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PSEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr PSEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PSEvent::Class_Name()
{
   return "PSEvent";
}

//______________________________________________________________________________
const char *PSEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PSEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PSEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PSEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PSEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PSEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PSEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PSEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void PSEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class PSEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PSEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(PSEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PSEvent(void *p) {
      return  p ? new(p) ::PSEvent : new ::PSEvent;
   }
   static void *newArray_PSEvent(Long_t nElements, void *p) {
      return p ? new(p) ::PSEvent[nElements] : new ::PSEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_PSEvent(void *p) {
      delete ((::PSEvent*)p);
   }
   static void deleteArray_PSEvent(void *p) {
      delete [] ((::PSEvent*)p);
   }
   static void destruct_PSEvent(void *p) {
      typedef ::PSEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PSEvent

namespace {
  void TriggerDictionaryInitialization_libMuonObjs_PSEvent_dict_Impl() {
    static const char* headers[] = {
"MuonObjs/PSEvent.hh",
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
#line 1 "libMuonObjs_PSEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$MuonObjs/PSEvent.hh")))  PSEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libMuonObjs_PSEvent_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "MuonObjs/PSEvent.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"PSEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libMuonObjs_PSEvent_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libMuonObjs_PSEvent_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libMuonObjs_PSEvent_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libMuonObjs_PSEvent_dict() {
  TriggerDictionaryInitialization_libMuonObjs_PSEvent_dict_Impl();
}
