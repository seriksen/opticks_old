
// https://stackoverflow.com/questions/1908687/how-to-redirect-the-output-back-to-the-screen-after-freopenout-txt-a-stdo

#include <cassert>
#include "S_freopen_redirect.hh"
#include "SSys.hh"



int main(void)
{
    const char* path = "/tmp/S_freopen_redirect_test.log" ;
    SSys::Dump(path); 
    {
       S_freopen_redirect fr(stdout, path) ; 
       SSys::Dump(path); 
    }
    SSys::Dump(path); 


    int rc = SSys::run("tboolean.py --tag 1 --tagoffset 0 --det tboolean-box --src torch ");
    assert( rc == 0 );


    return 0;
}


/*

::

    double OContext::launch_redirected_(unsigned entry, unsigned width, unsigned height)
    {
        assert( m_llogpath ) ; 

        S_freopen_redirect sfr(stdout, m_llogpath );

        double dt = launch_( entry, width, height ) ;

        return dt ;                              
    }



OContext::launch_redirected_ succeeds to write kernel rtPrintf 
logging to file BUT a subsequent same process "system" invokation 
of python has problems indicating that cleanup is not complete
::

    Traceback (most recent call last):
      File "/Users/blyth/opticks/ana/tboolean.py", line 62, in <module>
        print ab
    IOError: [Errno 9] Bad file descriptor
    2017-12-13 15:33:13.436 INFO  [321569] [SSys::run@50] tboolean.py --tag 1 --tagoffset 0 --det tboolean-box --src torch   rc_raw : 256 rc : 1
    2017-12-13 15:33:13.436 WARN  [321569] [SSys::run@57] SSys::run FAILED with  cmd tboolean.py --tag 1 --tagoffset 0 --det tboolean-box --src torch  
    2017-12-13 15:33:13.436 INFO  [321569] [OpticksAna::run@79] OpticksAna::run anakey tboolean cmdline tboolean.py --tag 1 --tagoffset 0 --det tboolean-box --src torch   rc 1 rcmsg OpticksAna::run non-zero RC from ana script
    2017-12-13 15:33:13.436 FATAL [321569] [Opticks::dumpRC@186]  rc 1 rcmsg : OpticksAna::run non-zero RC from ana script
    2017-12-13 15:33:13.436 INFO  [321569] [SSys::WaitForInput@151] SSys::WaitForInput OpticksAna::run paused : hit RETURN to continue...


This issue is reproduced by this test


*/


