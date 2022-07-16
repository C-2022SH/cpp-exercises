#include "CHashTable.h"


HashTable::HashTable() :
    pHashTable(nullptr)
{
}

HashTable::~HashTable()
{
    delete_hashTable(pHashTable);
}


// �ؽ����̺� ����
PHASHTABLE HashTable::create_hashTable()
{
    pHashTable = new HASHTABLE;

    return pHashTable;
}

// �ؽ����̺� ����
void HashTable::delete_hashTable(PHASHTABLE _pHash)
{
    // �迭�� ��� �������� �����Ѵ�.
    for (int i = 0; i < _pHash->iTableSize; i++)
    {
        PITEM pTargetItem = _pHash->arrTableItems[i];

        // ���� ����Ʈ�� �����۱��� ���� �����Ѵ�.
        while (pTargetItem)
        {
            PITEM pNextItem = pTargetItem->pLinkedNode;

            delete pTargetItem;

            pTargetItem = pNextItem;
        }
    }

    delete _pHash;

    // ���� ���� �ʱ�ȭ
    _pHash = nullptr;
}

void HashTable::insert_hashItem(PHASHTABLE _pHash, char* _cKey, char* _cValue)
{
    int iCreatedKey = getStringIndex(_cKey);
    PITEM pCurrentItem = _pHash->arrTableItems[iCreatedKey];
    PITEM pPrevItem = nullptr;

    // �浹�� �ƴ� ���
    if (!pCurrentItem)
    {
        // ������ �߰�
        _pHash->arrTableItems[iCreatedKey] = new ITEM;
        _pHash->iItemCount++;

        strcpy_s(_pHash->arrTableItems[iCreatedKey]->cKey, _cKey);
        strcpy_s(_pHash->arrTableItems[iCreatedKey]->cValue, _cValue);
        _pHash->arrTableItems[iCreatedKey]->pLinkedNode = nullptr;
    }

    // �浹�� ���
    else
    {
        // ������ �浹���� Ȯ��
        while (pCurrentItem)
        {
            // Ű ���� ������ ������ ���: �浹 �ƴ�!
            if (strcmp(pCurrentItem->cKey, _cKey) == 0)
            {
                // �� ������Ʈ �� ����
                strcpy_s(pCurrentItem->cValue, _cValue);
                return;
            }

            // Ű ���� �������� ���� ���: �浹! ��ũ�帮��Ʈ�� ��ȸ�ϸ� Ž��
            else
            {
                pPrevItem = pCurrentItem;
                pCurrentItem = pCurrentItem->pLinkedNode;
            }
        }

        // ������ ����� �ڿ� ������ �߰�
        pPrevItem->pLinkedNode = new ITEM;
        _pHash->iItemCount++;

        strcpy_s(pPrevItem->pLinkedNode->cKey, _cKey);
        strcpy_s(pPrevItem->pLinkedNode->cValue, _cValue);

        pPrevItem->pLinkedNode->pLinkedNode = nullptr;

    }
}

PITEM HashTable::search_hashItem(PHASHTABLE _pHash, char* _cKey)
{
    int iCreatedKey = getStringIndex(_cKey);

    PITEM pCurrentItem = _pHash->arrTableItems[iCreatedKey];

    if (pCurrentItem)
    {
        while (strcmp(pCurrentItem->cKey, _cKey) != 0)
        {
            if (pCurrentItem->pLinkedNode)
                pCurrentItem = pCurrentItem->pLinkedNode;

            else
                return nullptr;
        }
    }

    return pCurrentItem;
}

int HashTable::delete_hashItem(PHASHTABLE _pHash, char* _cKey)
{
    int iCreatedKey = getStringIndex(_cKey);

    PITEM pPrevItem = nullptr;
    PITEM pCurrentItem = _pHash->arrTableItems[iCreatedKey];

    if (pCurrentItem)
    {
        while (strcmp(pCurrentItem->cKey, _cKey) != 0)
        {
            if (pCurrentItem->pLinkedNode)
            {
                pPrevItem = pCurrentItem;
                pCurrentItem = pCurrentItem->pLinkedNode;
            }

            else
                // ������ �������� ã�� �� ����
                return 1;
        }

        // ���� �����(�浹��) �������� ��� - ����Ʈ ����
        if (pPrevItem)
            pPrevItem->pLinkedNode = pCurrentItem->pLinkedNode;

        // ������ �������� ��� - ���� �������� ����´�
        else
            _pHash->arrTableItems[iCreatedKey] = pCurrentItem->pLinkedNode;

        // ������ ����
        delete pCurrentItem;

        return 0;
    }
    
    return 1;
}