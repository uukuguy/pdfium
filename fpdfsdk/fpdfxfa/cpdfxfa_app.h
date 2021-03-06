// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef FPDFSDK_FPDFXFA_CPDFXFA_APP_H_
#define FPDFSDK_FPDFXFA_CPDFXFA_APP_H_

#include "xfa/fxfa/fxfa.h"

class CPDFSDK_FormFillEnvironment;
class IFXJS_Runtime;

class CPDFXFA_App : public IXFA_AppProvider {
 public:
  static CPDFXFA_App* GetInstance();
  static void ReleaseInstance();

  CPDFXFA_App();
  ~CPDFXFA_App() override;

  FX_BOOL Initialize(v8::Isolate* pIsolate);
  CXFA_FFApp* GetXFAApp() { return m_pXFAApp.get(); }

  FX_BOOL AddFormFillEnv(CPDFSDK_FormFillEnvironment* pFormFillEnv);
  FX_BOOL RemoveFormFillEnv(CPDFSDK_FormFillEnvironment* pFormFillEnv);

  FX_BOOL IsJavaScriptInitialized() const { return m_bJavaScriptInitialized; }
  void SetJavaScriptInitialized(FX_BOOL bInitialized) {
    m_bJavaScriptInitialized = bInitialized;
  }

  v8::Isolate* GetJSERuntime() const { return m_pIsolate; }

  // IFXA_AppProvider:
  void GetLanguage(CFX_WideString& wsLanguage) override;
  void GetPlatform(CFX_WideString& wsPlatform) override;
  void GetAppName(CFX_WideString& wsName) override;

  void Beep(uint32_t dwType) override;
  int32_t MsgBox(const CFX_WideString& wsMessage,
                 const CFX_WideString& wsTitle,
                 uint32_t dwIconType,
                 uint32_t dwButtonType) override;
  CFX_WideString Response(const CFX_WideString& wsQuestion,
                          const CFX_WideString& wsTitle,
                          const CFX_WideString& wsDefaultAnswer,
                          FX_BOOL bMark) override;

  IFX_FileRead* DownloadURL(const CFX_WideString& wsURL) override;
  FX_BOOL PostRequestURL(const CFX_WideString& wsURL,
                         const CFX_WideString& wsData,
                         const CFX_WideString& wsContentType,
                         const CFX_WideString& wsEncode,
                         const CFX_WideString& wsHeader,
                         CFX_WideString& wsResponse) override;
  FX_BOOL PutRequestURL(const CFX_WideString& wsURL,
                        const CFX_WideString& wsData,
                        const CFX_WideString& wsEncode) override;

  void LoadString(int32_t iStringID, CFX_WideString& wsString) override;
  IFWL_AdapterTimerMgr* GetTimerMgr() override;

  CFX_ArrayTemplate<CPDFSDK_FormFillEnvironment*> m_pFormFillEnvList;

 protected:
  FX_BOOL m_bJavaScriptInitialized;
  std::unique_ptr<CXFA_FFApp> m_pXFAApp;
  v8::Isolate* m_pIsolate;
};

#endif  // FPDFSDK_FPDFXFA_CPDFXFA_APP_H_
