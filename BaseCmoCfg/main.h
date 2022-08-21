#pragma once

#include <BMLPlus/BMLAll.h>
#include <vector>
#include <string>

typedef const char* C_CKSTRING;

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}
class BaseCmoCfg : public IMod {
public:
	BaseCmoCfg(IBML* bml) :
		IMod(bml) {
	}

	virtual C_CKSTRING GetID() override { return "BaseCmoCfg"; }
	virtual C_CKSTRING GetVersion() override { return BML_VERSION; }
	virtual C_CKSTRING GetName() override { return "Base Cmo Config"; }
	virtual C_CKSTRING GetAuthor() override { return "yyc12345"; }
	virtual C_CKSTRING GetDescription() override { return "Change GameSettings array located in Base.cmo. Almost settings need restart game to apply!"; }
	DECLARE_BML_VERSION;

private:
	virtual void OnLoad() override;
	virtual void OnLoadObject(C_CKSTRING filename, CKBOOL isMap, C_CKSTRING masterName,
		CK_CLASSID filterClass, CKBOOL addtoscene, CKBOOL reuseMeshes, CKBOOL reuseMaterials,
		CKBOOL dynamic, XObjectArray* objArray, CKObject* masterObj) override;

	IProperty* m_core_props[2];


};

