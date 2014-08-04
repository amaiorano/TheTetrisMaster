#ifndef _GAME_NODE_H_
#define _GAME_NODE_H_

#include <string>
#include "Geom2D.h"

typedef Point2d<float> NodePos;
typedef Size2d<float> NodeSize;

class GameNode
{
public:
	// Node name
	std::string& Name()					{ return m_strName; }
	const std::string& Name() const		{ return m_strName; }

	// Node position
	NodePos& Pos()					{ return m_pos; }
	const NodePos& Pos() const		{ return m_pos; }

	// NodeSize size
	NodeSize& Size()				{ return m_size; }
	const NodeSize& Size() const	{ return m_size; }

protected:
	GameNode() : m_strName("Unnamed") { }
	/*virtual*/ ~GameNode() {}

	std::string m_strName;
	NodePos m_pos;
	NodeSize m_size;
};

#endif // _GAME_NODE_H_
