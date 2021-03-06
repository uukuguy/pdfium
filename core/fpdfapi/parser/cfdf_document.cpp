// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "core/fpdfapi/parser/cfdf_document.h"

#include "core/fpdfapi/edit/cpdf_creator.h"
#include "core/fpdfapi/parser/cpdf_dictionary.h"
#include "core/fpdfapi/parser/cpdf_syntax_parser.h"
#include "third_party/base/ptr_util.h"

CFDF_Document::CFDF_Document()
    : CPDF_IndirectObjectHolder(),
      m_pRootDict(nullptr),
      m_pFile(nullptr),
      m_bOwnFile(FALSE),
      m_pByteStringPool(pdfium::MakeUnique<CFX_ByteStringPool>()) {}

CFDF_Document::~CFDF_Document() {
  if (m_bOwnFile && m_pFile)
    m_pFile->Release();
  m_pByteStringPool.DeleteObject();  // Make weak.
}

CFDF_Document* CFDF_Document::CreateNewDoc() {
  CFDF_Document* pDoc = new CFDF_Document;
  pDoc->m_pRootDict = new CPDF_Dictionary(pDoc->GetByteStringPool());
  pDoc->AddIndirectObject(pDoc->m_pRootDict);
  pDoc->m_pRootDict->SetFor("FDF",
                            new CPDF_Dictionary(pDoc->GetByteStringPool()));
  return pDoc;
}

CFDF_Document* CFDF_Document::ParseFile(IFX_FileRead* pFile, FX_BOOL bOwnFile) {
  if (!pFile)
    return nullptr;

  std::unique_ptr<CFDF_Document> pDoc(new CFDF_Document);
  pDoc->ParseStream(pFile, bOwnFile);
  return pDoc->m_pRootDict ? pDoc.release() : nullptr;
}

CFDF_Document* CFDF_Document::ParseMemory(const uint8_t* pData, uint32_t size) {
  return CFDF_Document::ParseFile(FX_CreateMemoryStream((uint8_t*)pData, size),
                                  TRUE);
}

void CFDF_Document::ParseStream(IFX_FileRead* pFile, FX_BOOL bOwnFile) {
  m_pFile = pFile;
  m_bOwnFile = bOwnFile;
  CPDF_SyntaxParser parser;
  parser.InitParser(m_pFile, 0);
  while (1) {
    bool bNumber;
    CFX_ByteString word = parser.GetNextWord(&bNumber);
    if (bNumber) {
      uint32_t objnum = FXSYS_atoui(word.c_str());
      word = parser.GetNextWord(&bNumber);
      if (!bNumber)
        break;

      word = parser.GetNextWord(nullptr);
      if (word != "obj")
        break;

      CPDF_Object* pObj = parser.GetObject(this, objnum, 0, true);
      if (!pObj)
        break;

      ReplaceIndirectObjectIfHigherGeneration(objnum, pObj);
      word = parser.GetNextWord(nullptr);
      if (word != "endobj")
        break;
    } else {
      if (word != "trailer")
        break;

      if (CPDF_Dictionary* pMainDict =
              ToDictionary(parser.GetObject(this, 0, 0, true))) {
        m_pRootDict = pMainDict->GetDictFor("Root");
        pMainDict->Release();
      }
      break;
    }
  }
}

FX_BOOL CFDF_Document::WriteBuf(CFX_ByteTextBuf& buf) const {
  if (!m_pRootDict)
    return FALSE;

  buf << "%FDF-1.2\r\n";
  for (const auto& pair : *this)
    buf << pair.first << " 0 obj\r\n"
        << pair.second.get() << "\r\nendobj\r\n\r\n";

  buf << "trailer\r\n<</Root " << m_pRootDict->GetObjNum()
      << " 0 R>>\r\n%%EOF\r\n";
  return TRUE;
}
