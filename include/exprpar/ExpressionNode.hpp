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

#ifndef EXPRPAR_EXPRESSIONNODE_HPP
#define EXPRPAR_EXPRESSIONNODE_HPP

#include <string>
#include <memory>

#include "exprpar/ExpressionToken.hpp"

namespace ortis
{

/**
	\brief Embodies a node within an expression syntax tree

	\author Matthew Milton

	\date June 2020
**/
class ExpressionNode
{

private:

	ExpressionToken token;                       ///< expression token for this node
	std::unique_ptr<ExpressionNode> left_node;   ///< child left node representing left operand for operator node; null for value nodes
	std::unique_ptr<ExpressionNode> right_node;  ///< child right node representing right operand for operator node; null for value nodes

public:

	/**
		\brief default constructor

		\param token expression token that embodies what type is node (operator, value, etc.)

		\param left_node child left node representing left operand for operator node; null for
		value nodes. The constructed node takes ownership of this given left_node.

		\param right_node child right node representing right operand for operator node; null for
		value nodes. The constructed node takes ownership of this given right_node.
	**/
	ExpressionNode
	(
		const ExpressionToken& token = ExpressionToken(),
		std::unique_ptr<ExpressionNode> left_node  = std::unique_ptr<ExpressionNode>(nullptr),
		std::unique_ptr<ExpressionNode> right_node = std::unique_ptr<ExpressionNode>(nullptr)
	);

	/**
		\brief move constructor
	**/
	ExpressionNode(ExpressionNode&& base);

	/**
		\brief move assignment operator
	**/
	ExpressionNode&
	operator=(ExpressionNode&& base);

	/**
		\brief casts expression node and its children as a single string

		The string representation of the node tree will be in normal polish notation with
		parentheses.  So for instance, the expression a*b+c will be expressed as +(*(a,b),c).

		\return string representation of the expression node tree
	**/
	std::string
	asString() const;

	/**
		\brief cast operator to std::string

		same as calling asString().
	**/
	inline
	operator std::string() const
	{
		return asString();
	}

	/**
		\brief sets token for the node
	**/
	void
	setToken(const ExpressionToken& token);

	/**
		\brief sets token for node by moving the token to node
	**/
	void
	setToken(ExpressionToken&& token);

	/**
		\brief gets node's token, allowing modification
	**/
	ExpressionToken&
	getToken();

	/**
		\brief gets node's token, disallowing modification
	**/
	const ExpressionToken&
	getToken() const;

	/**
		\brief sets left child node of node, taking ownership of it

		\throw invalid_argument if node is null
	**/
    void
    setLeftNode(std::unique_ptr<ExpressionNode> node);

    /**
		\brief gets left child node, allowing modification

		\return observing raw pointer to left child node.  Check if nullptr before dereferencing it.
		The returned pointer's associated object should persist as long as node does.
    **/
    ExpressionNode*
    getLeftNode();

     /**
		\brief gets left child node, disallowing modification

		\return observing raw pointer to left child node.  Check if nullptr before dereferencing it.
		The returned pointer's associated object should persist as long as node does.
    **/
    const ExpressionNode*
    getLeftNode() const;

    /**
		\brief sets right child node of node, taking ownership of it

		\throw invalid_argument if node is null
	**/
    void
    setRightNode(std::unique_ptr<ExpressionNode> node);

    /**
		\brief gets right child node, allowing modification

		\return observing raw pointer to right child node.  Check if nullptr before dereferencing it.
		The returned pointer's associated object should persist as long as node does.
    **/
    ExpressionNode*
    getRightNode();

    /**
		\brief gets right child node, disallowing modification

		\return observing raw pointer to right child node.  Check if nullptr before dereferencing it.
		The returned pointer's associated object should persist as long as node does.
    **/
    const ExpressionNode*
    getRightNode() const;




};

} //namespace ortis

#endif // EXPRESSIONNODE_HPP
