// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkkwdImyreldItmpdILinux5dO14mIGCC_11_3dIOnlObjsdIlibOnlObjsmIshareddIAbsADCRaw_dict
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
#include "OnlObjs/AbsADCRaw.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_AbsADCRaw(void *p);
   static void deleteArray_AbsADCRaw(void *p);
   static void destruct_AbsADCRaw(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AbsADCRaw*)
   {
      ::AbsADCRaw *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AbsADCRaw >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AbsADCRaw", ::AbsADCRaw::Class_Version(), "OnlObjs/AbsADCRaw.hh", 11,
                  typeid(::AbsADCRaw), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AbsADCRaw::Dictionary, isa_proxy, 4,
                  sizeof(::AbsADCRaw) );
      instance.SetDelete(&delete_AbsADCRaw);
      instance.SetDeleteArray(&deleteArray_AbsADCRaw);
      instance.SetDestructor(&destruct_AbsADCRaw);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AbsADCRaw*)
   {
      return GenerateInitInstanceLocal((::AbsADCRaw*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AbsADCRaw*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AbsADCRaw::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AbsADCRaw::Class_Name()
{
   return "AbsADCRaw";
}

//______________________________________________________________________________
const char *AbsADCRaw::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AbsADCRaw*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AbsADCRaw::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AbsADCRaw*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbsADCRaw::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AbsADCRaw*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbsADCRaw::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AbsADCRaw*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AbsADCRaw::Streamer(TBuffer &R__b)
{
   // Stream an object of class AbsADCRaw.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AbsADCRaw::Class(),this);
   } else {
      R__b.WriteClassBuffer(AbsADCRaw::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AbsADCRaw(void *p) {
      delete ((::AbsADCRaw*)p);
   }
   static void deleteArray_AbsADCRaw(void *p) {
      delete [] ((::AbsADCRaw*)p);
   }
   static void destruct_AbsADCRaw(void *p) {
      typedef ::AbsADCRaw current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AbsADCRaw

namespace {
  void TriggerDictionaryInitialization_libOnlObjs_AbsADCRaw_dict_Impl() {
    static const char* headers[] = {
"OnlObjs/AbsADCRaw.hh",
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
#line 1 "libOnlObjs_AbsADCRaw_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$OnlObjs/AbsADCRaw.hh")))  AbsADCRaw;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libOnlObjs_AbsADCRaw_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "OnlObjs/AbsADCRaw.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AbsADCRaw", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libOnlObjs_AbsADCRaw_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libOnlObjs_AbsADCRaw_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libOnlObjs_AbsADCRaw_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libOnlObjs_AbsADCRaw_dict() {
  TriggerDictionaryInitialization_libOnlObjs_AbsADCRaw_dict_Impl();
}
