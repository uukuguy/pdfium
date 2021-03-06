// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FWL_CORE_IFWL_APP_H_
#define XFA_FWL_CORE_IFWL_APP_H_

#include <memory>

#include "core/fxcrt/fx_string.h"

class CFWL_NoteDriver;
class CFWL_WidgetMgr;
class CXFA_FFApp;
class CXFA_FWLAdapterWidgetMgr;
class IFWL_Widget;

enum FWL_KeyFlag {
  FWL_KEYFLAG_Ctrl = 1 << 0,
  FWL_KEYFLAG_Alt = 1 << 1,
  FWL_KEYFLAG_Shift = 1 << 2,
  FWL_KEYFLAG_Command = 1 << 3,
  FWL_KEYFLAG_LButton = 1 << 4,
  FWL_KEYFLAG_RButton = 1 << 5,
  FWL_KEYFLAG_MButton = 1 << 6
};

class IFWL_App {
 public:
  explicit IFWL_App(CXFA_FFApp* pAdapter);
  ~IFWL_App();

  CXFA_FFApp* GetAdapterNative();
  CFWL_WidgetMgr* GetWidgetMgr();
  CFWL_NoteDriver* GetNoteDriver() const { return m_pNoteDriver.get(); }

 private:
  CXFA_FFApp* const m_pAdapterNative;
  std::unique_ptr<CFWL_WidgetMgr> m_pWidgetMgr;
  std::unique_ptr<CFWL_NoteDriver> m_pNoteDriver;
};

IFWL_App* FWL_GetApp();
void FWL_SetApp(IFWL_App* pApp);

CXFA_FFApp* FWL_GetAdapterNative();
CXFA_FWLAdapterWidgetMgr* FWL_GetAdapterWidgetMgr();

#endif  // XFA_FWL_CORE_IFWL_APP_H_
