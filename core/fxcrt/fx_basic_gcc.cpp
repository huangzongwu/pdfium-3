// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include <cctype>
#include <cwctype>
#include <limits>

#include "core/fxcrt/include/fx_ext.h"
#include "core/fxcrt/include/fx_string.h"

template <typename IntType, typename CharType>
IntType FXSYS_StrToInt(const CharType* str) {
  if (!str)
    return 0;

  bool neg = std::numeric_limits<IntType>::is_signed && *str == '-';
  if (neg)
    str++;

  IntType num = 0;
  while (*str && FXSYS_isDecimalDigit(*str)) {
    IntType val = FXSYS_toDecimalDigit(*str);
    if (num > (std::numeric_limits<IntType>::max() - val) / 10)
      break;

    num = num * 10 + val;
    str++;
  }
  return neg ? -num : num;
}

template <typename T, typename UT, typename STR_T>
STR_T FXSYS_IntToStr(T value, STR_T str, int radix) {
  if (radix < 2 || radix > 16) {
    str[0] = 0;
    return str;
  }
  if (value == 0) {
    str[0] = '0';
    str[1] = 0;
    return str;
  }
  int i = 0;
  UT uvalue;
  if (value < 0) {
    str[i++] = '-';
    // Standard trick to avoid undefined behaviour when negating INT_MIN.
    uvalue = static_cast<UT>(-(value + 1)) + 1;
  } else {
    uvalue = value;
  }
  int digits = 1;
  T order = uvalue / radix;
  while (order > 0) {
    digits++;
    order = order / radix;
  }
  for (int d = digits - 1; d > -1; d--) {
    str[d + i] = "0123456789abcdef"[uvalue % radix];
    uvalue /= radix;
  }
  str[digits + i] = 0;
  return str;
}

#ifdef __cplusplus
extern "C" {
#endif
int32_t FXSYS_atoi(const FX_CHAR* str) {
  return FXSYS_StrToInt<int32_t, FX_CHAR>(str);
}
uint32_t FXSYS_atoui(const FX_CHAR* str) {
  return FXSYS_StrToInt<uint32_t>(str);
}
int32_t FXSYS_wtoi(const FX_WCHAR* str) {
  return FXSYS_StrToInt<int32_t, FX_WCHAR>(str);
}
int64_t FXSYS_atoi64(const FX_CHAR* str) {
  return FXSYS_StrToInt<int64_t, FX_CHAR>(str);
}
int64_t FXSYS_wtoi64(const FX_WCHAR* str) {
  return FXSYS_StrToInt<int64_t, FX_WCHAR>(str);
}
const FX_CHAR* FXSYS_i64toa(int64_t value, FX_CHAR* str, int radix) {
  return FXSYS_IntToStr<int64_t, uint64_t, FX_CHAR*>(value, str, radix);
}
#ifdef __cplusplus
}
#endif
#if _FXM_PLATFORM_ != _FXM_PLATFORM_WINDOWS_
#ifdef __cplusplus
extern "C" {
#endif
int FXSYS_GetACP() {
  return 0;
}
FX_DWORD FXSYS_GetFullPathName(const FX_CHAR* filename,
                               FX_DWORD buflen,
                               FX_CHAR* buf,
                               FX_CHAR** filepart) {
  int srclen = FXSYS_strlen(filename);
  if (!buf || (int)buflen < srclen + 1) {
    return srclen + 1;
  }
  FXSYS_strcpy(buf, filename);
  return srclen;
}
FX_DWORD FXSYS_GetModuleFileName(void* hModule, char* buf, FX_DWORD bufsize) {
  return (FX_DWORD)-1;
}
#ifdef __cplusplus
}
#endif
#endif
#if _FXM_PLATFORM_ != _FXM_PLATFORM_WINDOWS_
#ifdef __cplusplus
extern "C" {
#endif
FXSYS_FILE* FXSYS_wfopen(const FX_WCHAR* filename, const FX_WCHAR* mode) {
  return FXSYS_fopen(CFX_ByteString::FromUnicode(filename),
                     CFX_ByteString::FromUnicode(mode));
}
char* FXSYS_strlwr(char* str) {
  if (!str) {
    return NULL;
  }
  char* s = str;
  while (*str) {
    *str = FXSYS_tolower(*str);
    str++;
  }
  return s;
}
char* FXSYS_strupr(char* str) {
  if (!str) {
    return NULL;
  }
  char* s = str;
  while (*str) {
    *str = FXSYS_toupper(*str);
    str++;
  }
  return s;
}
FX_WCHAR* FXSYS_wcslwr(FX_WCHAR* str) {
  if (!str) {
    return NULL;
  }
  FX_WCHAR* s = str;
  while (*str) {
    *str = FXSYS_tolower(*str);
    str++;
  }
  return s;
}
FX_WCHAR* FXSYS_wcsupr(FX_WCHAR* str) {
  if (!str) {
    return NULL;
  }
  FX_WCHAR* s = str;
  while (*str) {
    *str = FXSYS_toupper(*str);
    str++;
  }
  return s;
}
int FXSYS_stricmp(const char* dst, const char* src) {
  int f, l;
  do {
    if (((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z')) {
      f -= ('A' - 'a');
    }
    if (((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z')) {
      l -= ('A' - 'a');
    }
  } while (f && (f == l));
  return (f - l);
}
int FXSYS_wcsicmp(const FX_WCHAR* dst, const FX_WCHAR* src) {
  FX_WCHAR f, l;
  do {
    if (((f = (FX_WCHAR)(*(dst++))) >= 'A') && (f <= 'Z')) {
      f -= ('A' - 'a');
    }
    if (((l = (FX_WCHAR)(*(src++))) >= 'A') && (l <= 'Z')) {
      l -= ('A' - 'a');
    }
  } while (f && (f == l));
  return (f - l);
}
char* FXSYS_itoa(int value, char* str, int radix) {
  return FXSYS_IntToStr<int32_t, uint32_t, FX_CHAR*>(value, str, radix);
}
#ifdef __cplusplus
}
#endif
#endif
#if _FXM_PLATFORM_ != _FXM_PLATFORM_WINDOWS_
#ifdef __cplusplus
extern "C" {
#endif
int FXSYS_WideCharToMultiByte(FX_DWORD codepage,
                              FX_DWORD dwFlags,
                              const FX_WCHAR* wstr,
                              int wlen,
                              FX_CHAR* buf,
                              int buflen,
                              const FX_CHAR* default_str,
                              int* pUseDefault) {
  int len = 0;
  for (int i = 0; i < wlen; i++) {
    if (wstr[i] < 0x100) {
      if (buf && len < buflen) {
        buf[len] = (FX_CHAR)wstr[i];
      }
      len++;
    }
  }
  return len;
}
int FXSYS_MultiByteToWideChar(FX_DWORD codepage,
                              FX_DWORD dwFlags,
                              const FX_CHAR* bstr,
                              int blen,
                              FX_WCHAR* buf,
                              int buflen) {
  int wlen = 0;
  for (int i = 0; i < blen; i++) {
    if (buf && wlen < buflen) {
      buf[wlen] = bstr[i];
    }
    wlen++;
  }
  return wlen;
}
#ifdef __cplusplus
}
#endif
#endif