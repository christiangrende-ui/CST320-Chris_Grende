//*******************************************************
// Purpose: Utility routines used for code generation
//*******************************************************
#include <iostream>
#include <stdio.h>
#include <string>
#include "emit.h"

static FILE* Output = nullptr;
static int Next_Label = 1;

bool InitOutput(const std::string filename)
{
    Output = fopen(filename.c_str(), "w");
    if (Output == NULL)
    {
        std::cerr << "Unable to open output file." << std::endl;
        return false;
    }
    return true;
}

bool FinalizeOutput()
{
    if (Output != nullptr) fclose(Output);
    Output = nullptr;
    return true;
}

std::string GenerateLabel()
{
    Next_Label++;
    std::string label("$LANG_LABEL");
    label += std::to_string(Next_Label);
    return label;
}

void EmitInt(int val)
{
    fprintf(Output, " %d ", val);
}

void EmitString(std::string str)
{
    fprintf(Output, "%s", str.c_str());
}
