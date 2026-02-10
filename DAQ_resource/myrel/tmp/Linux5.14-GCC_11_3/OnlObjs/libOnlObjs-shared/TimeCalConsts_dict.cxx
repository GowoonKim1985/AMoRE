// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIkkwdImyreldItmpdILinux5dO14mIGCC_11_3dIOnlObjsdIlibOnlObjsmIshareddITimeCalConsts_dict
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
#include "OnlObjs/TimeCalConsts.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_TimeCalConsts(void *p = nullptr);
   static void *newArray_TimeCalConsts(Long_t size, void *p);
   static void delete_TimeCalConsts(void *p);
   static void deleteArray_TimeCalConsts(void *p);
   static void destruct_TimeCalConsts(void *p);
   static void streamer_TimeCalConsts(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TimeCalConsts*)
   {
      ::TimeCalConsts *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TimeCalConsts >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TimeCalConsts", ::TimeCalConsts::Class_Version(), "OnlObjs/TimeCalConsts.hh", 8,
                  typeid(::TimeCalConsts), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TimeCalConsts::Dictionary, isa_proxy, 16,
                  sizeof(::TimeCalConsts) );
      instance.SetNew(&new_TimeCalConsts);
      instance.SetNewArray(&newArray_TimeCalConsts);
      instance.SetDelete(&delete_TimeCalConsts);
      instance.SetDeleteArray(&deleteArray_TimeCalConsts);
      instance.SetDestructor(&destruct_TimeCalConsts);
      instance.SetStreamerFunc(&streamer_TimeCalConsts);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TimeCalConsts*)
   {
      return GenerateInitInstanceLocal((::TimeCalConsts*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TimeCalConsts*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TimeCalConsts::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TimeCalConsts::Class_Name()
{
   return "TimeCalConsts";
}

//______________________________________________________________________________
const char *TimeCalConsts::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TimeCalConsts*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TimeCalConsts::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TimeCalConsts*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TimeCalConsts::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TimeCalConsts*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TimeCalConsts::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TimeCalConsts*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TimeCalConsts::Streamer(TBuffer &R__b)
{
   // Stream an object of class TimeCalConsts.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         map<int,int> &R__stl =  calconsts;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            int R__t2;
            R__b >> R__t2;
            typedef int Value_t;
            std::pair<Value_t const, int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b.CheckByteCount(R__s, R__c, TimeCalConsts::IsA());
   } else {
      R__c = R__b.WriteVersion(TimeCalConsts::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         map<int,int> &R__stl =  calconsts;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<int,int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TimeCalConsts(void *p) {
      return  p ? new(p) ::TimeCalConsts : new ::TimeCalConsts;
   }
   static void *newArray_TimeCalConsts(Long_t nElements, void *p) {
      return p ? new(p) ::TimeCalConsts[nElements] : new ::TimeCalConsts[nElements];
   }
   // Wrapper around operator delete
   static void delete_TimeCalConsts(void *p) {
      delete ((::TimeCalConsts*)p);
   }
   static void deleteArray_TimeCalConsts(void *p) {
      delete [] ((::TimeCalConsts*)p);
   }
   static void destruct_TimeCalConsts(void *p) {
      typedef ::TimeCalConsts current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TimeCalConsts(TBuffer &buf, void *obj) {
      ((::TimeCalConsts*)obj)->::TimeCalConsts::Streamer(buf);
   }
} // end of namespace ROOT for class ::TimeCalConsts

namespace ROOT {
   static TClass *maplEintcOintgR_Dictionary();
   static void maplEintcOintgR_TClassManip(TClass*);
   static void *new_maplEintcOintgR(void *p = nullptr);
   static void *newArray_maplEintcOintgR(Long_t size, void *p);
   static void delete_maplEintcOintgR(void *p);
   static void deleteArray_maplEintcOintgR(void *p);
   static void destruct_maplEintcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,int>*)
   {
      map<int,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,int>", -2, "map", 100,
                  typeid(map<int,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,int>) );
      instance.SetNew(&new_maplEintcOintgR);
      instance.SetNewArray(&newArray_maplEintcOintgR);
      instance.SetDelete(&delete_maplEintcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOintgR);
      instance.SetDestructor(&destruct_maplEintcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,int> >()));

      ::ROOT::AddClassAlternate("map<int,int>","std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,int>*)nullptr)->GetClass();
      maplEintcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int> : new map<int,int>;
   }
   static void *newArray_maplEintcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int>[nElements] : new map<int,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOintgR(void *p) {
      delete ((map<int,int>*)p);
   }
   static void deleteArray_maplEintcOintgR(void *p) {
      delete [] ((map<int,int>*)p);
   }
   static void destruct_maplEintcOintgR(void *p) {
      typedef map<int,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,int>

namespace {
  void TriggerDictionaryInitialization_libOnlObjs_TimeCalConsts_dict_Impl() {
    static const char* headers[] = {
"OnlObjs/TimeCalConsts.hh",
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
#line 1 "libOnlObjs_TimeCalConsts_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$OnlObjs/TimeCalConsts.hh")))  TimeCalConsts;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libOnlObjs_TimeCalConsts_dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "OnlObjs/TimeCalConsts.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"TimeCalConsts", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libOnlObjs_TimeCalConsts_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libOnlObjs_TimeCalConsts_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libOnlObjs_TimeCalConsts_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libOnlObjs_TimeCalConsts_dict() {
  TriggerDictionaryInitialization_libOnlObjs_TimeCalConsts_dict_Impl();
}
