#pragma once

#include <map>
#include <string>

/*
NB the codes are all divide into 6... they are the line numbers into the texture 

delta:g4_00.96ff965744a2f6b78c24e33c80d3a4cd.dae blyth$ cat GBoundaryLibMetadataMaterialMap.json
{
    "ADTableStainlessSteel": "336",
    "Acrylic": "84",
    "Air": "12",
    "Aluminium": "24",
    "Bialkali": "126",
    "DeadWater": "42",
    "ESR": "162",
    "Foam": "30",
    "GdDopedLS": "102",

*/

class Lookup {
   public:  

   typedef std::map<unsigned int, unsigned int> Lookup_t ;
   typedef std::map<std::string, unsigned int> Map_t ;

   static const char* ANAME ; 
   static const char* BNAME ; 

   public:  
       // setup
       Lookup();
       void create(const char* dir);  
       // loads maps and creates lookups from A->B and B->A 
       // loada from parent of dir : ANAME ChromaMaterialMap.json               (/dd/Materials/shortname : code) 
       // loadb from dir           ; BNAME GBoundaryLibMetadataMaterialMap.json              ( shortname : code )

   private:
       void loada(const char* adir, const char* aname=ANAME, const char* aprefix="/dd/Materials/");
       void loadb(const char* bdir, const char* bname=BNAME, const char* bprefix="");

   public:  
       // use
       int a2b(unsigned int a);
       int b2a(unsigned int b);
       void dump(const char* msg);
       std::string acode2name(unsigned int acode);
       std::string bcode2name(unsigned int bcode);

   private:  
       //Map_t parseTree(boost::property_tree::ptree& tree, const char* prefix) ;
       void dumpMap(const char* msg, Map_t& map);

       std::map<unsigned int, unsigned int> _create(const char* apath, const char* aprefix, const char* bpath, const char* bprefix);
       std::map<unsigned int, unsigned int> _create(Map_t& a, Map_t&b);
       std::string find(Map_t& m, unsigned int code);

       int lookup(Lookup_t& lkup, unsigned int x); 

   private:
        Map_t m_a ; 
        Map_t m_b ; 
        Lookup_t m_a2b ; 
        Lookup_t m_b2a ; 

        std::string m_apath ; 
        std::string m_bpath ; 
 
};



