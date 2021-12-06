/*

Copyright (C) 2020 Matthew Milton

This file is part of the LB-LMC Solver C++ Code Generation Library, included in the
Open Real-Time Simulation (ORTiS) Framework.

LB-LMC Solver C++ Code Generation Library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LB-LMC Solver C++ Code Generation Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LB-LMC Solver C++ Code Generation Library.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "exprpar/ExpressionNode.hpp"

#include <utility>
#include <stdexcept>
#include <sstream>

namespace ortis
{

ExpressionNode::ExpressionNode
(
	const ExpressionToken& token,
	std::unique_ptr<ExpressionNode> left_node,
	std::unique_ptr<ExpressionNode> right_node
) :
	token(token),
	left_node(std::move(left_node)),
	right_node(std::move(right_node))
{}

ExpressionNode::ExpressionNode(ExpressionNode&& base) :
	token(std::move(base.token)),
	left_node(std::move(base.left_node)),
	right_node(std::move(base.right_node))
{}

ExpressionNode&
ExpressionNode::operator=(ExpressionNode&& base)
{
	token = std::move(base.token);
	left_node = std::move(base.left_node);
	right_node = std::move(base.right_node);

	return *this;
}

std::string
ExpressionNode::asString() const
{
	std::stringstream sstrm;

	sstrm <<
	token.getSymbol();

	if(token.getType() == ExpressionToken::VALUE)
	{
		return sstrm.str();
	}

	if(token.getType() == ExpressionToken::OPERATOR)
	{
		sstrm <<
		"(" <<
		( (left_node)  ? left_node->asString()  : "" ) <<
		( (right_node) ? ", "+ right_node->asString() : "" ) <<
		")";
	}

	return sstrm.str();

}

void
ExpressionNode::setToken(const ExpressionToken& token)
{
	this->token = token;
}

void
ExpressionNode::setToken(ExpressionToken&& token)
{
	this->token = token;
}

ExpressionToken&
ExpressionNode::getToken()
{
	return token;
}


const ExpressionToken&
ExpressionNode::getToken() const
{
	return token;
}

void
ExpressionNode::setLeftNode(std::unique_ptr<ExpressionNode> node)
{
	if(!node)
	{
		throw
		std::invalid_argument
		(
			"void ExpressionNode::setLeftNode(std::unique_ptr<ExpressionNode> node) -- node cannot be null"
		);
	}

	left_node = std::move(node);
}

ExpressionNode*
ExpressionNode::getLeftNode()
{
	return left_node.get();
}

const ExpressionNode*
ExpressionNode::getLeftNode() const
{
	return left_node.get();
}

void
ExpressionNode::setRightNode(std::unique_ptr<ExpressionNode> node)
{
	if(!node)
	{
		throw
		std::invalid_argument
		(
			"void ExpressionNode::setRightNode(std::unique_ptr<ExpressionNode> node) -- node cannot be null"
		);
	}

	right_node = std::move(node);
}

ExpressionNode*
ExpressionNode::getRightNode()
{
	return right_node.get();
}

const ExpressionNode*
ExpressionNode::getRightNode() const
{
	return right_node.get();
}

} //namespace ortis
