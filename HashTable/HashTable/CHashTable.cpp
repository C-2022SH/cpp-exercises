#include "CHashTable.h"


HashTable::HashTable() :
    pHashTable(nullptr)
{
}

HashTable::~HashTable()
{
    delete_hashTable(pHashTable);
}


// 해시테이블 생성
PHASHTABLE HashTable::create_hashTable()
{
    pHashTable = new HASHTABLE;

    return pHashTable;
}

// 해시테이블 삭제
void HashTable::delete_hashTable(PHASHTABLE _pHash)
{
    // 배열의 모든 아이템을 삭제한다.
    for (int i = 0; i < _pHash->iTableSize; i++)
    {
        PITEM pTargetItem = _pHash->arrTableItems[i];

        // 연결 리스트의 아이템까지 전부 삭제한다.
        while (pTargetItem)
        {
            PITEM pNextItem = pTargetItem->pLinkedNode;

            delete pTargetItem;

            pTargetItem = pNextItem;
        }
    }

    delete _pHash;

    // 전역 변수 초기화
    _pHash = nullptr;
}

void HashTable::insert_hashItem(PHASHTABLE _pHash, char* _cKey, char* _cValue)
{
    int iCreatedKey = getStringIndex(_cKey);
    PITEM pCurrentItem = _pHash->arrTableItems[iCreatedKey];
    PITEM pPrevItem = nullptr;

    // 충돌이 아닌 경우
    if (!pCurrentItem)
    {
        // 아이템 추가
        _pHash->arrTableItems[iCreatedKey] = new ITEM;
        _pHash->iItemCount++;

        strcpy_s(_pHash->arrTableItems[iCreatedKey]->cKey, _cKey);
        strcpy_s(_pHash->arrTableItems[iCreatedKey]->cValue, _cValue);
        _pHash->arrTableItems[iCreatedKey]->pLinkedNode = nullptr;
    }

    // 충돌인 경우
    else
    {
        // 정말로 충돌인지 확인
        while (pCurrentItem)
        {
            // 키 값이 완전히 동일한 경우: 충돌 아님!
            if (strcmp(pCurrentItem->cKey, _cKey) == 0)
            {
                // 값 업데이트 후 종료
                strcpy_s(pCurrentItem->cValue, _cValue);
                return;
            }

            // 키 값이 동일하지 않은 경우: 충돌! 링크드리스트를 순회하며 탐색
            else
            {
                pPrevItem = pCurrentItem;
                pCurrentItem = pCurrentItem->pLinkedNode;
            }
        }

        // 마지막 노드의 뒤에 아이템 추가
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
                // 삭제할 아이템을 찾을 수 없음
                return 1;
        }

        // 내가 연결된(충돌된) 아이템일 경우 - 리스트 연결
        if (pPrevItem)
            pPrevItem->pLinkedNode = pCurrentItem->pLinkedNode;

        // 유일한 아이템일 경우 - 뒤의 아이템을 끌어온다
        else
            _pHash->arrTableItems[iCreatedKey] = pCurrentItem->pLinkedNode;

        // 아이템 삭제
        delete pCurrentItem;

        return 0;
    }
    
    return 1;
}