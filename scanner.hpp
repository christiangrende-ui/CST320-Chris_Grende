#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int yylex(void);
extern FILE* yyin;

#ifdef __cplusplus
}
#endif
