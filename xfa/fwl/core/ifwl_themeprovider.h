// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FWL_CORE_IFWL_THEMEPROVIDER_H_
#define XFA_FWL_CORE_IFWL_THEMEPROVIDER_H_

#include "core/fxcrt/fx_coordinates.h"
#include "core/fxcrt/fx_system.h"
#include "xfa/fwl/core/fwl_error.h"
#include "xfa/fwl/theme/cfwl_widgettp.h"

class CFWL_ThemeBackground;
class CFWL_ThemePart;
class CFWL_ThemeText;
class IFWL_Widget;

class IFWL_ThemeProvider {
 public:
  virtual ~IFWL_ThemeProvider() {}
  virtual FX_BOOL DrawBackground(CFWL_ThemeBackground* pParams) = 0;
  virtual FX_BOOL DrawText(CFWL_ThemeText* pParams) = 0;
  virtual void* GetCapacity(CFWL_ThemePart* pThemePart,
                            CFWL_WidgetCapacity dwCapacity) = 0;
  virtual FX_BOOL IsCustomizedLayout(IFWL_Widget* pWidget) = 0;
  virtual FX_BOOL CalcTextRect(CFWL_ThemeText* pParams, CFX_RectF& rect) = 0;
};

#endif  // XFA_FWL_CORE_IFWL_THEMEPROVIDER_H_
