#include "writelog.h"

int main(int argc, char const *argv[])
{
    _init_wlog( LOG_DEBUG, (KBYTE * KBYTE * KBYTE));
    _create_log("/home/github/printlog/src/","writelog.log");
    
    int i = 0;
    while(i < 100)
    {
        wlog_info("hello~ \n");
        wlog_debug("hi ~ \n");
        wlog_err("bye~ \n");
        wlog_warn("bi \n\n");
        n_sleep(0,50);
        i++;

        if(i == 50)
            n_sleep(1,50);
    }

    n_sleep(5,0);

    _destroy_wlog();
    return 0;
}
