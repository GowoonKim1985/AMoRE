// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkkwdImyreldItmpdILinux5dO14mIGCC_11_3dIOnlObjsdIlibOnlObjsmIshareddIADCHeader_dict
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
#include "OnlObjs/ADCHeader.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_ADCHeader(void *p = nullptr);
   static void *newArray_ADCHeader(Long_t size, void *p);
   static void delete_ADCHeader(void *p);
   static void deleteArray_ADCHeader(void *p);
   static void destruct_ADCHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ADCHeader*)
   {
      ::ADCHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ADCHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ADCHeader", ::ADCHeader::Class_Version(), "OnlObjs/ADCHeader.hh", 10,
                  typeid(::ADCHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ADCHeader::Dictionary, isa_proxy, 4,
                  sizeof(::ADCHeader) );
      instance.SetNew(&new_ADCHeader);
      instance.SetNewArray(&newArray_ADCHeader);
      instance.SetDelete(&delete_ADCHeader);
      instance.SetDeleteArray(&deleteArray_ADCHeader);
      instance.SetDestructor(&destruct_ADCHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ADCHeader*)
   {
      return GenerateInitInstanceLocal((::ADCHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ADCHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ADCHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ADCHeader::Class_Name()
{
   return "ADCHeader";
}

//______________________________________________________________________________
const char *ADCHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADCHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ADCHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADCHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ADCHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADCHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ADCHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADCHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ADCHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class ADCHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ADCHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(ADCHeader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ADCHeader(void *p) {
      return  p ? new(p) ::ADCHeader : new ::ADCHeader;
   }
   static void *newArray_ADCHeader(Long_t nElements, void *p) {
      return p ? new(p) ::ADCHeader[nElements] : new ::ADCHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_ADCHeader(void *p) {
      delete ((::ADCHeader*)p);
   }
   static void deleteArray_ADCHeader(void *p) {
      delete [] ((::ADCHeader*)p);
   }
   static void destruct_ADCHeader(void *p) {
      typedef ::ADCHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ADCHeader

namespace {
  void TriggerDictionaryInitialization_libOnlObjs_ADCHeader_dict_Impl() {
    static const char* headers[] = {
"OnlObjs/ADCHeader.hh",
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
#line 1 "libOnlObjs_ADCHeader_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$OnlObjs/ADCHeader.hh")))  ADCHeader;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libOnlObjs_ADCHeader_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "OnlObjs/ADCHeader.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ADCHeader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libOnlObjs_ADCHeader_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libOnlObjs_ADCHeader_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libOnlObjs_ADCHeader_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libOnlObjs_ADCHeader_dict() {
  TriggerDictionaryInitialization_libOnlObjs_ADCHeader_dict_Impl();
}
