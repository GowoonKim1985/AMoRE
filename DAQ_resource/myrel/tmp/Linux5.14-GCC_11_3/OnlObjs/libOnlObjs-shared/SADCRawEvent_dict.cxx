// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkkwdImyreldItmpdILinux5dO14mIGCC_11_3dIOnlObjsdIlibOnlObjsmIshareddISADCRawEvent_dict
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
#include "OnlObjs/SADCRawEvent.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_SADCRawEvent(void *p);
   static void deleteArray_SADCRawEvent(void *p);
   static void destruct_SADCRawEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SADCRawEvent*)
   {
      ::SADCRawEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SADCRawEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("SADCRawEvent", ::SADCRawEvent::Class_Version(), "OnlObjs/SADCRawEvent.hh", 6,
                  typeid(::SADCRawEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SADCRawEvent::Dictionary, isa_proxy, 4,
                  sizeof(::SADCRawEvent) );
      instance.SetDelete(&delete_SADCRawEvent);
      instance.SetDeleteArray(&deleteArray_SADCRawEvent);
      instance.SetDestructor(&destruct_SADCRawEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SADCRawEvent*)
   {
      return GenerateInitInstanceLocal((::SADCRawEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SADCRawEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr SADCRawEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SADCRawEvent::Class_Name()
{
   return "SADCRawEvent";
}

//______________________________________________________________________________
const char *SADCRawEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SADCRawEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SADCRawEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SADCRawEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SADCRawEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SADCRawEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SADCRawEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SADCRawEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void SADCRawEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class SADCRawEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(SADCRawEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(SADCRawEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SADCRawEvent(void *p) {
      delete ((::SADCRawEvent*)p);
   }
   static void deleteArray_SADCRawEvent(void *p) {
      delete [] ((::SADCRawEvent*)p);
   }
   static void destruct_SADCRawEvent(void *p) {
      typedef ::SADCRawEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SADCRawEvent

namespace {
  void TriggerDictionaryInitialization_libOnlObjs_SADCRawEvent_dict_Impl() {
    static const char* headers[] = {
"OnlObjs/SADCRawEvent.hh",
nullptr
    };
    static const char* includePaths[] = {
"/home/kkw/myrel/tmp/Linux5.14-GCC_11_3",
"/home/cupsoft/dist_CUP/releases/dev_1.0/tmp/Linux5.14-GCC_11_3",
"/home/kkw/myrel/include",
"/home/cupsoft/dist_CUP/releases/dev_1.0/include",
"/home/cupsoft/Products/ROOT/V6.26.04_2022.10/include",
"/home/cupsoft/Products/ROOT/V6.26.04_2022.10/include/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libOnlObjs_SADCRawEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$OnlObjs/SADCRawEvent.hh")))  SADCRawEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libOnlObjs_SADCRawEvent_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "OnlObjs/SADCRawEvent.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"SADCRawEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libOnlObjs_SADCRawEvent_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libOnlObjs_SADCRawEvent_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libOnlObjs_SADCRawEvent_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libOnlObjs_SADCRawEvent_dict() {
  TriggerDictionaryInitialization_libOnlObjs_SADCRawEvent_dict_Impl();
}
