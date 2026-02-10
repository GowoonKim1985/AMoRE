// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkmseodIWorksdImuon_3dO1dO2dItmpdILinux5dO14mIGCC_11_4dIRawObjsdIlibRawObjsmIshareddIArrayS_dict
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
#include "RawObjs/ArrayS.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_ArrayS(void *p = nullptr);
   static void *newArray_ArrayS(Long_t size, void *p);
   static void delete_ArrayS(void *p);
   static void deleteArray_ArrayS(void *p);
   static void destruct_ArrayS(void *p);
   static void streamer_ArrayS(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ArrayS*)
   {
      ::ArrayS *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ArrayS >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ArrayS", ::ArrayS::Class_Version(), "RawObjs/ArrayS.hh", 25,
                  typeid(::ArrayS), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ArrayS::Dictionary, isa_proxy, 19,
                  sizeof(::ArrayS) );
      instance.SetNew(&new_ArrayS);
      instance.SetNewArray(&newArray_ArrayS);
      instance.SetDelete(&delete_ArrayS);
      instance.SetDeleteArray(&deleteArray_ArrayS);
      instance.SetDestructor(&destruct_ArrayS);
      instance.SetStreamerFunc(&streamer_ArrayS);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ArrayS*)
   {
      return GenerateInitInstanceLocal((::ArrayS*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ArrayS*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ArrayS::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ArrayS::Class_Name()
{
   return "ArrayS";
}

//______________________________________________________________________________
const char *ArrayS::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ArrayS*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ArrayS::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ArrayS*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ArrayS::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ArrayS*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ArrayS::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ArrayS*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ArrayS(void *p) {
      return  p ? new(p) ::ArrayS : new ::ArrayS;
   }
   static void *newArray_ArrayS(Long_t nElements, void *p) {
      return p ? new(p) ::ArrayS[nElements] : new ::ArrayS[nElements];
   }
   // Wrapper around operator delete
   static void delete_ArrayS(void *p) {
      delete ((::ArrayS*)p);
   }
   static void deleteArray_ArrayS(void *p) {
      delete [] ((::ArrayS*)p);
   }
   static void destruct_ArrayS(void *p) {
      typedef ::ArrayS current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ArrayS(TBuffer &buf, void *obj) {
      ((::ArrayS*)obj)->::ArrayS::Streamer(buf);
   }
} // end of namespace ROOT for class ::ArrayS

namespace {
  void TriggerDictionaryInitialization_libRawObjs_ArrayS_dict_Impl() {
    static const char* headers[] = {
"RawObjs/ArrayS.hh",
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
#line 1 "libRawObjs_ArrayS_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(Array of shorts)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RawObjs/ArrayS.hh")))  ArrayS;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libRawObjs_ArrayS_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RawObjs/ArrayS.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ArrayS", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libRawObjs_ArrayS_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libRawObjs_ArrayS_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libRawObjs_ArrayS_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libRawObjs_ArrayS_dict() {
  TriggerDictionaryInitialization_libRawObjs_ArrayS_dict_Impl();
}
