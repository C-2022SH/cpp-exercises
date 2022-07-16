#pragma once

#include <cassert>


enum class NODE_TYPE
{
	PARENT,
	LCHILD,
	RCHILD,

	END
};


// template ������ typedef ���� �� ����! �̤Ѥ�

// �ƹ�ư �̰� ������ ��� (������ �� ��ü)
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


// ������ �� ���� ���
template <typename T1, typename T2>
struct tBSTNode
{
	// data, �θ���, �ڽĳ��
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


	// ������
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
	tBSTNode<T1, T2>*	m_pRoot;		// ��Ʈ ��常 �˸� �� �� �� �ִ�!
	int					m_iCount;


public:
	bool insert(const tDataPair<T1, T2>& _tPair);
	tBSTNode<T1, T2>* GetInorderSuccessor(tBSTNode<T1, T2>* _pNode);		// �����ļ���
	tBSTNode<T1, T2>* GetInorderPredecessor(tBSTNode<T1, T2>* _pNode);		// ����������

public:
	class iterator;

	// ����: ������ȸ
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
		CBST<T1, T2>*		m_pBST;		// �θ� ���
		tBSTNode<T1, T2>*	m_pNode;	// ���� ����Ű�� ���
										// nullptr �� ��� end()

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

		// ���� ������ �� ������ �Ѵ�
		// ����ü ��ü�� ����
		const tDataPair<T1, T2>& operator *()
		{
			assert(m_pNode);

			return m_pNode->dataPair;
		}

		// ����ü�� �ּҸ� ����
		const tDataPair<T1, T2>* operator ->()
		{
			assert(m_pNode);

			return &m_pNode->dataPair;
		}

		// ������ȸ ����!!
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
//   ���� �κ�
// =============
//
template<typename T1, typename T2>
inline bool CBST<T1, T2>::insert(const tDataPair<T1, T2>& _tPair)
{
	tBSTNode<T1, T2>* pNewNode = new tBSTNode<T1, T2>(_tPair);

	// ���� ���� �����Ͱ� ù ��°
	if (m_pRoot == nullptr)
	{
		m_pRoot = pNewNode;
	}

	// �̹� ��Ʈ ��尡 ����
	else
	{
		tBSTNode<T1, T2>* pNode = m_pRoot;
		NODE_TYPE node_type = NODE_TYPE::END;

		while (true)
		{
			// node_type ����ֱ�
			if (pNode->dataPair.first < pNewNode->dataPair.first)
				node_type = NODE_TYPE::RCHILD;
				
			else if (pNode->dataPair.first > pNewNode->dataPair.first)
				node_type = NODE_TYPE::LCHILD;

			// ���� ���� ��� - �ߺ� Ű�� ������� �ʴ´�.
			else
				return false;


			// ó��
			if (pNode->arrNode[(int)node_type])
				// ������ �ڸ��� ������ ������ ��������
				pNode = pNode->arrNode[(int)node_type];

			else
			{
				// ������ �ڸ��� ������� �ű⿡ ����
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

	// ������ �ڽ��� �ִ� ���, ������ �ڽ����� ���� ���� �ڽ��� ���� ������ �������� �������� -> ������ ���� �ڽ��� �ļ���
	if (_pNode->arrNode[(int)NODE_TYPE::RCHILD])
	{
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::RCHILD];

		while (pSuccessor->arrNode[(int)NODE_TYPE::LCHILD])
		{
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::LCHILD];
		}
	}

	// ������ �ڽ��� ���� ���, �ڽ��� �θ��� ���� �ڽ��� ������ �ö󰣴� -> �θ� �ļ���
	else
	{
		pSuccessor = _pNode;

		while (true)
		{
			// ��Ʈ ��� üũ
			if (pSuccessor->isRoot())
				break;


			// �θ��� ���� �ڽ����� üũ
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
	// ������ ����ϰ� �����ڰ�~~
	tBSTNode<T1, T2>* pSuccessor = nullptr;

	// ���� �ڽ��� �ִ� ���, ���� �ڽ����� ���� ������ �ڽ��� ���� ������ ���������� �������� -> ������ ������ �ڽ��� ������
	if (_pNode->arrNode[(int)NODE_TYPE::LCHILD])
	{
		pSuccessor = _pNode->arrNode[(int)NODE_TYPE::LCHILD];

		while (pSuccessor->arrNode[(int)NODE_TYPE::RCHILD])
		{
			pSuccessor = pSuccessor->arrNode[(int)NODE_TYPE::RCHILD];
		}
	}

	// ���� �ڽ��� �ִ� ���, �ڽ��� �θ��� ������ �ڽ��� ������ �ö󰣴� -> �θ� ������
	else
	{
		pSuccessor = _pNode;

		while (true)
		{
			// ��Ʈ ��� üũ
			if (pSuccessor->isRoot())
				break;


			// �θ��� ������ �ڽ����� üũ
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

	// ���� ���� ��带 ã�´� == ���� ��尡 nullptr �� ������ ã�´�
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

	// ���� ������ ��带 ã�´� == ������ ��尡 nullptr �� ������ ã�´�
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
		// node_type ����ֱ�
		if (pNode->dataPair.first < _find)
			node_type = NODE_TYPE::RCHILD;

		else if (pNode->dataPair.first > _find)
			node_type = NODE_TYPE::LCHILD;

		else
			// ���� ���� ��� - ã��!!!
			break;


		// ó��
		if (pNode->arrNode[(int)node_type])
			// Ž�� ���
			pNode = pNode->arrNode[(int)node_type];

		else
		{
			// ��ã��!!
			pNode = nullptr;
			break;
		}
	}

	return iterator(this, pNode);
}
