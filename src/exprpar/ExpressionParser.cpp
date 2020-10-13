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

#include "exprpar/ExpressionParser.hpp"
#include "exprpar/ExpressionNode.hpp"

#include <cstddef>
#include <stack>
#include <vector>
#include <utility>
#include <stdexcept>
//#include <iostream>

namespace ortis
{

const std::string ExpressionParser::WHITESPACE_CHARS(" \n\r\t\f\v");
const std::string ExpressionParser::BAD_START_CHARS("1234567890.!@$^&*()-+=\\|;:\"'<>,?/~`[]{}");
const std::string ExpressionParser::VALID_NAME_CHARS("1234567890_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const std::string ExpressionParser::VALID_NUMBER_CHARS("1234567890.eE+-");
const std::string ExpressionParser::DIGIT_CHARS("1234567890");
const std::string ExpressionParser::OPERATOR_CHARS("*/+-");
const std::string ExpressionParser::BRACKET_CHARS("()");
const std::string ExpressionParser::VALUE_CHARS("1234567890_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.");

const std::map<std::string,unsigned char> ExpressionParser::OPERATOR_PRECEDENCE
{
	{ "-", 1 }, //binary minus
    { "+", 1 }, //binary plus
    { "*", 2 }, //binary multiply
    { "/", 2 }, //binary divide
    { "u-", 3}, //unary minus (sign negation)
    { "u+", 3}, //unary plus
};

std::vector<ExpressionToken>
ExpressionParser::tokenize(const std::string& expr_str) const
{
	if(expr_str.empty())
	{
		return std::vector<ExpressionToken>();
	}

	std::vector<ExpressionToken> tokens;

	std::size_t pos_begin = 0;
	std::size_t pos_end   = 0;

	while(pos_begin < expr_str.size() )
	{
        pos_begin = expr_str.find_first_not_of(WHITESPACE_CHARS, pos_begin);

        if(pos_begin == std::string::npos)
		{
			break;
		}

		pos_end = pos_begin+1;

		switch( expr_str[pos_begin] )
		{
			case '*':

				tokens.push_back
				(
					ExpressionToken
					(
						ExpressionToken::OPERATOR,
						std::string("*"),
						OPERATOR_PRECEDENCE.at("*"),
						2
					)
				);

			break;

			case '/':

				tokens.push_back
				(
					ExpressionToken
					(
						ExpressionToken::OPERATOR,
						std::string("/"),
						OPERATOR_PRECEDENCE.at("/"),
						2
					)
				);

			break;

			case '+':
			{

				unsigned char num_operands = 2;
				std::string operator_symbol;

				if
				(
					tokens.empty() ||
					(
						tokens.back().getType() != ExpressionToken::VALUE &&
						tokens.back().getType() != ExpressionToken::RIGHT_BRACKET
					)
				)
				{
					num_operands = 1; //unary
					operator_symbol = "u+";
				}
				else
				{
					num_operands = 2; //binary
					operator_symbol = "+";
				}

				tokens.push_back
				(
					ExpressionToken
					(
						ExpressionToken::OPERATOR,
						operator_symbol,
						OPERATOR_PRECEDENCE.at(operator_symbol),
						num_operands
					)
				);
			}
			break;

			case '-':
			{
				unsigned char num_operands = 2;
				std::string operator_symbol;

				if
				(
					tokens.empty() ||
					(
						tokens.back().getType() != ExpressionToken::VALUE &&
						tokens.back().getType() != ExpressionToken::RIGHT_BRACKET
					)
				)
				{
					num_operands = 1; //unary
					operator_symbol = "u-";
				}
				else
				{
					num_operands = 2; //binary
					operator_symbol = "-";
				}

				tokens.push_back
				(
					ExpressionToken
					(
						ExpressionToken::OPERATOR,
						operator_symbol,
						OPERATOR_PRECEDENCE.at(operator_symbol),
						num_operands
					)
				);
			}
			break;

			case '(':

				tokens.push_back
				(
					ExpressionToken
					(
						ExpressionToken::LEFT_BRACKET,
						std::string("(")
					)
				);

			break;

			case  ')':

				tokens.push_back
				(
					ExpressionToken
					(
						ExpressionToken::RIGHT_BRACKET,
						std::string(")")
					)
				);

			break;

			default:

				if( !isValueChar(expr_str[pos_begin]) )
				{
                    throw
                    std::invalid_argument
                    (
						std::string
						(
							"std::vector<ExpressionToken> "
							"ExpressionParser::tokenize(const std::string& expr_str) -- "
							"expr_str has unsupported token at zeroth position "
						) +

						std::to_string(pos_begin)
					);
				}

				pos_end = expr_str.find_first_not_of(VALUE_CHARS, pos_begin+1);
				std::size_t length = (pos_end == std::string::npos) ? (std::string::npos) : (pos_end-pos_begin);

				tokens.push_back
				(
					ExpressionToken
					(
						ExpressionToken::VALUE,
						expr_str.substr(pos_begin, length),
						0
					)
				);



			break;

		}

        pos_begin = pos_end;
	}

	return tokens;
}

std::vector<ExpressionToken>
ExpressionParser::tokenizeToPostfix(const std::string& expr_str) const
{
	std::vector<ExpressionToken> infix_tokens = tokenize(expr_str);

	std::stack<ExpressionToken> operator_stack;
	std::vector<ExpressionToken> output_queue;

    for(auto& token : infix_tokens)
	{
        switch(token.getType())
        {
			case ExpressionToken::VALUE :

				output_queue.push_back(std::move(token));

			break;

			case ExpressionToken::OPERATOR :

				while
				(
					!operator_stack.empty() &&
					(
						operator_stack.top().hasHigherPrecedence(token.getPrecedence()) ||
						operator_stack.top().hasEqualPrecedence(token.getPrecedence())
					) &&
					( operator_stack.top().getType() != ExpressionToken::LEFT_BRACKET )
				)
				{
                    output_queue.push_back( std::move( operator_stack.top() ) );
                    operator_stack.pop();
				}

				operator_stack.push(std::move(token));

			break;

			case ExpressionToken::LEFT_BRACKET :

				operator_stack.push(std::move(token));

			break;

			case ExpressionToken::RIGHT_BRACKET :

                while
				(
					!operator_stack.empty() &&
					( operator_stack.top().getType() != ExpressionToken::LEFT_BRACKET)
				)
				{
					output_queue.push_back(std::move(operator_stack.top()));
					operator_stack.pop();
				}

				if
				(
					!operator_stack.empty() &&
					operator_stack.top().getType() == ExpressionToken::LEFT_BRACKET
				)
				{
					operator_stack.pop();
				}

			break;

			default:

			break;
        }
	}

	while(!operator_stack.empty())
	{
		output_queue.push_back(std::move(operator_stack.top()));
		operator_stack.pop();
	}

	return output_queue;
}

std::string
ExpressionParser::expressAsPostfix(const std::string& expr_str) const
{
	const auto expr_tokens = tokenizeToPostfix(expr_str);

	std::string new_expression;

	for(const auto& token : expr_tokens)
	{
		new_expression += token.getSymbol() + " ";
	}

	return new_expression;
}

Expression
ExpressionParser::parse(const std::string& expr_str) const
{
	if(expr_str.empty())
	{
		return Expression();
	}

	std::vector<ExpressionToken> infix_tokens = tokenize(expr_str);

	std::stack<ExpressionToken> operator_stack;
	std::stack<ExpressionNode> output_stack;

    for(auto& token : infix_tokens)
	{
        switch(token.getType())
        {
			case ExpressionToken::VALUE :

				output_stack.push( ExpressionNode(token) );

			break;

			case ExpressionToken::OPERATOR :

				while
				(
					!operator_stack.empty() &&
					(
						operator_stack.top().hasHigherPrecedence(token.getPrecedence()) ||
						operator_stack.top().hasEqualPrecedence(token.getPrecedence())
					) &&
					( operator_stack.top().getType() != ExpressionToken::LEFT_BRACKET )
				)
				{
                    addNode(output_stack, operator_stack.top() );
                    operator_stack.pop();
				}

				operator_stack.push(std::move(token));

			break;

			case ExpressionToken::LEFT_BRACKET :

				operator_stack.push(std::move(token));

			break;

			case ExpressionToken::RIGHT_BRACKET :

                while
				(
					!operator_stack.empty() &&
					( operator_stack.top().getType() != ExpressionToken::LEFT_BRACKET)
				)
				{
					addNode(output_stack, operator_stack.top() );
					operator_stack.pop();
				}

				if
				(
					!operator_stack.empty() &&
					operator_stack.top().getType() == ExpressionToken::LEFT_BRACKET
				)
				{
					operator_stack.pop();
				}

			break;

			default:

			break;
        }
	}

	while(!operator_stack.empty())
	{
		addNode(output_stack, operator_stack.top() );
		operator_stack.pop();
	}

	return Expression(std::move(output_stack.top()));
}

void
ExpressionParser::addNode(std::stack<ExpressionNode>& nodes, const ExpressionToken& token) const
{
    if
	(
		token.getType() != ExpressionToken::VALUE &&
		token.getType() != ExpressionToken::OPERATOR
	)
	{
		return;
	}

    switch (token.getNumberOfOperands())
    {
		case 0: //value
		{
			nodes.push
			(
				ExpressionNode
				(
					token,
					nullptr,
					nullptr
				)
			);
		}
		break;

		case 1: //unary
		{

			if(nodes.size() < 1)
			{
				throw
				std::runtime_error
				(
					"void "
					"ExpressionParser::addNode(std::stack<ExpressionNode>& nodes, "
					"const ExpressionToken& token) -- "
					"unary operator has fewer than 1 operand"
				);
			}

			ExpressionNode left_node  = std::move( nodes.top() );
			nodes.pop();

			nodes.push
			(
				ExpressionNode
				(
					token,
					std::make_unique<ExpressionNode>(std::move(left_node)),
					nullptr
				)
			);
		}
		break;

		case 2: //binary
		{
			if(nodes.size() < 2)
			{
				throw
				std::runtime_error
				(
					"void "
					"ExpressionParser::addNode(std::stack<ExpressionNode>& nodes, "
					"const ExpressionToken& token) -- "
					"binary operator has fewer than 2 operands"
				);
			}

			ExpressionNode right_node = std::move( nodes.top() );
			nodes.pop();

			ExpressionNode left_node  = std::move( nodes.top() );
			nodes.pop();

			nodes.push
			(
				ExpressionNode
				(
					token,
					std::make_unique<ExpressionNode>(std::move(left_node)),
					std::make_unique<ExpressionNode>(std::move(right_node))
				)
			);
		}
		break;

		default:
		{

		}
		break;

    }
}

#if 1
bool
ExpressionParser::isWhitespace(char c) const
{
	return ( WHITESPACE_CHARS.find(c) != std::string::npos );
}

bool
ExpressionParser::isBracket(char c) const
{
	return ( BRACKET_CHARS.find(c) != std::string::npos );
}

bool
ExpressionParser::isOperator(char c) const
{
	return ( OPERATOR_CHARS.find(c) != std::string::npos );
}

#endif

bool
ExpressionParser::isValueChar(char c) const
{
	return ( VALUE_CHARS.find(c) != std::string::npos );
}

} //namespace ortis

