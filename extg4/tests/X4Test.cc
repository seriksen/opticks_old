// TEST=X4Test om-t

#include <string>
#include "X4.hh"
#include "X4Named.hh"
#include "OPTICKS_LOG.hh"

void test_Name()
{
    std::string name = "/dd/material/Water" ; 

    LOG(info) 
        << std::endl 
        << " name      : " << name   << std::endl   
        << " Name      : " << X4::Name(name)  << std::endl    
        << " ShortName : " << X4::ShortName(name) << std::endl     
        << " BaseName  : " << X4::BaseName(name)     
        ;
}


void test_GDMLName()
{
    const char* n = "/hello/cruel/world" ; 
    X4Named* xn = new X4Named(n) ; 

    const char* gn = X4::GDMLName(xn) ; 

    LOG(info) 
        << " n  " << n << std::endl  
        << " gn " << gn << std::endl  
        ;
}

void test_ptr_format_0()
{
    const char* n = "/hello/cruel/world" ; 
    X4Named* xn = new X4Named(n); 

    std::stringstream ss ; 
    const char* name = "world" ; 
    ss << name ;
    ss << xn ; 
    LOG(info) << ss.str() ; 
}

void test_ptr_format_1()
{
    const char* n = "/hello/cruel/world" ; 
    X4Named* xn = new X4Named(n) ; 

    std::stringstream ss ; 
    const char* name = "world" ; 
    ss << name ;
    ss << static_cast<void*>(xn) ; 
    LOG(info) << ss.str() ; 
}
void test_ptr_format_2()
{
    X4Named xn("hello") ; 
    std::stringstream ss ; 
    ss << xn.name ;
    ss << static_cast<void*>(&xn) ; 
    LOG(info) << ss.str() ; 
}


int main(int argc, char** argv)
{   
    OPTICKS_LOG(argc, argv);

    //test_Name();
    test_GDMLName();

    test_ptr_format_0();
    test_ptr_format_1();
    test_ptr_format_2();


    return 0 ; 
}


