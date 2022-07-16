#pragma once
#include "CResource.h"


class CTexture :
    public CResource
{
private:
    HDC     m_dc;       // mem_dc �� �̹����� �ø� DC
    HBITMAP m_hBit;     // �̹����� ũ�⿡ �´� ��Ʈ�� �ڵ�
    BITMAP  m_bitInfo;  // ��Ʈ���� ����


public:
    void Load(const wstring& _strFilePath);

    UINT Width() { return m_bitInfo.bmWidth; }
    UINT Height() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }


private:
    CTexture();
    virtual ~CTexture();

    friend class CResourceManager;
};