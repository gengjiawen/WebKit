/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSWorkletGlobalScope.h"

#include "ActiveDOMObject.h"
#include "ExtendedDOMClientIsoSubspaces.h"
#include "ExtendedDOMIsoSubspaces.h"
#include "JSDOMAttribute.h"
#include "JSDOMBinding.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMGlobalObjectInlines.h"
#include "JSDOMWrapperCache.h"
#include "JSWorkletGlobalScope.h"
#include "ScriptExecutionContext.h"
#include "WebCoreJSClientData.h"
#include "WorkletGlobalScope.h"
#include <JavaScriptCore/HeapAnalyzer.h>
#include <JavaScriptCore/JSCInlines.h>
#include <JavaScriptCore/JSDestructibleObjectHeapCellType.h>
#include <JavaScriptCore/SlotVisitorMacros.h>
#include <JavaScriptCore/SubspaceInlines.h>
#include <wtf/GetPtr.h>
#include <wtf/PointerPreparations.h>
#include <wtf/URL.h>


namespace WebCore {
using namespace JSC;

// Attributes

static JSC_DECLARE_CUSTOM_GETTER(jsWorkletGlobalScopeConstructor);
static JSC_DECLARE_CUSTOM_GETTER(jsWorkletGlobalScope_WorkletGlobalScopeConstructor);

using JSWorkletGlobalScopeDOMConstructor = JSDOMConstructorNotConstructable<JSWorkletGlobalScope>;

/* Hash table */

static const struct CompactHashIndex JSWorkletGlobalScopeTableIndex[2] = {
    { 0, -1 },
    { -1, -1 },
};


static const HashTableValue JSWorkletGlobalScopeTableValues[] =
{
    { "WorkletGlobalScope"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum), NoIntrinsic, { HashTableValue::GetterSetterType, jsWorkletGlobalScope_WorkletGlobalScopeConstructor, 0 } },
};

static const HashTable JSWorkletGlobalScopeTable = { 1, 1, static_cast<uint8_t>(static_cast<unsigned>(JSC::PropertyAttribute::DontEnum)), JSWorkletGlobalScope::info(), JSWorkletGlobalScopeTableValues, JSWorkletGlobalScopeTableIndex };
template<> const ClassInfo JSWorkletGlobalScopeDOMConstructor::s_info = { "WorkletGlobalScope"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSWorkletGlobalScopeDOMConstructor) };

template<> JSValue JSWorkletGlobalScopeDOMConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    return JSEventTarget::getConstructor(vm, &globalObject);
}

template<> void JSWorkletGlobalScopeDOMConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->length, jsNumber(0), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    JSString* nameString = jsNontrivialString(vm, "WorkletGlobalScope"_s);
    m_originalName.set(vm, this, nameString);
    putDirect(vm, vm.propertyNames->name, nameString, JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->prototype, JSWorkletGlobalScope::prototype(vm, globalObject), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::DontDelete);
}

/* Hash table for prototype */

static const struct CompactHashIndex JSWorkletGlobalScopePrototypeTableIndex[2] = {
    { -1, -1 },
    { 0, -1 },
};


static const HashTableValue JSWorkletGlobalScopePrototypeTableValues[] =
{
    { "constructor"_s, static_cast<unsigned>(PropertyAttribute::DontEnum), NoIntrinsic, { HashTableValue::GetterSetterType, jsWorkletGlobalScopeConstructor, 0 } },
};

static const HashTable JSWorkletGlobalScopePrototypeTable = { 1, 1, static_cast<uint8_t>(static_cast<unsigned>(PropertyAttribute::DontEnum)), JSWorkletGlobalScope::info(), JSWorkletGlobalScopePrototypeTableValues, JSWorkletGlobalScopePrototypeTableIndex };
const ClassInfo JSWorkletGlobalScopePrototype::s_info = { "WorkletGlobalScope"_s, &Base::s_info, &JSWorkletGlobalScopePrototypeTable, nullptr, CREATE_METHOD_TABLE(JSWorkletGlobalScopePrototype) };

void JSWorkletGlobalScopePrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSWorkletGlobalScope::info(), JSWorkletGlobalScopePrototypeTableValues, *this);
    JSC_TO_STRING_TAG_WITHOUT_TRANSITION();
}

const ClassInfo JSWorkletGlobalScope::s_info = { "WorkletGlobalScope"_s, &Base::s_info, &JSWorkletGlobalScopeTable, nullptr, CREATE_METHOD_TABLE(JSWorkletGlobalScope) };

JSWorkletGlobalScope::JSWorkletGlobalScope(VM& vm, Structure* structure, Ref<WorkletGlobalScope>&& impl)
    : JSEventTarget(vm, structure, WTFMove(impl))
{
}

void JSWorkletGlobalScope::finishCreation(VM& vm, JSProxy* proxy)
{
    Base::finishCreation(vm, proxy);

    static_assert(!std::is_base_of<ActiveDOMObject, WorkletGlobalScope>::value, "Interface is not marked as [ActiveDOMObject] even though implementation class subclasses ActiveDOMObject.");

}

JSObject* JSWorkletGlobalScope::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return JSWorkletGlobalScopePrototype::create(vm, &globalObject, JSWorkletGlobalScopePrototype::createStructure(vm, &globalObject, JSEventTarget::prototype(vm, globalObject)));
}

JSObject* JSWorkletGlobalScope::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSWorkletGlobalScope>(vm, globalObject);
}

JSValue JSWorkletGlobalScope::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSWorkletGlobalScopeDOMConstructor, DOMConstructorID::WorkletGlobalScope>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

JSC_DEFINE_CUSTOM_GETTER(jsWorkletGlobalScopeConstructor, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName))
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSWorkletGlobalScopePrototype*>(JSValue::decode(thisValue));
    if (UNLIKELY(!prototype))
        return throwVMTypeError(lexicalGlobalObject, throwScope);
    return JSValue::encode(JSWorkletGlobalScope::getConstructor(JSC::getVM(lexicalGlobalObject), prototype->globalObject()));
}

static inline JSValue jsWorkletGlobalScope_WorkletGlobalScopeConstructorGetter(JSGlobalObject& lexicalGlobalObject, JSWorkletGlobalScope& thisObject)
{
    UNUSED_PARAM(lexicalGlobalObject);
    return JSWorkletGlobalScope::getConstructor(JSC::getVM(&lexicalGlobalObject), &thisObject);
}

JSC_DEFINE_CUSTOM_GETTER(jsWorkletGlobalScope_WorkletGlobalScopeConstructor, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName attributeName))
{
    return IDLAttribute<JSWorkletGlobalScope>::get<jsWorkletGlobalScope_WorkletGlobalScopeConstructorGetter>(*lexicalGlobalObject, thisValue, attributeName);
}

JSC::GCClient::IsoSubspace* JSWorkletGlobalScope::subspaceForImpl(JSC::VM& vm)
{
    return WebCore::subspaceForImpl<JSWorkletGlobalScope, UseCustomHeapCellType::Yes>(vm,
        [] (auto& spaces) { return spaces.m_clientSubspaceForWorkletGlobalScope.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_clientSubspaceForWorkletGlobalScope = std::forward<decltype(space)>(space); },
        [] (auto& spaces) { return spaces.m_subspaceForWorkletGlobalScope.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_subspaceForWorkletGlobalScope = std::forward<decltype(space)>(space); },
        [] (auto& server) -> JSC::HeapCellType& { return server.m_heapCellTypeForJSWorkletGlobalScope; }
    );
}

void JSWorkletGlobalScope::analyzeHeap(JSCell* cell, HeapAnalyzer& analyzer)
{
    auto* thisObject = jsCast<JSWorkletGlobalScope*>(cell);
    analyzer.setWrappedObjectForCell(cell, &thisObject->wrapped());
    if (thisObject->scriptExecutionContext())
        analyzer.setLabelForCell(cell, "url "_s + thisObject->scriptExecutionContext()->url().string());
    Base::analyzeHeap(cell, analyzer);
}

WorkletGlobalScope* JSWorkletGlobalScope::toWrapped(JSC::VM&, JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSWorkletGlobalScope*>(value))
        return &wrapper->wrapped();
    return nullptr;
}

}
