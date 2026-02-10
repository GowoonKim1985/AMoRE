// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_4dIRawObjsdIlibRawObjsmIshareddIEventInfo_dict
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
#include "RawObjs/EventInfo.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_EventInfo(void *p = nullptr);
   static void *newArray_EventInfo(Long_t size, void *p);
   static void delete_EventInfo(void *p);
   static void deleteArray_EventInfo(void *p);
   static void destruct_EventInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EventInfo*)
   {
      ::EventInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EventInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("EventInfo", ::EventInfo::Class_Version(), "RawObjs/EventInfo.hh", 6,
                  typeid(::EventInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::EventInfo::Dictionary, isa_proxy, 4,
                  sizeof(::EventInfo) );
      instance.SetNew(&new_EventInfo);
      instance.SetNewArray(&newArray_EventInfo);
      instance.SetDelete(&delete_EventInfo);
      instance.SetDeleteArray(&deleteArray_EventInfo);
      instance.SetDestructor(&destruct_EventInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EventInfo*)
   {
      return GenerateInitInstanceLocal((::EventInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EventInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr EventInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *EventInfo::Class_Name()
{
   return "EventInfo";
}

//______________________________________________________________________________
const char *EventInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int EventInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EventInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EventInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EventInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EventInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void EventInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class EventInfo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EventInfo::Class(),this);
   } else {
      R__b.WriteClassBuffer(EventInfo::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EventInfo(void *p) {
      return  p ? new(p) ::EventInfo : new ::EventInfo;
   }
   static void *newArray_EventInfo(Long_t nElements, void *p) {
      return p ? new(p) ::EventInfo[nElements] : new ::EventInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_EventInfo(void *p) {
      delete ((::EventInfo*)p);
   }
   static void deleteArray_EventInfo(void *p) {
      delete [] ((::EventInfo*)p);
   }
   static void destruct_EventInfo(void *p) {
      typedef ::EventInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EventInfo

namespace {
  void TriggerDictionaryInitialization_libRawObjs_EventInfo_dict_Impl() {
    static const char* headers[] = {
"RawObjs/EventInfo.hh",
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
#line 1 "libRawObjs_EventInfo_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$RawObjs/EventInfo.hh")))  EventInfo;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawObjs_EventInfo_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RawObjs/EventInfo.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"EventInfo", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawObjs_EventInfo_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawObjs_EventInfo_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawObjs_EventInfo_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawObjs_EventInfo_dict() {
  TriggerDictionaryInitialization_libRawObjs_EventInfo_dict_Impl();
}
