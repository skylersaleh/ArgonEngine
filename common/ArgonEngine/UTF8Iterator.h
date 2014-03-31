//Generated by the Argon Build System

/**
 * @brief This file provides and implementation of an std::string iterator that iterates for UTF-8 characters instead of bytes.
 * @file UTF8Iterator.h
 * @author Skyler Saleh
 *
 * @details
 * UTF-8 is a variable length encoding of text that is backwards compatible with ASCII. With standard ASCII, you can only represent 128 different characters,
 * a good portion of which are control codes. This makes ASCII unsuitable for things like symbols, and foreign languages. In Unicode you can represent over 1,000,000 different character.
 * Also UTF-8 is an Industry standard and is used just about everywhere. However, iterating over an UTF-8 string is a little more difficult than an ASCII string. This is because in ASCII
 * each character is guaranteed to only be one byte. However in UTF-8 each character can be anywhere between 1 and 6 bytes long. Which means you can not iterate through UTF-8 strings
 * in the same way that you can a ASCII one, and you can't use the std::string iterator. This class remedies this by providing a UTF-8 iterator for std::string.
 **/

#ifndef UTF8Iterator_h
#define UTF8Iterator_h
#include <string>
namespace Argon {
/**
 * @brief This class provides and implementation of an std::string iterator that iterates for UTF-8 characters instead of bytes.
 * @details
 * UTF-8 is a variable length encoding of text that is backwards compatible with ASCII. With standard ASCII, you can only represent 128 different characters,
 * a good portion of which are control codes. This makes ASCII unsuitable for things like symbols, and foreign languages. In Unicode you can represent over 1,000,000 different character.
 * Also UTF-8 is an Industry standard and is used just about everywhere. However, iterating over an UTF-8 string is a little more difficult than an ASCII string. This is because in ASCII
 * each character is guaranteed to only be one byte. However in UTF-8 each character can be anywhere between 1 and 6 bytes long. Which means you can not iterate through UTF-8 strings
 * in the same way that you can a ASCII one, and you can't use the std::string iterator. This class remedies this by providing a UTF-8 iterator for std::string.
 **/
struct UTF8Iterator : public std::iterator<std::bidirectional_iterator_tag, uint32_t, std::string::difference_type, const uint32_t*, const uint32_t&>
{
    typedef std::string::const_iterator raw_iterator;
    raw_iterator it;
    UTF8Iterator(){};
    UTF8Iterator(const std::string::iterator it) : it(it){};
    UTF8Iterator(const raw_iterator it) : it(it){};
    UTF8Iterator(const UTF8Iterator& other) : it(other.it){};
    UTF8Iterator& operator=(const UTF8Iterator& other){
        it = other.it; return *this;
    }
    UTF8Iterator& operator=(const raw_iterator other){
        it = other;
        return *this;
    }
    UTF8Iterator& operator--(){
        const unsigned char masks[2]={128,64};
        if(*(--it)&masks[0])
            if (!(*(--it) & masks[1]))
                if (!(*(--it) & masks[1])) --it;
        return *this;
    }
    UTF8Iterator& operator++(){
        const unsigned char masks[4]={128,32,16,8};
        unsigned char byte = *it;
        int offset = byte & masks[0] ? byte & masks[1] ? byte & masks[2] ? 4 : 3 : 2 : 1;
        it += offset;
        return *this;
    }
    UTF8Iterator operator--(int){
        UTF8Iterator t = *this; --(*this); return t;
    }
    UTF8Iterator operator++(int){
        UTF8Iterator t = *this; ++(*this); return t;
    }
    bool operator==(const UTF8Iterator& other) const {
        return it==other.it;
    }
    bool operator!=(const UTF8Iterator& other) const {
        return it!=other.it;
    }
    bool operator<(const UTF8Iterator& other) const {
        return it<other.it;
    }
    bool operator>(const UTF8Iterator& other) const {
        return it>other.it;
    }
    bool operator<=(const UTF8Iterator& other) const {
        return it<=other.it;
    }
    bool operator>=(const UTF8Iterator& other) const {
        return it>=other.it;
    }
    uint32_t operator*() const {
        uint32_t codePoint = 0;

        char bytes[4] = {*it,*(it+1),*(it+2),*(it+3)};
        const unsigned char masks[3]={128,32,16};

        if(bytes[0] & masks[0])
        {
            if(bytes[0] & masks[1])
            {
                if(bytes[0] & masks[2])
                {
                    codePoint = (bytes[0] & 0x07) << 18;
                    codePoint +=  (bytes[1] & 0x3f) << 12;
                    codePoint +=  (bytes[2] & 0x3f) << 6;
                    codePoint += (bytes[3] & 0x3f);
                }
                else
                {
                    codePoint = (bytes[0] & 0x0f) << 12;
                    codePoint += (bytes[1] & 0x3f) << 6;
                    codePoint +=  (bytes[2] & 0x3f);
                }
            }
            else
            {
                codePoint = (bytes[0] & 0x1f) << 6;
                codePoint +=  (bytes[1] & 0x3f);
            }
        }
        else
        {
            codePoint = bytes[0];
        }

        return codePoint;

    }
    ~UTF8Iterator(){}
};
}
#endif
