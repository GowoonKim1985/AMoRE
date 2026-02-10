// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkkwdImyreldItmpdILinux5dO14mIGCC_11_3dIOnlObjsdIlibOnlObjsmIshareddIBuiltEvent_dict
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
#include "OnlObjs/BuiltEvent.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_BuiltEvent(void *p = nullptr);
   static void *newArray_BuiltEvent(Long_t size, void *p);
   static void delete_BuiltEvent(void *p);
   static void deleteArray_BuiltEvent(void *p);
   static void destruct_BuiltEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BuiltEvent*)
   {
      ::BuiltEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::BuiltEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("BuiltEvent", ::BuiltEvent::Class_Version(), "OnlObjs/BuiltEvent.hh", 10,
                  typeid(::BuiltEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::BuiltEvent::Dictionary, isa_proxy, 4,
                  sizeof(::BuiltEvent) );
      instance.SetNew(&new_BuiltEvent);
      instance.SetNewArray(&newArray_BuiltEvent);
      instance.SetDelete(&delete_BuiltEvent);
      instance.SetDeleteArray(&deleteArray_BuiltEvent);
      instance.SetDestructor(&destruct_BuiltEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BuiltEvent*)
   {
      return GenerateInitInstanceLocal((::BuiltEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BuiltEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr BuiltEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *BuiltEvent::Class_Name()
{
   return "BuiltEvent";
}

//______________________________________________________________________________
const char *BuiltEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BuiltEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int BuiltEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::BuiltEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *BuiltEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BuiltEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *BuiltEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::BuiltEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void BuiltEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class BuiltEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(BuiltEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(BuiltEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BuiltEvent(void *p) {
      return  p ? new(p) ::BuiltEvent : new ::BuiltEvent;
   }
   static void *newArray_BuiltEvent(Long_t nElements, void *p) {
      return p ? new(p) ::BuiltEvent[nElements] : new ::BuiltEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_BuiltEvent(void *p) {
      delete ((::BuiltEvent*)p);
   }
   static void deleteArray_BuiltEvent(void *p) {
      delete [] ((::BuiltEvent*)p);
   }
   static void destruct_BuiltEvent(void *p) {
      typedef ::BuiltEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BuiltEvent

namespace {
  void TriggerDictionaryInitialization_libOnlObjs_BuiltEvent_dict_Impl() {
    static const char* headers[] = {
"OnlObjs/BuiltEvent.hh",
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
#line 1 "libOnlObjs_BuiltEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$OnlObjs/BuiltEvent.hh")))  BuiltEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libOnlObjs_BuiltEvent_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "OnlObjs/BuiltEvent.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"BuiltEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libOnlObjs_BuiltEvent_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libOnlObjs_BuiltEvent_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libOnlObjs_BuiltEvent_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libOnlObjs_BuiltEvent_dict() {
  TriggerDictionaryInitialization_libOnlObjs_BuiltEvent_dict_Impl();
}
