#pragma once

#include <BMLPlus/BMLAll.h>

typedef const char* C_CKSTRING;

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}
class Coredump : public IMod {
public:
	Coredump(IBML* bml) :
		IMod(bml) {
	}

	virtual C_CKSTRING GetID() override { return "Coredump"; }
	virtual C_CKSTRING GetVersion() override { return BML_VERSION; }
	virtual C_CKSTRING GetName() override { return "Runtime Virtools Context Dumper"; }
	virtual C_CKSTRING GetAuthor() override { return "yyc12345"; }
	virtual C_CKSTRING GetDescription() override { return "Dump runtime Virtools context into Virtools file."; }
	DECLARE_BML_VERSION;

private:
	virtual void OnLoad() override;
};

class CoredumpCommand : public ICommand {
public:
	virtual std::string GetName() override { return "coredump"; };
	virtual std::string GetAlias() override { return "breakpoint"; };
	virtual std::string GetDescription() override { return "Dump current Virtools context into Virtools file."; };
	virtual bool IsCheat() override { return false; };

	virtual void Execute(IBML* bml, const std::vector<std::string>& args) override;
	virtual const std::vector<std::string> GetTabCompletion(IBML* bml, const std::vector<std::string>& args) override;

};
