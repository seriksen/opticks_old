/*
 * Copyright (c) 2019 Opticks Team. All Rights Reserved.
 *
 * This file is part of Opticks
 * (see https://bitbucket.org/simoncblyth/opticks).
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */


#include <iostream>
#include <iomanip>
#include <cstring>
#include <cassert>
#include <sstream>

#include "SASCII.hh"


const char SASCII::UPPER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;   
const char SASCII::LOWER[] = "abcdefghijklmnopqrstuvwxyz" ;  
const char SASCII::NUMBER[] = "0123456789" ;  
const char SASCII::OTHER[] = "_" ; 
const char SASCII::ALLOWED[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789"
"_" 
 ; 

unsigned SASCII::Count( char c, const char* list )
{
    int count(0) ;  
    for(unsigned i=0 ; i < strlen(list) ; i++ )
    {
        if(list[i] == c) count++ ;   
    }
    return count ; 
}
 
bool SASCII::IsUpper( char c ){ return 1 == Count(c, UPPER) ; }
bool SASCII::IsLower( char c ){ return 1 == Count(c, LOWER) ; }
bool SASCII::IsNumber( char c ){ return 1 == Count(c, NUMBER) ; }
bool SASCII::IsOther( char c ){ return 1 == Count(c, OTHER) ; }
bool SASCII::IsAllowed( char c ){ return 1 == Count(c, ALLOWED) ; }


char SASCII::Classify( char c )
{
   if(IsUpper(c)) return 'U' ; 
   else if(IsLower(c)) return 'L' ;  
   else if(IsNumber(c)) return 'N' ;  
   else if(IsOther(c)) return 'O' ;  
   else return '?' ; 
}



void SASCII::DumpAllowed()
{
   Dump(ALLOWED) ; 
}

void SASCII::Dump(const char* s)
{
    char* p = strdup(s) ; 
    while(*p)
    {
       int i = *p ; 
       std::cout 
           << std::setw(5) << *p 
           << std::setw(5) << i 
           << std::setw(5) << Classify(*p) 
           << std::endl ; 
       p++;  
    }
}


SASCII::SASCII(const char* s_)
    :
    s(strdup(s_)),
    len(strlen(s)),
    upper(0),
    lower(0),
    number(0),
    other(0),
    allowed(0)
{
    init(); 
}

void SASCII::init()
{
    for(unsigned i=0 ; i < len ; i++)
    {
        char c = s[i]; 
        if(IsUpper(c)) upper++ ; 
        if(IsLower(c)) lower++ ; 
        if(IsNumber(c)) number++ ; 
        if(IsOther(c)) other++ ; 
        if(IsAllowed(c)) allowed++ ; 
    }     
    assert( len == allowed ); 
}


std::string SASCII::getFirst(unsigned n) const 
{
    std::stringstream ss ; 
    unsigned num(0); 
    for(unsigned i=0 ; i < len ; i++)
    {
        char c = s[i]; 
        if(num < n)
        {
            ss << c ;
            num++ ;   
        } 
    }
    return ss.str();  
}

std::string SASCII::getFirstUpper(unsigned n) const 
{
    std::stringstream ss ; 
    unsigned num(0); 
    for(unsigned i=0 ; i < len ; i++)
    {
        char c = s[i]; 
        if(IsUpper(c) && num < n)
        {
            ss << c ;
            num++ ;   
        } 
    }
    return ss.str();  
}

std::string SASCII::getFirstLast() const 
{
    std::stringstream ss ; 
    ss << s[0] ; 
    ss << s[len-1] ; 
    return ss.str();  
}

std::string SASCII::getTwoChar(unsigned first, unsigned second) const 
{
    std::stringstream ss ; 
    for(unsigned i=0 ; i < len ; i++)
    {
        char c = s[i]; 
        if( i == first ) ss << c ;
        if( i == second ) ss << c ; 
    }
    return ss.str();  
}


