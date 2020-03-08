/*

Copyright (C) 2019-2020 Matthew Milton

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

#ifndef LBLMC_STRINGPROCESSOR_HPP
#define LBLMC_STRINGPROCESSOR_HPP

#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

#include <iostream>

namespace lblmc
{

/**
	\brief Utility class used to systematically edit and check character strings (std::string)

	\author Matthew Milton
	\date 2019
**/
class StringProcessor
{

private:

	std::string* str;	///< observing (non-owning) pointer to string that is to be processed

public:

//==================================================================================================
	// CONSTRUCTORS

	/**
		\brief default constructor (deleted)
	**/
	StringProcessor() = delete;

	/**
		\brief parameter constructor

		StringProcessor must be assigned an existing string to process.

		\param str the existing string that will be processed.  This string must persist as long,
		or longer, in scope as the constructed StringProcessor
	**/
	explicit StringProcessor(std::string& str) : str(&str) {}

	/**
		\brief copy constructor
	**/
	StringProcessor(const StringProcessor& base) : str(base.str) {}

//==================================================================================================
	// OPERATORS

	/**
		\brief assignment operator

		Assigns an existing string to the processor

		\param rhs the existing string that will be processed
		\return *this
	**/
	inline StringProcessor& operator=(std::string& rhs)
	{
		str = &rhs;
		return *this;
	}

	/**
		\brief assignment operator

		Assigns another StringProcessor to the processor

		\param rhs the StringProcessor to assign from
		\return *this
	**/
	inline StringProcessor& operator=(const StringProcessor& rhs)
	{
		str = rhs.str;
		return *this;
	}

//==================================================================================================
	// ACCESSORS

	/**
		\brief sets existing string to process
		\param str the existing string that will be processed
	**/
	inline void setString(std::string& str) { this->str = &str; }

	/**
		\brief gets string that is being processed
		\return the processed string
	**/
	inline std::string& getString() { return *str; }

	/**
		\brief makes a copy of the processed string
		\return copy of the processed string
	**/
	inline std::string copyString() { return std::string(*str); }

//==================================================================================================
	// CHECKERS

	/**
		\brief determines if a character is delimiter for a word

		A word is any string that is delimited by either whitespace, nothing, or punctuation/display
		characters (#;/+*-,<>()$ etc.) in a larger string.  Characters that are not word delimiters
		include digits (0-9), underscore (_) and letters (a-z, A-Z), so if a string is delimited by
		them, the string is not a word.

		Note that a string can contain the word delimiter characters within but not be a word if
		the string is delimited by the non-word delimiters in larger string.
		Example of a non word string with word delimiters can include $cat in abc$catdef as $cat is
		delimited by letters; while abc $cat#def or abc;$cat,def has $cat being a word.

		\param c the character to check if a word delimiter
		\return true if a word delimiter; false otherwise
	**/
	inline bool isWordDelimiter(int c)
	{
		return (ispunct(c) || isspace(c) || c=='\0')&&(c!='_');
	}

	/**
		\brief determines if a string is a word within a larger string

		A word is any string that is delimited by either whitespace, nothing, or punctuation/display
		characters (#;/+*-,<>()$ etc.) in a larger string.  Characters that are not word delimiters
		include digits (0-9) and letters (a-z, A-Z), so if a string is delimited by them, the
		string is not a word.

		Note that a string can contain the word delimiter characters within but not be a word if
		the string is delimited by the non-word delimiters in larger string.
		Example of a non word string with word delimiters can include $cat in abc$catdef as $cat is
		delimited by letters; while abc $cat#def or abc;$cat,def has $cat being a word.

		\param word string to check if a word or not
		\param containing_str the string that contains the possible word string
		\param starting character 0th index position in containing_str to begin the check
		\return true if a word argument is a word; false otherwise
	**/
	inline bool isWord(const std::string& word, const std::string& containing_str, int start_pos=0)
	{
		int pos = containing_str.find(word, start_pos);
		if(pos < 0) return false;

		char c = ' ';

		if(pos != 0)
			c = containing_str[pos-1];
        if(!isWordDelimiter(c)) return false;

        if(pos+word.size() < str->size())
			c = containing_str[pos+word.size()];
		if(!isWordDelimiter(c)) return false;

		return true;
	}

//==================================================================================================
	// FINDERS, SEARCHERS, AND FETCHERS

	/**
		\brief checks if a substring exists in the processed string
	**/
	inline bool containsString(std::string search_str)
	{
		return (str->find(search_str) != std::string::npos);
	}

	/**
		\brief checks if a word exists in the processed string
	**/
	inline bool containsWord(std::string search_str)
	{
		return isWord(search_str, *str, 0);
	}

	/**
		\brief finds first instance of a substring in the processed string and returns its 0-th
		character position
		\return position of first character of first instance of string; -1 otherwise
	**/
	inline int findString(std::string search_str, int start_pos = 0)
	{
		return str->find(search_str, start_pos);
	}

	/**
		\brief finds all instances of a substring in the processed string and returns each instance's
		0-th character starting position
		\return a vector of the integer positions; the vector is empty if no instances found
	**/
	inline std::vector<int> findStringAll(std::string search_str)
	{
		std::vector<int> ret;

		int pos = -1;

		while(true)
		{
			pos = str->find(search_str, pos+1);
			if(pos >= 0)
			{
				ret.push_back(pos);
			}
			else
			{
				break;
			}
		}

		return ret;
	}

	/**
		\brief finds first instance of a word in the processed string and returns its 0-th
		character position
		\return position of first character of first instance of word; -1 otherwise
	**/
    inline int findWord(std::string search_str, int start_pos = 0)
    {
    	int pos = start_pos-1;

        while(true)
		{
			pos = str->find(search_str, pos+1);
			if(pos < 0) return pos;

			if(isWord(search_str, *str, pos))
				return pos;
			else
				continue;
		}

		return -1;
    }

    /**
		\brief finds all instances of a word in the processed string and returns each instance's
		0-th character position
		\return a vector of the integer positions; the vector is empty if no instances found
	**/
    inline std::vector<int> findWordAll(std::string search_str)
    {
    	std::vector<int> ret;

		int pos = -1;

		while(true)
		{
			pos = str->find(search_str, pos+1);
			if(pos >= 0)
			{
				if(isWord(search_str,*str,pos))
					ret.push_back(pos);
			}
			else
			{
				break;
			}
		}

		return ret;
    }

    /**
		\brief finds the first substring line from given starting position in processed string
		that ends with given delimiter string

		To get the position of the delimiter, use findString(delim, start_pos).  The length of the
		returned line string also indicates position by (length - start_pos).

		\param delim delimiter string that line ends with
		\param start_pos starting 0-th position in processed string to begin searching for the line
		\return the delimited line; returns std::string("") if delimiter is not found or line is empty

		\todo test this method (Feb 24, 2019)
    **/
    inline
	std::string fetchLine(std::string delim = "\n", int start_pos = 0)
	{
		int pos = str->find(delim, start_pos);

		if(pos == std::string::npos) return std::string("");

		return str->substr(start_pos, pos);
	}

	/**
		\brief finds the first substring line from given starting position in processed string
		that ends with given delimiter string

		To get the position of the delimiter, use findString(delim, start_pos).  The length of the
		returned line string also indicates position by (length - start_pos).  Also, the position
		can be found from (post_delim_pos - delimiter length).

		\param delim delimiter string that line ends with
		\param post_delim_pos position immediately after end of delimiter; sets to -1 if no
		delimiter found or at end of processed string
		\param start_pos starting 0-th position in processed string to begin searching for the line
		\return the delimited line; returns std::string("") if delimiter is not found or line is empty

		\todo test this method (Feb 24, 2019)
    **/
    inline
	std::string fetchLine(int& post_delim_pos, std::string delim = "\n", int start_pos = 0)
	{
		int pos = str->find(delim, start_pos);

		if(pos == std::string::npos)
		{
			post_delim_pos = pos;
			return std::string("");
		}

		int post_pos = pos + delim.length();

		if(post_pos < str->length() )
			post_delim_pos = post_pos;
		else
			post_delim_pos = std::string::npos;

		return str->substr(start_pos, pos);
	}

    /**
		\brief finds the first substring from given starting position in processed string that is delimited by given delimiter strings
		\param pre_delimiter the starting delimiter that delimits the substring; argument of "" indicates start of processed string at start_pos
		\param post_delimiter the ending delimiter that delimits the substring; argument of "" indicates end of processed string
		\param start_pos starting 0-th position in processed string to begin searching for the delimiters
		\return 2-element int array where 1st element is the position of the substring and 2nd element is the length of the substring
		If 1st element is negative, then no delimited string was found.
    **/
    inline
    std::array<int,2>
    findDelimitedString(std::string pre_delimiter, std::string post_delimiter, int start_pos = 0)
    {
		if(pre_delimiter == "" && post_delimiter == "")
		{
			if(str->size() == 0)
				return std::array<int,2>{-1,0};
			else
				return std::array<int,2>{0,str->size()};
		}

		int str_start = 0;
		int str_end = 0;

		if(pre_delimiter == "")
			str_start = start_pos;
		else
		{
			str_start = str->find(pre_delimiter,start_pos);
			if(str_start < 0) return std::array<int,2>{-1,0};
			str_start += pre_delimiter.size();
		}

		if(post_delimiter == "")
			str_end = str->size()-1;
		else
		{
			str_end = str->find(post_delimiter, str_start);
			if(str_end < 0) return std::array<int,2>{-1,0};
			str_end -= 1;
		}

		return std::array<int,2>{str_start,str_end-str_start+1};
    }

    /**
		\brief fetches the first substring from starting position in processed string that is delimited by given delimiter strings
		\param pre_delimiter the starting delimiter that delimits the substring; argument of "" indicates start of processed string at start_pos
		\param post_delimiter the ending delimiter that delimits the substring; argument of "" indicates end of processed string
		\param start_pos starting 0-th position in processed string to begin searching for the delimiters
		\return a string that contains the delimited string; string is empty if no substring is delimited by given delimiters from start_pos
    **/
    inline std::string fetchDelimitedString(std::string pre_delimiter, std::string post_delimiter, int start_pos = 0)
    {
    	if(pre_delimiter == "" && post_delimiter == "") return str->substr(start_pos, std::string::npos);

    	int str_start = 0;
		int str_end = 0;

		if(pre_delimiter == "")
			str_start = start_pos;
		else
		{
			str_start = str->find(pre_delimiter,start_pos);
			if(str_start < 0) return std::string("");
			str_start += pre_delimiter.size();
		}

		if(post_delimiter == "")
			str_end = str->size()-1;
		else
		{
			str_end = str->find(post_delimiter, str_start);
			if(str_end < 0) return std::string("");
			str_end -= 1;
		}

		return str->substr(str_start, str_end-str_start+1);
    }

    /**
		\brief tokenizes the processed string into collection of substrings separated by word
		delimiters

		This method divides up the processed string into a collection of substrings, called tokens,
		which is delimited from one another in the processed string by word delimiters.

		The processed string is not altered by the tokenization.

		\param start_pos zeroth position in processed string to start tokenization
		\param length length of characters from start_pos to include in the tokenization
		\return vector containing the substring tokens; vector is empty if no tokens are found

		\todo test this method (Feb 24, 2019)
    **/
    inline
    std::vector<std::string>
    tokenizeString(std::string delim, int start_pos = 0, int length = -1)
    {
    	std::vector<std::string> tokens;

    	int pos = 0;

    	while (pos != -1)
		{
			 tokens.push_back( fetchLine(pos, delim, pos) );
		}

    	return tokens;
    }

//==================================================================================================
	// STRING/WORD REPLACERS

	/**
		\brief replaces first instance of a string with a new string in processed string, searching from given position
		\param old_str the string that will be replaced
		\param new_str the replacement string
		\param start_pos zeroth position in processed string on where to start the search and replace
		\return zeroth position in processed string immediately after the replaced string; -1 if no word is replaced
	**/
	inline int replaceString(std::string old_str, std::string new_str, int start_pos = 0)
	{
        if(new_str == old_str) return std::string::npos;

        int pos = str->find(old_str, start_pos);
        if(pos < 0) return std::string::npos;

        std::string before = str->substr(0,pos);
        std::string after  = str->substr(pos+old_str.size());

        *str = before;
        *str += new_str;
        *str += after;

        return pos+new_str.size();
	}

	inline void replaceStringAll(std::string old_str, std::string new_str, int start_pos=0)
	{
		int pos = start_pos;

		do
		{
			pos = replaceString(old_str, new_str, pos);
		}
		while( pos >= 0 );
	}

	/**
		\brief replaces all instances of given strings contained in processed string with new strings

		This method is useful to replace a collection of strings easily in vectorized form

		\param old_strs vector of strings to replace
		\param new_strs vector of the replacement strings; must equal size of old_strs where each
		element corresponds to a string in old_strs at same vector positions (i.e. replace
		old_strs[i] with new_strs[i] ).

		\throw std::invalid_argument() if old_strs size does not equal new_strs size
	**/
	inline void replaceStringAll(std::vector<std::string> old_strs, std::vector<std::string> new_strs)
	{
		if(old_strs.size() != new_strs.size())
		{
			throw std::invalid_argument("StringProcessor::replaceStringAll(vector,vector): number of old_strs must equal number of new_strs");
		}

		for(int i = 0; i< old_strs.size(); i++)
		{
			replaceStringAll(old_strs[i], new_strs[i]);
		}
	}

	/**
		\brief replaces first instance of a word with a new word in processed string, searching from given position
		\param old_word the word that will be replaced
		\param new_word the replacement word
		\param start_pos zeroth position in processed string on where to start the search and replace
		\return zeroth position in processed string immediately after the replaced word; -1 if no word is replaced
	**/
	inline int replaceWord(std::string old_word, std::string new_word, int start_pos = 0)
	{
		if(new_word == old_word) return std::string::npos;

        int pos = start_pos-1;

        while(true)
		{
			//std::cout << "hi! ";

			pos = str->find(old_word, pos+1);
			if(pos < 0) return std::string::npos;

			char c = ' ';

			if(pos != 0)
				c = (*str)[pos-1];

            if(!isWordDelimiter(c)) continue;

            //if(pos+old_word.size()+1 < str->size())
            if(pos+old_word.size() < str->size())
			{
				c = (*str)[pos+old_word.size()];
			}

			if(!isWordDelimiter(c))
			{
				//std::cout << c << " NOT a word delimiter\n";
				continue;
			}
			else
			{
				//std::cout << c << " IS a word delimiter\n";
				break;
			}
		}
		//std::cout << "bye! ";

        std::string before = str->substr(0,pos);
        std::string after  = str->substr(pos+old_word.size());

        *str = before;
        *str += new_word;
        *str += after;

        return pos+new_word.size(); //CHECK IF THIS CORRECT
	}

	/**
		\todo check if new code works correctly! 02.03.2019.2119
		\brief replaces all instances of a word contained in processed string with a new word
		\param old_word the word to replace
		\param new_word the replacement word
	**/
	inline void replaceWordAll(std::string old_word, std::string new_word, int start_pos=0)
	{
		int pos = start_pos;

		do
		{
			pos = replaceWord(old_word, new_word, pos);
		}
		while( pos >= 0 );
	}

	/**
		\brief replaces all instances of given words contained in processed string with new words

		This method is useful to replace a collection of words easily in vectorized form

		\param old_words vector of words to replace
		\param new_words vector of the replacement word; must equal size of old_words where each
		element corresponds to a word in old_word at same vector positions (i.e. replace
		old_words[i] with new_words[i] ).

		\throw std::invalid_argument() if old_words size does not equal new_words size
	**/
	inline void replaceWordAll(std::vector<std::string> old_words, std::vector<std::string> new_words)
	{
		if(old_words.size() != new_words.size())
		{
			throw std::invalid_argument("StringProcessor::replaceWordAll(vector,vector): number of old_words must equal number of new_words");
		}

		for(int i = 0; i< old_words.size(); i++)
		{
			replaceWordAll(old_words[i], new_words[i]);
		}
	}

	inline void replaceDelimitedString(std::string pre_delimiter, std::string post_delimiter, std::string replace_str, int start_pos = 0)
	{
		if(pre_delimiter.empty() && post_delimiter.empty()) return;

		std::array<int,2> str_pos_length = findDelimitedString(pre_delimiter,post_delimiter,start_pos);
		if(str_pos_length[0] == -1) return;

		std::string before = str->substr(0,str_pos_length[0]);
		std::string after  = str->substr(str_pos_length[0]+str_pos_length[1]);

		*str  = before;
		*str += replace_str;
		*str += after;
	}

//==================================================================================================

};

} //namespace lblmc

#endif // LBLMC_STRINGPROCESSOR_HPP
