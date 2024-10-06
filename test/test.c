#include "writelog.h"
#include "wutil.h"

int main(int argc, char const *argv[])
{
    init_wlog( LOG_DEBUG, (KBYTE * KBYTE));
    create_logfile("/home/testdir/","testlog.log");
    
    int i = 0;

    while (i < 10)
    {
        wlog_info("hello~ \n");
        wlog_debug("hi ~ \n");
        wlog_err("bye~ \n");
        wlog_warn("bi \n\n");

        n_sleep(1,0);

        i++;
    }

    n_sleep(1,0);

    destroy_wlog();
    
    return 0;
}
