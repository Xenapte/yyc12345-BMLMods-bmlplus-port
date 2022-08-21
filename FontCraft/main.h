#pragma once

#include <BMLPlus/BMLAll.h>
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <basetsd.h>

using namespace Gdiplus;
typedef const char* C_CKSTRING;

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

struct FontData {
	float ustart;
	float vstart;
	float uwidth;
	float uprewidth;
	float upostwidth;
	float vwidth;
};

class FontCraft : public IMod {
	public:
	FontCraft(IBML* bml) : 
		IMod(bml),
		gdiplusStartupInput(),
		ckarrayData()
	{}

	virtual C_CKSTRING GetID() override { return "FontCraft"; }
	virtual C_CKSTRING GetVersion() override { return BML_VERSION; }
	virtual C_CKSTRING GetName() override { return "Font Craft"; }
	virtual C_CKSTRING GetAuthor() override { return "yyc12345"; }
	virtual C_CKSTRING GetDescription() override { return "Change Ballance Font."; }
	DECLARE_BML_VERSION;

	private:
	virtual void OnLoad() override;
	virtual void OnUnload() override;
	virtual void OnLoadObject(C_CKSTRING filename, CKBOOL isMap, C_CKSTRING masterName,
		CK_CLASSID filterClass, CKBOOL addtoscene, CKBOOL reuseMeshes, CKBOOL reuseMaterials,
		CKBOOL dynamic, XObjectArray* objArray, CKObject* masterObj) override;

	IProperty* m_core_props[6];

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	Bitmap* bitmapFont;
	std::vector<FontData*> ckarrayData;

};

