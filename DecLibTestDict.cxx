// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME DecLibTestDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
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
#include "DecLibTest.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_Pulse(void *p = nullptr);
   static void *newArray_Pulse(Long_t size, void *p);
   static void delete_Pulse(void *p);
   static void deleteArray_Pulse(void *p);
   static void destruct_Pulse(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Pulse*)
   {
      ::Pulse *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Pulse >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Pulse", ::Pulse::Class_Version(), "DecLibTest.hh", 145,
                  typeid(::Pulse), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Pulse::Dictionary, isa_proxy, 4,
                  sizeof(::Pulse) );
      instance.SetNew(&new_Pulse);
      instance.SetNewArray(&newArray_Pulse);
      instance.SetDelete(&delete_Pulse);
      instance.SetDeleteArray(&deleteArray_Pulse);
      instance.SetDestructor(&destruct_Pulse);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Pulse*)
   {
      return GenerateInitInstanceLocal(static_cast<::Pulse*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Pulse*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Event(void *p = nullptr);
   static void *newArray_Event(Long_t size, void *p);
   static void delete_Event(void *p);
   static void deleteArray_Event(void *p);
   static void destruct_Event(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Event*)
   {
      ::Event *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Event >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Event", ::Event::Class_Version(), "DecLibTest.hh", 157,
                  typeid(::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Event::Dictionary, isa_proxy, 4,
                  sizeof(::Event) );
      instance.SetNew(&new_Event);
      instance.SetNewArray(&newArray_Event);
      instance.SetDelete(&delete_Event);
      instance.SetDeleteArray(&deleteArray_Event);
      instance.SetDestructor(&destruct_Event);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Event*)
   {
      return GenerateInitInstanceLocal(static_cast<::Event*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Event*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecFile(void *p = nullptr);
   static void *newArray_DecFile(Long_t size, void *p);
   static void delete_DecFile(void *p);
   static void deleteArray_DecFile(void *p);
   static void destruct_DecFile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DecFile*)
   {
      ::DecFile *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DecFile >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DecFile", ::DecFile::Class_Version(), "DecLibTest.hh", 174,
                  typeid(::DecFile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DecFile::Dictionary, isa_proxy, 4,
                  sizeof(::DecFile) );
      instance.SetNew(&new_DecFile);
      instance.SetNewArray(&newArray_DecFile);
      instance.SetDelete(&delete_DecFile);
      instance.SetDeleteArray(&deleteArray_DecFile);
      instance.SetDestructor(&destruct_DecFile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DecFile*)
   {
      return GenerateInitInstanceLocal(static_cast<::DecFile*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::DecFile*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DecManager(void *p);
   static void deleteArray_DecManager(void *p);
   static void destruct_DecManager(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DecManager*)
   {
      ::DecManager *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DecManager >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DecManager", ::DecManager::Class_Version(), "DecLibTest.hh", 206,
                  typeid(::DecManager), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DecManager::Dictionary, isa_proxy, 4,
                  sizeof(::DecManager) );
      instance.SetDelete(&delete_DecManager);
      instance.SetDeleteArray(&deleteArray_DecManager);
      instance.SetDestructor(&destruct_DecManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DecManager*)
   {
      return GenerateInitInstanceLocal(static_cast<::DecManager*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::DecManager*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Pulse::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Pulse::Class_Name()
{
   return "Pulse";
}

//______________________________________________________________________________
const char *Pulse::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Pulse*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Pulse::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Pulse*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Pulse::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Pulse*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Pulse::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Pulse*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Event::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Event::Class_Name()
{
   return "Event";
}

//______________________________________________________________________________
const char *Event::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Event::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Event::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Event::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DecFile::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DecFile::Class_Name()
{
   return "DecFile";
}

//______________________________________________________________________________
const char *DecFile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DecFile*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DecFile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DecFile*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DecFile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DecFile*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DecFile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DecFile*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DecManager::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DecManager::Class_Name()
{
   return "DecManager";
}

//______________________________________________________________________________
const char *DecManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DecManager*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DecManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DecManager*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DecManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DecManager*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DecManager::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DecManager*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Pulse::Streamer(TBuffer &R__b)
{
   // Stream an object of class Pulse.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Pulse::Class(),this);
   } else {
      R__b.WriteClassBuffer(Pulse::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Pulse(void *p) {
      return  p ? new(p) ::Pulse : new ::Pulse;
   }
   static void *newArray_Pulse(Long_t nElements, void *p) {
      return p ? new(p) ::Pulse[nElements] : new ::Pulse[nElements];
   }
   // Wrapper around operator delete
   static void delete_Pulse(void *p) {
      delete (static_cast<::Pulse*>(p));
   }
   static void deleteArray_Pulse(void *p) {
      delete [] (static_cast<::Pulse*>(p));
   }
   static void destruct_Pulse(void *p) {
      typedef ::Pulse current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Pulse

//______________________________________________________________________________
void Event::Streamer(TBuffer &R__b)
{
   // Stream an object of class Event.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Event::Class(),this);
   } else {
      R__b.WriteClassBuffer(Event::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Event(void *p) {
      return  p ? new(p) ::Event : new ::Event;
   }
   static void *newArray_Event(Long_t nElements, void *p) {
      return p ? new(p) ::Event[nElements] : new ::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_Event(void *p) {
      delete (static_cast<::Event*>(p));
   }
   static void deleteArray_Event(void *p) {
      delete [] (static_cast<::Event*>(p));
   }
   static void destruct_Event(void *p) {
      typedef ::Event current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Event

//______________________________________________________________________________
void DecFile::Streamer(TBuffer &R__b)
{
   // Stream an object of class DecFile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DecFile::Class(),this);
   } else {
      R__b.WriteClassBuffer(DecFile::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DecFile(void *p) {
      return  p ? new(p) ::DecFile : new ::DecFile;
   }
   static void *newArray_DecFile(Long_t nElements, void *p) {
      return p ? new(p) ::DecFile[nElements] : new ::DecFile[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecFile(void *p) {
      delete (static_cast<::DecFile*>(p));
   }
   static void deleteArray_DecFile(void *p) {
      delete [] (static_cast<::DecFile*>(p));
   }
   static void destruct_DecFile(void *p) {
      typedef ::DecFile current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::DecFile

//______________________________________________________________________________
void DecManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class DecManager.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DecManager::Class(),this);
   } else {
      R__b.WriteClassBuffer(DecManager::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DecManager(void *p) {
      delete (static_cast<::DecManager*>(p));
   }
   static void deleteArray_DecManager(void *p) {
      delete [] (static_cast<::DecManager*>(p));
   }
   static void destruct_DecManager(void *p) {
      typedef ::DecManager current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::DecManager

namespace ROOT {
   static TClass *vectorlEboolgR_Dictionary();
   static void vectorlEboolgR_TClassManip(TClass*);
   static void *new_vectorlEboolgR(void *p = nullptr);
   static void *newArray_vectorlEboolgR(Long_t size, void *p);
   static void delete_vectorlEboolgR(void *p);
   static void deleteArray_vectorlEboolgR(void *p);
   static void destruct_vectorlEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bool>*)
   {
      vector<bool> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bool>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bool>", -2, "vector", 596,
                  typeid(vector<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEboolgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<bool>) );
      instance.SetNew(&new_vectorlEboolgR);
      instance.SetNewArray(&newArray_vectorlEboolgR);
      instance.SetDelete(&delete_vectorlEboolgR);
      instance.SetDeleteArray(&deleteArray_vectorlEboolgR);
      instance.SetDestructor(&destruct_vectorlEboolgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bool> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<bool>","std::vector<bool, std::allocator<bool> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<bool>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<bool>*>(nullptr))->GetClass();
      vectorlEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEboolgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<bool> : new vector<bool>;
   }
   static void *newArray_vectorlEboolgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<bool>[nElements] : new vector<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEboolgR(void *p) {
      delete (static_cast<vector<bool>*>(p));
   }
   static void deleteArray_vectorlEboolgR(void *p) {
      delete [] (static_cast<vector<bool>*>(p));
   }
   static void destruct_vectorlEboolgR(void *p) {
      typedef vector<bool> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<bool>

namespace ROOT {
   static TClass *vectorlEULong64_tgR_Dictionary();
   static void vectorlEULong64_tgR_TClassManip(TClass*);
   static void *new_vectorlEULong64_tgR(void *p = nullptr);
   static void *newArray_vectorlEULong64_tgR(Long_t size, void *p);
   static void delete_vectorlEULong64_tgR(void *p);
   static void deleteArray_vectorlEULong64_tgR(void *p);
   static void destruct_vectorlEULong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ULong64_t>*)
   {
      vector<ULong64_t> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ULong64_t>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ULong64_t>", -2, "vector", 389,
                  typeid(vector<ULong64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEULong64_tgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ULong64_t>) );
      instance.SetNew(&new_vectorlEULong64_tgR);
      instance.SetNewArray(&newArray_vectorlEULong64_tgR);
      instance.SetDelete(&delete_vectorlEULong64_tgR);
      instance.SetDeleteArray(&deleteArray_vectorlEULong64_tgR);
      instance.SetDestructor(&destruct_vectorlEULong64_tgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ULong64_t> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<ULong64_t>","std::vector<unsigned long long, std::allocator<unsigned long long> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<ULong64_t>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEULong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<ULong64_t>*>(nullptr))->GetClass();
      vectorlEULong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEULong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEULong64_tgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ULong64_t> : new vector<ULong64_t>;
   }
   static void *newArray_vectorlEULong64_tgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<ULong64_t>[nElements] : new vector<ULong64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEULong64_tgR(void *p) {
      delete (static_cast<vector<ULong64_t>*>(p));
   }
   static void deleteArray_vectorlEULong64_tgR(void *p) {
      delete [] (static_cast<vector<ULong64_t>*>(p));
   }
   static void destruct_vectorlEULong64_tgR(void *p) {
      typedef vector<ULong64_t> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<ULong64_t>

namespace ROOT {
   static TClass *vectorlEPulsegR_Dictionary();
   static void vectorlEPulsegR_TClassManip(TClass*);
   static void *new_vectorlEPulsegR(void *p = nullptr);
   static void *newArray_vectorlEPulsegR(Long_t size, void *p);
   static void delete_vectorlEPulsegR(void *p);
   static void deleteArray_vectorlEPulsegR(void *p);
   static void destruct_vectorlEPulsegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Pulse>*)
   {
      vector<Pulse> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Pulse>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Pulse>", -2, "vector", 389,
                  typeid(vector<Pulse>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPulsegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Pulse>) );
      instance.SetNew(&new_vectorlEPulsegR);
      instance.SetNewArray(&newArray_vectorlEPulsegR);
      instance.SetDelete(&delete_vectorlEPulsegR);
      instance.SetDeleteArray(&deleteArray_vectorlEPulsegR);
      instance.SetDestructor(&destruct_vectorlEPulsegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Pulse> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<Pulse>","std::vector<Pulse, std::allocator<Pulse> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<Pulse>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPulsegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<Pulse>*>(nullptr))->GetClass();
      vectorlEPulsegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPulsegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPulsegR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Pulse> : new vector<Pulse>;
   }
   static void *newArray_vectorlEPulsegR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Pulse>[nElements] : new vector<Pulse>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPulsegR(void *p) {
      delete (static_cast<vector<Pulse>*>(p));
   }
   static void deleteArray_vectorlEPulsegR(void *p) {
      delete [] (static_cast<vector<Pulse>*>(p));
   }
   static void destruct_vectorlEPulsegR(void *p) {
      typedef vector<Pulse> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<Pulse>

namespace ROOT {
   static TClass *vectorlEPulsemUgR_Dictionary();
   static void vectorlEPulsemUgR_TClassManip(TClass*);
   static void *new_vectorlEPulsemUgR(void *p = nullptr);
   static void *newArray_vectorlEPulsemUgR(Long_t size, void *p);
   static void delete_vectorlEPulsemUgR(void *p);
   static void deleteArray_vectorlEPulsemUgR(void *p);
   static void destruct_vectorlEPulsemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Pulse*>*)
   {
      vector<Pulse*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Pulse*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Pulse*>", -2, "vector", 389,
                  typeid(vector<Pulse*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPulsemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Pulse*>) );
      instance.SetNew(&new_vectorlEPulsemUgR);
      instance.SetNewArray(&newArray_vectorlEPulsemUgR);
      instance.SetDelete(&delete_vectorlEPulsemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEPulsemUgR);
      instance.SetDestructor(&destruct_vectorlEPulsemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Pulse*> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<Pulse*>","std::vector<Pulse*, std::allocator<Pulse*> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<Pulse*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPulsemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<Pulse*>*>(nullptr))->GetClass();
      vectorlEPulsemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPulsemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPulsemUgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Pulse*> : new vector<Pulse*>;
   }
   static void *newArray_vectorlEPulsemUgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Pulse*>[nElements] : new vector<Pulse*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPulsemUgR(void *p) {
      delete (static_cast<vector<Pulse*>*>(p));
   }
   static void deleteArray_vectorlEPulsemUgR(void *p) {
      delete [] (static_cast<vector<Pulse*>*>(p));
   }
   static void destruct_vectorlEPulsemUgR(void *p) {
      typedef vector<Pulse*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<Pulse*>

namespace ROOT {
   static TClass *vectorlELong64_tgR_Dictionary();
   static void vectorlELong64_tgR_TClassManip(TClass*);
   static void *new_vectorlELong64_tgR(void *p = nullptr);
   static void *newArray_vectorlELong64_tgR(Long_t size, void *p);
   static void delete_vectorlELong64_tgR(void *p);
   static void deleteArray_vectorlELong64_tgR(void *p);
   static void destruct_vectorlELong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Long64_t>*)
   {
      vector<Long64_t> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Long64_t>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Long64_t>", -2, "vector", 389,
                  typeid(vector<Long64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlELong64_tgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Long64_t>) );
      instance.SetNew(&new_vectorlELong64_tgR);
      instance.SetNewArray(&newArray_vectorlELong64_tgR);
      instance.SetDelete(&delete_vectorlELong64_tgR);
      instance.SetDeleteArray(&deleteArray_vectorlELong64_tgR);
      instance.SetDestructor(&destruct_vectorlELong64_tgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Long64_t> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<Long64_t>","std::vector<long long, std::allocator<long long> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<Long64_t>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlELong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<Long64_t>*>(nullptr))->GetClass();
      vectorlELong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlELong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlELong64_tgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Long64_t> : new vector<Long64_t>;
   }
   static void *newArray_vectorlELong64_tgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Long64_t>[nElements] : new vector<Long64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlELong64_tgR(void *p) {
      delete (static_cast<vector<Long64_t>*>(p));
   }
   static void deleteArray_vectorlELong64_tgR(void *p) {
      delete [] (static_cast<vector<Long64_t>*>(p));
   }
   static void destruct_vectorlELong64_tgR(void *p) {
      typedef vector<Long64_t> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<Long64_t>

namespace ROOT {
   static TClass *vectorlEDataBuffergR_Dictionary();
   static void vectorlEDataBuffergR_TClassManip(TClass*);
   static void *new_vectorlEDataBuffergR(void *p = nullptr);
   static void *newArray_vectorlEDataBuffergR(Long_t size, void *p);
   static void delete_vectorlEDataBuffergR(void *p);
   static void deleteArray_vectorlEDataBuffergR(void *p);
   static void destruct_vectorlEDataBuffergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DataBuffer>*)
   {
      vector<DataBuffer> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DataBuffer>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DataBuffer>", -2, "vector", 389,
                  typeid(vector<DataBuffer>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDataBuffergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<DataBuffer>) );
      instance.SetNew(&new_vectorlEDataBuffergR);
      instance.SetNewArray(&newArray_vectorlEDataBuffergR);
      instance.SetDelete(&delete_vectorlEDataBuffergR);
      instance.SetDeleteArray(&deleteArray_vectorlEDataBuffergR);
      instance.SetDestructor(&destruct_vectorlEDataBuffergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DataBuffer> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<DataBuffer>","std::vector<DataBuffer, std::allocator<DataBuffer> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<DataBuffer>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDataBuffergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<DataBuffer>*>(nullptr))->GetClass();
      vectorlEDataBuffergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDataBuffergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDataBuffergR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<DataBuffer> : new vector<DataBuffer>;
   }
   static void *newArray_vectorlEDataBuffergR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<DataBuffer>[nElements] : new vector<DataBuffer>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDataBuffergR(void *p) {
      delete (static_cast<vector<DataBuffer>*>(p));
   }
   static void deleteArray_vectorlEDataBuffergR(void *p) {
      delete [] (static_cast<vector<DataBuffer>*>(p));
   }
   static void destruct_vectorlEDataBuffergR(void *p) {
      typedef vector<DataBuffer> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<DataBuffer>

namespace {
  void TriggerDictionaryInitialization_DecLibTestDict_Impl() {
    static const char* headers[] = {
"DecLibTest.hh",
nullptr
    };
    static const char* includePaths[] = {
"/home/terawatt/Programs/root/root-install/include/",
"/home/terawatt/Programs/declib/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "DecLibTestDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$DecLibTest.hh")))  Pulse;
class __attribute__((annotate("$clingAutoload$DecLibTest.hh")))  Event;
class __attribute__((annotate("$clingAutoload$DecLibTest.hh")))  DecFile;
class __attribute__((annotate("$clingAutoload$DecLibTest.hh")))  DecManager;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DecLibTestDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "DecLibTest.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"DecFile", payloadCode, "@",
"DecManager", payloadCode, "@",
"Event", payloadCode, "@",
"Pulse", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DecLibTestDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DecLibTestDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DecLibTestDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DecLibTestDict() {
  TriggerDictionaryInitialization_DecLibTestDict_Impl();
}
