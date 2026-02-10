// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkkwdImyreldItmpdILinux5dO14mIGCC_11_3dIOnlObjsdIlibOnlObjsmIshareddIFADCRawEvent_dict
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
#include "OnlObjs/FADCRawEvent.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_FADCRawEvent(void *p);
   static void deleteArray_FADCRawEvent(void *p);
   static void destruct_FADCRawEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FADCRawEvent*)
   {
      ::FADCRawEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::FADCRawEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("FADCRawEvent", ::FADCRawEvent::Class_Version(), "OnlObjs/FADCRawEvent.hh", 6,
                  typeid(::FADCRawEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::FADCRawEvent::Dictionary, isa_proxy, 4,
                  sizeof(::FADCRawEvent) );
      instance.SetDelete(&delete_FADCRawEvent);
      instance.SetDeleteArray(&deleteArray_FADCRawEvent);
      instance.SetDestructor(&destruct_FADCRawEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FADCRawEvent*)
   {
      return GenerateInitInstanceLocal((::FADCRawEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::FADCRawEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr FADCRawEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *FADCRawEvent::Class_Name()
{
   return "FADCRawEvent";
}

//______________________________________________________________________________
const char *FADCRawEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::FADCRawEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int FADCRawEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::FADCRawEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *FADCRawEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::FADCRawEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *FADCRawEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::FADCRawEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void FADCRawEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class FADCRawEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(FADCRawEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(FADCRawEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_FADCRawEvent(void *p) {
      delete ((::FADCRawEvent*)p);
   }
   static void deleteArray_FADCRawEvent(void *p) {
      delete [] ((::FADCRawEvent*)p);
   }
   static void destruct_FADCRawEvent(void *p) {
      typedef ::FADCRawEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FADCRawEvent

namespace {
  void TriggerDictionaryInitialization_libOnlObjs_FADCRawEvent_dict_Impl() {
    static const char* headers[] = {
"OnlObjs/FADCRawEvent.hh",
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
#line 1 "libOnlObjs_FADCRawEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$OnlObjs/FADCRawEvent.hh")))  FADCRawEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libOnlObjs_FADCRawEvent_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "OnlObjs/FADCRawEvent.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"FADCRawEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libOnlObjs_FADCRawEvent_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libOnlObjs_FADCRawEvent_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libOnlObjs_FADCRawEvent_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libOnlObjs_FADCRawEvent_dict() {
  TriggerDictionaryInitialization_libOnlObjs_FADCRawEvent_dict_Impl();
}
