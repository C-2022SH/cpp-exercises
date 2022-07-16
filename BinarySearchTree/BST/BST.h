#pragma once

#include <cassert>


enum class NODE_TYPE
{
	PARENT,
	LCHILD,
	RCHILD,

	END
};


// template 에서는 typedef 만들 수 없다! ㅜㅡㅜ

// 아무튼 이건 데이터 페어 (데이터 그 자체)
template <typename T1, typename T2>
struct tDataPair
{
	T1 first;
	T2 second;
};

template<typename T1, typename T2>
tDataPair<T1, T2> make_bstpair(const T1& _first, const T2& _second)
{
	return tDataPair<T1, T2>{ _first, _second };
}


// 데이터 페어를 담은 노드
template <typename T1, typename T2>
struct tBSTNode
{
	// data, 부모노드, 자식노드
	tDataPair<T1, T2>	dataPair;

	tBSTNode* arrNode[(int)NODE_TYPE::END];

	bool isRoot()
	{
		if (arrNode[(int)NODE_TYPE::PARENT] == nullptr)
			return true;

		return false;
	}

	bool isLeftChild()
	{
		if (this == arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::LCHILD])
			return true;

		return false;
	}

	bool isRightChild()
	{
		if (this == arrNode[(int)NODE_TYPE::PARENT]->arrNode[(int)NODE_TYPE::RCHILD])
			return true;

		return false;
	}


	// 생성자
	tBSTNode() :
		dataPair(), arrNode{}
	{}

	tBSTNode(const tDataPair<T1, T2>& _tPair) :
		dataPair(_tPair), arrNode{}
	{}

	tBSTNode(const tDataPair<T1, T2>& _tPair, tBSTNode* _pParent, tBSTNode* _pLChild, tBSTNode* _pRChild) :
		dataPair(_tPair),
		arrNode{ _pParent, _pLChild, _pRChild }
	{}
};


template <typename T1, typename T2>
class CBST
{
private:
	tBSTNode<T1, T2>*	m_pRoot;		// 루트 노드만 알면 다 알 수 있다!
	int					m_iCount;


public:
	bool insert(const tDataPair<T1, T2>& _tPair);
	tBSTNode<T1, T2>* GetInorderSuccessor(tBSTNode<T1, T2>* _pNode);		// 중위후속자
	tBSTNode<T1, T2>* GetInorderPredecessor(tBSTNode<T1, T2>* _pNode);		// 중위선행자

public:
	class iterator;

	// 기준: 중위순회
	iterator begin();
	iterator end();
	iterator find(const T1& _find);


public:
	CBST() :
		m_pRoot(nullptr),
		m_iCount(0)
	{}

	~CBST()
	{}


	// =============
	//   iterator
	// =============
	//
	class iterator
	{
	private:
		CBST<T1, T2>*		m_pBST;		// 부모 노드
		tBSTNode<T1, T2>*	m_pNode;	// 현재 가리키는 노드
										// nullptr 인 경우 end()

	public:
		bool operator ==(const iterator& _other)
		{
			if (m_pBST == _other.m_pBST && m_pNode == _other.m_pNode)
				return true;

			return false;
		}

		bool operator !=(const iterator& _other)
		{
			return !(*this == _other);
		}

		// 값을 수정할 수 없도록 한다
		// 구조체 자체를 리턴
		const tDataPair<T1, T2>& operator *()
		{
			assert(m_pNode);

			return m_pNode->dataPair;
		}

		// 구조체의 주소를 리턴
		const tDataPair<T1, T2>* operator ->()
		{
			assert(m_pNode);

			return &m_pNode->dataPair;
		}

		// 중위순회 기준!!
		iterator& operator ++()
		{
			m_pNode = m_pBST->GetInorderSuccessor(m_pNode);
			return *this;
		}

	public:
		iterator() :
			m_pBST(nullptr),
			m_pNode(nullptr)
		{}

		iterator(CBST<T1, T2>* _pBST, tBSTNode<T1, T2>* _pNode) :
			m_pBST(_pBST),
			m_pNode(_pNode)
		{}
	};
};


// =============
//   구현 부분
// =============
//
template<typename T1, typename T2>
inline bool CBST<T1, T2>::insert(const tDataPair<T1, T2>& _tPair)
{
	tBSTNode<T1, T2>* pNewNode = new tBSTNode<T1, T2>(_tPair);

	// 지금 들어온 데이터가 첫 번째
	if (m_pRoot == nullptr)
	{
		m_pRoot = pNewNode;
	}

	// 이미 루트 노드가 존재
	else
	{
		tBSTNode<T1, T2>* pNode = m_pRoot;
		NODE_TYPE node_type = NODE_TYPE::END;

		while (true)
		{
			// node_type 잡아주기
			if (pNode->dataPair.first < pNewNode->dataPair.first)
				node_type = NODE_TYPE::RCHILD;
				
			else if (pNode->dataPair.first > pNewNode->dataPair.first)
				node_type = NODE_TYPE::LCHILD;

			// 값이 같은 경우 - 중복 키를 허용하지 않는다.
			else
				return false;


			// 처리
			if (pNode->arrNode[(int)node_type])
				// 가야할 자리에 없으면 밑으로 내려간다
				pNode = pNode->arrNode[(int)node_type];

			else
			{
				// 가야할 자리가 비었으면 거기에 들어간다
				pNode->arrNode[(int)node_type] = pNewNode;
				pNewNode->arrNode[(int)NODE_TYPE::PARENT] = pNode;

				break;
			}
		}
	}

	++m_iCount;
	return true;
}

template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInorderSuccessor(tBSTNode<T1, T2>* _pNode)
{
	tBSTNode<T1, T2>* pSuccessor = nullptr;

	// 오른쪽 자식이 있는 경우, 오른쪽 자식으로 가서 왼쪽 자식이 없을 때까지 왼쪽으로 내려간다 -> 직전의 왼쪽 자식이 후속자
	if (_pNode->arrNode[(int)NODE_TYPE::RCHILD])
	{
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::RCHILD];

		while (pSuccessor->arrNode[(int)NODE_TYPE::LCHILD])
		{
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::LCHILD];
		}
	}

	// 오른쪽 자식이 없는 경우, 자신이 부모의 왼쪽 자식일 때까지 올라간다 -> 부모가 후속자
	else
	{
		pSuccessor = _pNode;

		while (true)
		{
			// 루트 노드 체크
			if (pSuccessor->isRoot())
				break;


			// 부모의 왼쪽 자식인지 체크
			if (pSuccessor->isLeftChild())
			{
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
				break;
			}
			else
			{
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
			}
		}
	}

	return pSuccessor;
}

template<typename T1, typename T2>
inline tBSTNode<T1, T2>* CBST<T1, T2>::GetInorderPredecessor(tBSTNode<T1, T2>* _pNode)
{
	// 위에랑 비슷하게 가보자고~~
	tBSTNode<T1, T2>* pSuccessor = nullptr;

	// 왼쪽 자식이 있는 경우, 왼쪽 자식으로 가서 오른쪽 자식이 없을 때까지 오른쪽으로 내려간다 -> 직전의 오른쪽 자식이 선행자
	if (_pNode->arrNode[(int)NODE_TYPE::LCHILD])
	{
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::LCHILD];

		while (pSuccessor->arrNode[(int)NODE_TYPE::RCHILD])
		{
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::RCHILD];
		}
	}

	// 왼쪽 자식이 있는 경우, 자신이 부모의 오른쪽 자식일 때까지 올라간다 -> 부모가 선행자
	else
	{
		pSuccessor = _pNode;

		while (true)
		{
			// 루트 노드 체크
			if (pSuccessor->isRoot())
				break;


			// 부모의 오른쪽 자식인지 체크
			if (pSuccessor->isRightChild())
			{
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
				break;
			}
			else
			{
				pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::PARENT];
			}
		}
	}

	return pSuccessor;
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::begin()
{
	tBSTNode<T1, T2>* pNode = m_pRoot;

	// 가장 왼쪽 노드를 찾는다 == 왼쪽 노드가 nullptr 일 때까지 찾는다
	while (pNode->arrNode[(int)NODE_TYPE::LCHILD])
	{
		pNode = pNode->arrNode[(int)NODE_TYPE::LCHILD];
	}

	return iterator(this, pNode);
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::end()
{
	tBSTNode<T1, T2>* pNode = m_pRoot;

	// 가장 오른쪽 노드를 찾는다 == 오른쪽 노드가 nullptr 일 때까지 찾는다
	while (pNode->arrNode[(int)NODE_TYPE::RCHILD])
	{
		pNode = pNode->arrNode[(int)NODE_TYPE::RCHILD];
	}

	return iterator(this, pNode);
}

template<typename T1, typename T2>
inline typename CBST<T1, T2>::iterator CBST<T1, T2>::find(const T1& _find)
{
	tBSTNode<T1, T2>* pNode = m_pRoot;
	NODE_TYPE node_type = NODE_TYPE::END;

	while (true)
	{
		// node_type 잡아주기
		if (pNode->dataPair.first < _find)
			node_type = NODE_TYPE::RCHILD;

		else if (pNode->dataPair.first > _find)
			node_type = NODE_TYPE::LCHILD;

		else
			// 값이 같은 경우 - 찾음!!!
			break;


		// 처리
		if (pNode->arrNode[(int)node_type])
			// 탐색 계속
			pNode = pNode->arrNode[(int)node_type];

		else
		{
			// 못찾음!!
			pNode = nullptr;
			break;
		}
	}

	return iterator(this, pNode);
}
