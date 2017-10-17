// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TSagePhysicsDict

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

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "TSagePhysics.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TSagePhysics(void *p = 0);
   static void *newArray_TSagePhysics(Long_t size, void *p);
   static void delete_TSagePhysics(void *p);
   static void deleteArray_TSagePhysics(void *p);
   static void destruct_TSagePhysics(void *p);
   static void streamer_TSagePhysics(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSagePhysics*)
   {
      ::TSagePhysics *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSagePhysics >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSagePhysics", ::TSagePhysics::Class_Version(), "TSagePhysics.h", 47,
                  typeid(::TSagePhysics), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSagePhysics::Dictionary, isa_proxy, 16,
                  sizeof(::TSagePhysics) );
      instance.SetNew(&new_TSagePhysics);
      instance.SetNewArray(&newArray_TSagePhysics);
      instance.SetDelete(&delete_TSagePhysics);
      instance.SetDeleteArray(&deleteArray_TSagePhysics);
      instance.SetDestructor(&destruct_TSagePhysics);
      instance.SetStreamerFunc(&streamer_TSagePhysics);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSagePhysics*)
   {
      return GenerateInitInstanceLocal((::TSagePhysics*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSagePhysics*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TSagePhysics::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSagePhysics::Class_Name()
{
   return "TSagePhysics";
}

//______________________________________________________________________________
const char *TSagePhysics::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSagePhysics*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSagePhysics::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSagePhysics*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSagePhysics::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSagePhysics*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSagePhysics::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSagePhysics*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TSagePhysics::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSagePhysics.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      //This works around a msvc bug and should be harmless on other platforms
      typedef NPL::VDetector baseClass0;
      baseClass0::Streamer(R__b);
      {
         vector<int> &R__stl =  DetectorNumber;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<int> &R__stl =  PixelNumber;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<double> &R__stl =  Energy;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<double> &R__stl =  Time;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, TSagePhysics::IsA());
   } else {
      R__c = R__b.WriteVersion(TSagePhysics::IsA(), kTRUE);
      TObject::Streamer(R__b);
      //This works around a msvc bug and should be harmless on other platforms
      typedef NPL::VDetector baseClass0;
      baseClass0::Streamer(R__b);
      {
         vector<int> &R__stl =  DetectorNumber;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<int> &R__stl =  PixelNumber;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<double> &R__stl =  Energy;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<double>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<double> &R__stl =  Time;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<double>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSagePhysics(void *p) {
      return  p ? new(p) ::TSagePhysics : new ::TSagePhysics;
   }
   static void *newArray_TSagePhysics(Long_t nElements, void *p) {
      return p ? new(p) ::TSagePhysics[nElements] : new ::TSagePhysics[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSagePhysics(void *p) {
      delete ((::TSagePhysics*)p);
   }
   static void deleteArray_TSagePhysics(void *p) {
      delete [] ((::TSagePhysics*)p);
   }
   static void destruct_TSagePhysics(void *p) {
      typedef ::TSagePhysics current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TSagePhysics(TBuffer &buf, void *obj) {
      ((::TSagePhysics*)obj)->::TSagePhysics::Streamer(buf);
   }
} // end of namespace ROOT for class ::TSagePhysics

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 458,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 458,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace {
  void TriggerDictionaryInitialization_TSagePhysicsDict_Impl() {
    static const char* headers[] = {
"TSagePhysics.h",
0
    };
    static const char* includePaths[] = {
"../Core",
"../Physics",
"../../Core",
"../../Physics",
"/opt/local/libexec/root6/include/root",
"/Users/dacox/work/nptool/NPLib/Detectors/Sage/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TSagePhysicsDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(SagePhysics structure)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TSagePhysics.h")))  TSagePhysics;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TSagePhysicsDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TSagePhysics.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TSagePhysics", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TSagePhysicsDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TSagePhysicsDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TSagePhysicsDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TSagePhysicsDict() {
  TriggerDictionaryInitialization_TSagePhysicsDict_Impl();
}
