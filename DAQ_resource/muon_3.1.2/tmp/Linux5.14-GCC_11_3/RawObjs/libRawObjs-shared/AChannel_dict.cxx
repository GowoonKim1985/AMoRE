// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_3dIRawObjsdIlibRawObjsmIshareddIAChannel_dict
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
#include "RawObjs/AChannel.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_AChannel(void *p = nullptr);
   static void *newArray_AChannel(Long_t size, void *p);
   static void delete_AChannel(void *p);
   static void deleteArray_AChannel(void *p);
   static void destruct_AChannel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AChannel*)
   {
      ::AChannel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AChannel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AChannel", ::AChannel::Class_Version(), "RawObjs/AChannel.hh", 21,
                  typeid(::AChannel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AChannel::Dictionary, isa_proxy, 4,
                  sizeof(::AChannel) );
      instance.SetNew(&new_AChannel);
      instance.SetNewArray(&newArray_AChannel);
      instance.SetDelete(&delete_AChannel);
      instance.SetDeleteArray(&deleteArray_AChannel);
      instance.SetDestructor(&destruct_AChannel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AChannel*)
   {
      return GenerateInitInstanceLocal((::AChannel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AChannel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AChannel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AChannel::Class_Name()
{
   return "AChannel";
}

//______________________________________________________________________________
const char *AChannel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AChannel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AChannel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AChannel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AChannel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AChannel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AChannel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AChannel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AChannel::Streamer(TBuffer &R__b)
{
   // Stream an object of class AChannel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AChannel::Class(),this);
   } else {
      R__b.WriteClassBuffer(AChannel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AChannel(void *p) {
      return  p ? new(p) ::AChannel : new ::AChannel;
   }
   static void *newArray_AChannel(Long_t nElements, void *p) {
      return p ? new(p) ::AChannel[nElements] : new ::AChannel[nElements];
   }
   // Wrapper around operator delete
   static void delete_AChannel(void *p) {
      delete ((::AChannel*)p);
   }
   static void deleteArray_AChannel(void *p) {
      delete [] ((::AChannel*)p);
   }
   static void destruct_AChannel(void *p) {
      typedef ::AChannel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AChannel

namespace {
  void TriggerDictionaryInitialization_libRawObjs_AChannel_dict_Impl() {
    static const char* headers[] = {
"RawObjs/AChannel.hh",
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
#line 1 "libRawObjs_AChannel_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$RawObjs/AChannel.hh")))  AChannel;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawObjs_AChannel_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RawObjs/AChannel.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AChannel", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawObjs_AChannel_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawObjs_AChannel_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawObjs_AChannel_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawObjs_AChannel_dict() {
  TriggerDictionaryInitialization_libRawObjs_AChannel_dict_Impl();
}
