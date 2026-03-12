//*******************************************************
// Purpose: Utility routines used for code generation
//*******************************************************
#pragma once
#include <string>

bool InitOutput(const std::string filename);
bool FinalizeOutput();
std::string GenerateLabel();
void EmitInt(int val);
void EmitString(std::string str);
