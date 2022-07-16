// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include "CHashTable.h"

using namespace std;


int main()
{
    HashTable* cHash = new HashTable;

    PHASHTABLE pHash = cHash->create_hashTable();

    while (true)
    {
        cout << "\n\n========== choose menu ==========" << endl;

        cout << "1. insert" << endl;
        cout << "2. search" << endl;
        cout << "3. delete" << endl;
        cout << "4. quit\n" << endl;

        cin.clear();

        int iMenu;
        cin >> iMenu;

        if (iMenu == (int)MENU::END)
        {
            cHash->delete_hashTable(pHash);
            break;
        }

        switch (iMenu)
        {
        case (int)MENU::INSERT:
        {
            char cKey[CHAR_MAX];
            char cValue[CHAR_MAX];

            cin.clear();
            cin.ignore(1024, '\n');     // 엔터가 들어가 있으면 getline 이 무시되므로

            cout << "\ninput key : " << endl;
            cin.getline(cKey, CHAR_MAX);

            cout << "\ninput value : " << endl;
            cin.getline(cValue, CHAR_MAX);

            cHash->insert_hashItem(pHash, cKey, cValue);

            cout << "\ninsert finished > <" << endl;
        }
        break;

        case (int)MENU::SEARCH:
        {
            char cKey[CHAR_MAX];

            cout << "\ninput key : " << endl;
            cin >> cKey;

            PITEM pItem = cHash->search_hashItem(pHash, cKey);

            if (pItem)
            {
                cout << "\nvalue : \n" << pItem->cValue << endl;
            }
            else
                cout << "\nno matching item for the key T T" << endl;
        }
        break;

        case (int)MENU::DELETE:
        {
            char cKey[CHAR_MAX];

            cout << "\ninput key : " << endl;
            cin >> cKey;

            if (cHash->delete_hashItem(pHash, cKey) == 0)
                cout << "\nsuccessfully deleted > <" << endl;
            else
                cout << "\nno matching item to delete T T" << endl;
        }
            break;

        default:
            cin.clear();
            cin.ignore(1024, '\n');     // 엔터가 들어가 있으면 getline 이 무시되므로

            cout << "input error!" << endl;
            continue;
            break;
        }
    }

    _CrtDumpMemoryLeaks();

    return 0;
}