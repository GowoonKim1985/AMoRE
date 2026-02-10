// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_4dIRawObjsdIlibRawObjsmIshareddIAChannelData_dict
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
#include "RawObjs/AChannelData.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_AChannelData(void *p = nullptr);
   static void *newArray_AChannelData(Long_t size, void *p);
   static void delete_AChannelData(void *p);
   static void deleteArray_AChannelData(void *p);
   static void destruct_AChannelData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AChannelData*)
   {
      ::AChannelData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AChannelData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AChannelData", ::AChannelData::Class_Version(), "RawObjs/AChannelData.hh", 25,
                  typeid(::AChannelData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AChannelData::Dictionary, isa_proxy, 4,
                  sizeof(::AChannelData) );
      instance.SetNew(&new_AChannelData);
      instance.SetNewArray(&newArray_AChannelData);
      instance.SetDelete(&delete_AChannelData);
      instance.SetDeleteArray(&deleteArray_AChannelData);
      instance.SetDestructor(&destruct_AChannelData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AChannelData*)
   {
      return GenerateInitInstanceLocal((::AChannelData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AChannelData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AChannelData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AChannelData::Class_Name()
{
   return "AChannelData";
}

//______________________________________________________________________________
const char *AChannelData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AChannelData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AChannelData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AChannelData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AChannelData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AChannelData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AChannelData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AChannelData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AChannelData::Streamer(TBuffer &R__b)
{
   // Stream an object of class AChannelData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AChannelData::Class(),this);
   } else {
      R__b.WriteClassBuffer(AChannelData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AChannelData(void *p) {
      return  p ? new(p) ::AChannelData : new ::AChannelData;
   }
   static void *newArray_AChannelData(Long_t nElements, void *p) {
      return p ? new(p) ::AChannelData[nElements] : new ::AChannelData[nElements];
   }
   // Wrapper around operator delete
   static void delete_AChannelData(void *p) {
      delete ((::AChannelData*)p);
   }
   static void deleteArray_AChannelData(void *p) {
      delete [] ((::AChannelData*)p);
   }
   static void destruct_AChannelData(void *p) {
      typedef ::AChannelData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AChannelData

namespace {
  void TriggerDictionaryInitialization_libRawObjs_AChannelData_dict_Impl() {
    static const char* headers[] = {
"RawObjs/AChannelData.hh",
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
#line 1 "libRawObjs_AChannelData_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$RawObjs/AChannelData.hh")))  AChannelData;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawObjs_AChannelData_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RawObjs/AChannelData.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AChannelData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawObjs_AChannelData_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawObjs_AChannelData_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawObjs_AChannelData_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawObjs_AChannelData_dict() {
  TriggerDictionaryInitialization_libRawObjs_AChannelData_dict_Impl();
}
