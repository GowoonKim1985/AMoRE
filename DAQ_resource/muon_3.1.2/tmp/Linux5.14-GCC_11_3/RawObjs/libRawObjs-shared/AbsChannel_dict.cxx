// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_3dIRawObjsdIlibRawObjsmIshareddIAbsChannel_dict
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
#include "RawObjs/AbsChannel.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_AbsChannel(void *p = nullptr);
   static void *newArray_AbsChannel(Long_t size, void *p);
   static void delete_AbsChannel(void *p);
   static void deleteArray_AbsChannel(void *p);
   static void destruct_AbsChannel(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AbsChannel*)
   {
      ::AbsChannel *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AbsChannel >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AbsChannel", ::AbsChannel::Class_Version(), "RawObjs/AbsChannel.hh", 21,
                  typeid(::AbsChannel), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AbsChannel::Dictionary, isa_proxy, 4,
                  sizeof(::AbsChannel) );
      instance.SetNew(&new_AbsChannel);
      instance.SetNewArray(&newArray_AbsChannel);
      instance.SetDelete(&delete_AbsChannel);
      instance.SetDeleteArray(&deleteArray_AbsChannel);
      instance.SetDestructor(&destruct_AbsChannel);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AbsChannel*)
   {
      return GenerateInitInstanceLocal((::AbsChannel*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AbsChannel*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr AbsChannel::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AbsChannel::Class_Name()
{
   return "AbsChannel";
}

//______________________________________________________________________________
const char *AbsChannel::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AbsChannel*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AbsChannel::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AbsChannel*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbsChannel::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AbsChannel*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbsChannel::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AbsChannel*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void AbsChannel::Streamer(TBuffer &R__b)
{
   // Stream an object of class AbsChannel.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AbsChannel::Class(),this);
   } else {
      R__b.WriteClassBuffer(AbsChannel::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AbsChannel(void *p) {
      return  p ? new(p) ::AbsChannel : new ::AbsChannel;
   }
   static void *newArray_AbsChannel(Long_t nElements, void *p) {
      return p ? new(p) ::AbsChannel[nElements] : new ::AbsChannel[nElements];
   }
   // Wrapper around operator delete
   static void delete_AbsChannel(void *p) {
      delete ((::AbsChannel*)p);
   }
   static void deleteArray_AbsChannel(void *p) {
      delete [] ((::AbsChannel*)p);
   }
   static void destruct_AbsChannel(void *p) {
      typedef ::AbsChannel current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AbsChannel

namespace {
  void TriggerDictionaryInitialization_libRawObjs_AbsChannel_dict_Impl() {
    static const char* headers[] = {
"RawObjs/AbsChannel.hh",
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
#line 1 "libRawObjs_AbsChannel_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$RawObjs/AbsChannel.hh")))  AbsChannel;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawObjs_AbsChannel_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RawObjs/AbsChannel.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AbsChannel", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawObjs_AbsChannel_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawObjs_AbsChannel_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawObjs_AbsChannel_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawObjs_AbsChannel_dict() {
  TriggerDictionaryInitialization_libRawObjs_AbsChannel_dict_Impl();
}
