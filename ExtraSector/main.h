#pragma once

#include <BMLPlus/BMLAll.h>

typedef const char* C_CKSTRING;

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}
class ExtraSector : public IMod {
public:
	ExtraSector(IBML* bml) :
		IMod(bml),
		detected_level(8) {
	}

	virtual C_CKSTRING GetID() override { return "ExtraSector"; }
	virtual C_CKSTRING GetVersion() override { return BML_VERSION; }
	virtual C_CKSTRING GetName() override { return "Extra Sector"; }
	virtual C_CKSTRING GetAuthor() override { return "2jjy, Gamepiaynmo, yyc12345"; }
	virtual C_CKSTRING GetDescription() override { return "Yet Another 999 Sector Loader."; }
	DECLARE_BML_VERSION;

private:
	int detected_level;

	virtual void OnLoadObject(C_CKSTRING filename, CKBOOL isMap, C_CKSTRING masterName,
		CK_CLASSID filterClass, CKBOOL addtoscene, CKBOOL reuseMeshes, CKBOOL reuseMaterials,
		CKBOOL dynamic, XObjectArray* objArray, CKObject* masterObj) override;
	virtual void OnPostLoadLevel() override;
};

