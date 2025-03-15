#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int print_call_result(int code, const char* call);
#define PRINT_CALL_RESULT(CALL) print_call_result(CALL, #CALL)