// Copyright (c) 2021 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=0c54d50effa8f690b879fd6ba0cdc1b3221c1cf5$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_PRINT_JOB_CALLBACK_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_PRINT_JOB_CALLBACK_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_print_handler_capi.h"
#include "include/cef_print_handler.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefPrintJobCallbackCToCpp
    : public CefCToCppRefCounted<CefPrintJobCallbackCToCpp,
                                 CefPrintJobCallback,
                                 cef_print_job_callback_t> {
 public:
  CefPrintJobCallbackCToCpp();
  virtual ~CefPrintJobCallbackCToCpp();

  // CefPrintJobCallback methods.
  void Continue() OVERRIDE;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_PRINT_JOB_CALLBACK_CTOCPP_H_