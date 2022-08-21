#pragma once

#include <BMLPlus/BMLAll.h>

typedef const char* C_CKSTRING;

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

class BallanceOptiFine : public IMod {
	public:
	BallanceOptiFine(IBML* bml) : IMod(bml) {}

	virtual C_CKSTRING GetID() override { return "BallanceOptiFine"; }
	virtual C_CKSTRING GetVersion() override { return BML_VERSION; }
	virtual C_CKSTRING GetName() override { return "Ballance OptiFine"; }
	virtual C_CKSTRING GetAuthor() override { return "yyc12345"; }
	virtual C_CKSTRING GetDescription() override { return "Add shadow, transitional column and etc for old Ballance custom maps."; }
	DECLARE_BML_VERSION;

	private:
	virtual void OnLoad() override;
	virtual void OnLoadObject(C_CKSTRING filename, CKBOOL isMap, C_CKSTRING masterName,
		CK_CLASSID filterClass, CKBOOL addtoscene, CKBOOL reuseMeshes, CKBOOL reuseMaterials,
		CKBOOL dynamic, XObjectArray* objArray, CKObject* masterObj) override;

	IProperty* m_enable_props[4];
	IProperty* m_shadow_props[3];
	IProperty* m_material_props[5];
};