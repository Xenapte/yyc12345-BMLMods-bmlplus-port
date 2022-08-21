#include "main.h"

IMod* BMLEntry(IBML* bml) {
	return new FontCraft(bml);
}

// define bitmap font const
#define HW_IMG 512
#define HW_COUNT 16
#define HW_CELL ((float)HW_IMG / (float)HW_COUNT)

void FontCraft::OnLoad() {
	// load settings
	GetConfig()->SetCategoryComment("Core", "Core settings for FontCraft");

	m_core_props[0] = GetConfig()->GetProperty("Core", "Enable");
	m_core_props[0]->SetComment("Globally enable FontCraft");
	m_core_props[0]->SetDefaultBoolean(true);

	m_core_props[1] = GetConfig()->GetProperty("Core", "FontName");
	m_core_props[1]->SetComment("Font name");
	m_core_props[1]->SetDefaultString("Arial");

	m_core_props[2] = GetConfig()->GetProperty("Core", "FontSize");
	m_core_props[2]->SetComment("Font size?");
	m_core_props[2]->SetDefaultFloat(26.0);

	m_core_props[3] = GetConfig()->GetProperty("Core", "FontItalic");
	m_core_props[3]->SetComment("Font italic?");
	m_core_props[3]->SetDefaultBoolean(false);

	m_core_props[4] = GetConfig()->GetProperty("Core", "FontBold");
	m_core_props[4]->SetComment("Font bold?");
	m_core_props[4]->SetDefaultBoolean(false);

	m_core_props[5] = GetConfig()->GetProperty("Core", "FontUnderLine");
	m_core_props[5]->SetComment("Font underline?");
	m_core_props[5]->SetDefaultBoolean(false);

	// gdi+
	if (m_core_props[0]->GetBoolean()) {
		GetLogger()->Info("Drawing bitmap font image...");
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		bitmapFont = new Bitmap(HW_IMG, HW_IMG, PixelFormat32bppARGB);
		Graphics g(bitmapFont);
		g.Clear(Gdiplus::Color(0, 255, 0, 0));
		g.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAliasGridFit);

		std::string inputChar;
		std::wstring inputWChar;
		int neededLength = 0;
		bool errorParse = false;

		inputChar = m_core_props[1]->GetString();
		errorParse = false;
		if ((neededLength = MultiByteToWideChar(CP_ACP, 0, inputChar.c_str(), -1, NULL, 0)) <= 0)
			errorParse = true;
		else {
			neededLength += 10;
			inputWChar.resize(neededLength);
			if (MultiByteToWideChar(CP_ACP, 0, inputChar.c_str(), -1, &inputWChar[0], neededLength) <= 0)
				errorParse = true;
		}
		if (errorParse)
			inputWChar = L"Arial";

		int fs = FontStyle::FontStyleRegular;
		if (m_core_props[3]->GetBoolean())
			fs = fs | FontStyle::FontStyleItalic;
		if (m_core_props[4]->GetBoolean())
			fs = fs | FontStyle::FontStyleBold;
		if (m_core_props[5]->GetBoolean())
			fs = fs | FontStyle::FontStyleUnderline;
		Gdiplus::Font ft(inputWChar.c_str(), m_core_props[2]->GetFloat(), (FontStyle)fs, Gdiplus::UnitPixel);
		Gdiplus::SolidBrush sb(Gdiplus::Color::White);
		Gdiplus::StringFormat sf(Gdiplus::StringFormat::GenericTypographic());
		sf.SetFormatFlags(sf.GetFormatFlags() | StringFormatFlags::StringFormatFlagsMeasureTrailingSpaces);

		float x = 0, y = 0;
		Gdiplus::SizeF measureRes;
		Gdiplus::SizeF emptySize(0, 0);
		Gdiplus::SizeF drawPosition(0, 0);

		inputChar.resize(2);
		inputChar[0] = '\0';
		inputChar[1] = '\0';
		FontData* dataitem = NULL;
		for (int i = 0; i < HW_COUNT; ++i) {
			for (int j = 0; j < HW_COUNT; ++j) {
				errorParse = false;
				if ((neededLength = MultiByteToWideChar(1252, 0, inputChar.c_str(), -1, NULL, 0)) <= 0)
					errorParse = true;
				else {
					neededLength += 10;
					inputWChar.resize(neededLength);
					if (MultiByteToWideChar(1252, 0, inputChar.c_str(), -1, &inputWChar[0], neededLength) <= 0)
						errorParse = true;
				}

				if (errorParse)
					inputWChar = L"\0";

				g.MeasureString(inputWChar.c_str(), 1, &ft, emptySize, &sf, &measureRes);

				drawPosition.Width = x = j * HW_CELL;
				drawPosition.Height = y = i * HW_CELL + (HW_CELL - measureRes.Height) / 2;
				dataitem = new FontData();
				dataitem->ustart = x / HW_IMG;
				dataitem->vstart = (float)i / HW_COUNT;
				dataitem->uwidth = measureRes.Width / HW_IMG;
				dataitem->uprewidth = 0;
				dataitem->upostwidth = 0;
				dataitem->vwidth = 1.0f / HW_COUNT;
				ckarrayData.push_back(dataitem);

				g.DrawString(inputWChar.c_str(), 1, &ft, drawPosition, &sf, &sb);
				++inputChar[0];
			}
		}
	}
}

void FontCraft::OnUnload() {
	// gdi+
	if (m_core_props[0]->GetBoolean()) {
		delete bitmapFont;
		for (auto iter = ckarrayData.begin(); iter != ckarrayData.end(); iter++)
			delete (*iter);
		ckarrayData.clear();
		GdiplusShutdown(this->gdiplusToken);
	}
}

void FontCraft::OnLoadObject(C_CKSTRING filename, CKBOOL isMap, C_CKSTRING masterName,
	CK_CLASSID filterClass, CKBOOL addtoscene, CKBOOL reuseMeshes, CKBOOL reuseMaterials,
	CKBOOL dynamic, XObjectArray* objArray, CKObject* masterObj) {

	// process menu.nmo
	if (strcmp(filename, "3D Entities\\Menu.nmo")) return;

	if (m_core_props[0]->GetBoolean()) {
		CKContext* ctx = m_bml->GetCKContext();

		// process image
		GetLogger()->Info("Processing Font_1...");
		CKTexture* font = (CKTexture*)ctx->GetObjectByName("Font_1");
		Gdiplus::BitmapData gdiimage;
		CKBYTE* ckimage = NULL;
		Gdiplus::Rect lockTerrtory(0, 0, HW_IMG, HW_IMG);
		DWORD* pCK = NULL;
		DWORD* pGDI = NULL;
		if ((bitmapFont->LockBits(&lockTerrtory, Gdiplus::ImageLockMode::ImageLockModeRead | Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat32bppARGB, &gdiimage) == Gdiplus::Status::Ok) &&
			((ckimage = font->LockSurfacePtr()) != NULL)) {

			for (int rows = 0; rows < HW_IMG; ++rows) {
				pCK = (DWORD*)(ckimage + 4 * HW_IMG * rows);
				pGDI = (DWORD*)((BYTE*)gdiimage.Scan0 + (gdiimage.Stride * rows));
				for (int cols = 0; cols < HW_IMG; ++cols, ++pCK, ++pGDI) {
					*pCK = *pGDI;
				}
			}

			bitmapFont->UnlockBits(&gdiimage);
			font->ReleaseSurfacePtr();
			//font->FreeVideoMemory();
		} else GetLogger()->Error("Fail to lock GDI+ image.");



		// process ck array
		GetLogger()->Info("Processing M_FontData_01...");
		CKDataArray* dataarray = (CKDataArray*)ctx->GetObjectByName("M_FontData_01");
		dataarray->Clear();
		int counter = 0;
		for (auto iter = ckarrayData.begin(); iter != ckarrayData.end(); counter++, iter++) {
			dataarray->AddRow();
			dataarray->SetElementValue(counter, 0, &(*iter)->ustart, sizeof(float));
			dataarray->SetElementValue(counter, 1, &(*iter)->vstart, sizeof(float));
			dataarray->SetElementValue(counter, 2, &(*iter)->uwidth, sizeof(float));
			dataarray->SetElementValue(counter, 3, &(*iter)->uprewidth, sizeof(float));
			dataarray->SetElementValue(counter, 4, &(*iter)->upostwidth, sizeof(float));
			dataarray->SetElementValue(counter, 5, &(*iter)->vwidth, sizeof(float));
		}
	}
}

