// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_4dIRawObjsdIlibRawObjsmIshareddIFChannel_dict
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
#include "RawObjs/FChannel.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_FChannel(void *p = nullptr);
   static void *newArray_FChannel(Long_t size, void *p);
   static void delete_FChannel(void *p);
   static void deleteArray_FChannel(void *p);
   static void destruct_FChannel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FChannel*)
   {
      ::FChannel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::FChannel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("FChannel", ::FChannel::Class_Version(), "RawObjs/FChannel.hh", 25,
                  typeid(::FChannel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::FChannel::Dictionary, isa_proxy, 4,
                  sizeof(::FChannel) );
      instance.SetNew(&new_FChannel);
      instance.SetNewArray(&newArray_FChannel);
      instance.SetDelete(&delete_FChannel);
      instance.SetDeleteArray(&deleteArray_FChannel);
      instance.SetDestructor(&destruct_FChannel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FChannel*)
   {
      return GenerateInitInstanceLocal((::FChannel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::FChannel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr FChannel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *FChannel::Class_Name()
{
   return "FChannel";
}

//______________________________________________________________________________
const char *FChannel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::FChannel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int FChannel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::FChannel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *FChannel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::FChannel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *FChannel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::FChannel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void FChannel::Streamer(TBuffer &R__b)
{
   // Stream an object of class FChannel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(FChannel::Class(),this);
   } else {
      R__b.WriteClassBuffer(FChannel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_FChannel(void *p) {
      return  p ? new(p) ::FChannel : new ::FChannel;
   }
   static void *newArray_FChannel(Long_t nElements, void *p) {
      return p ? new(p) ::FChannel[nElements] : new ::FChannel[nElements];
   }
   // Wrapper around operator delete
   static void delete_FChannel(void *p) {
      delete ((::FChannel*)p);
   }
   static void deleteArray_FChannel(void *p) {
      delete [] ((::FChannel*)p);
   }
   static void destruct_FChannel(void *p) {
      typedef ::FChannel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FChannel

namespace {
  void TriggerDictionaryInitialization_libRawObjs_FChannel_dict_Impl() {
    static const char* headers[] = {
"RawObjs/FChannel.hh",
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
#line 1 "libRawObjs_FChannel_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$RawObjs/FChannel.hh")))  FChannel;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawObjs_FChannel_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RawObjs/FChannel.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"FChannel", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawObjs_FChannel_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawObjs_FChannel_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawObjs_FChannel_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawObjs_FChannel_dict() {
  TriggerDictionaryInitialization_libRawObjs_FChannel_dict_Impl();
}
