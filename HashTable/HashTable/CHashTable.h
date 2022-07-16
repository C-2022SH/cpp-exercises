#pragma once

#include <string>


#define MOD         128
#define MULTIPLIER  121
#define SPLITTER    127
#define TABLESIZE   128


enum class MENU
{
    START,

    INSERT,
    SEARCH,
    DELETE,

    END
};


typedef struct _hashItems
{
    char        cKey[CHAR_MAX];
    char        cValue[CHAR_MAX];
    _hashItems* pLinkedNode;

} ITEM, * PITEM;


typedef struct _hashTable
{
    PITEM   arrTableItems[TABLESIZE] = {};
    int     iTableSize = TABLESIZE;
    int     iItemCount = 0;

} HASHTABLE, * PHASHTABLE;



class HashTable
{
private:
    PHASHTABLE pHashTable;

private:
    int getStringIndex(char* _cKey)
    {
        int iStringInt = 0;

        for (int i = 0; _cKey[i]; i++)
            iStringInt += _cKey[i];

        int iIndex = (MULTIPLIER * iStringInt + SPLITTER) % MOD;
        return iIndex;
    }

public:
    PHASHTABLE create_hashTable();
    void delete_hashTable(PHASHTABLE _pHash);

    void insert_hashItem(PHASHTABLE _pHash, char* _cKey, char* _cValue);
    PITEM search_hashItem(PHASHTABLE _pHash, char* _cKey);
    int delete_hashItem(PHASHTABLE _pHash, char* _cKey);


public:
    HashTable();
    ~HashTable();
};