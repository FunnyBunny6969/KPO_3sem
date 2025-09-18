#include "Parm.h"
#include <iostream>
#include <cwchar>
#include "Error.h"

using namespace std;



namespace Parm {

    enum PARM_ERROR {
        ERR_IN_MISSING = 100,
        ERR_TOO_LONG = 104
    };

    PARM getparm(int argc, _TCHAR* argv[]) {
        PARM parm = {}; 

        for (int i = 1; i < argc; i++) 
            if (wcslen(argv[i]) >= PARM_MAX_SIZE) 
                throw ERROR_THROW(ERR_TOO_LONG);

        if (argc < 2) {
            throw ERROR_THROW(ERR_IN_MISSING);
        }
        else {
            for (int i = 1; i < argc; i++) {
                if (wcsstr(argv[i], PARM_IN) == argv[i]) 
                    wcscpy_s(parm.in, PARM_MAX_SIZE, argv[i] + wcslen(PARM_IN));
                else if (wcsstr(argv[i], PARM_OUT) == argv[i])
                    wcscpy_s(parm.out, PARM_MAX_SIZE, argv[i] + wcslen(PARM_OUT));
                else if (wcsstr(argv[i], PARM_LOG) == argv[i])
                    wcscpy_s(parm.log, PARM_MAX_SIZE, argv[i] + wcslen(PARM_LOG));
            }

			if (wcslen(parm.in) == 0) throw ERROR_THROW(ERR_IN_MISSING);
            if (wcslen(parm.out) == 0) {
                wcscpy_s(parm.out, PARM_MAX_SIZE, parm.in);
                wcsncat_s(parm.out, PARM_MAX_SIZE, PARM_OUT_DEFAULT_EXT, wcslen(PARM_OUT_DEFAULT_EXT));
            }
            if (wcslen(parm.log) == 0) {
                wcscpy_s(parm.log, PARM_MAX_SIZE, parm.in);
                wcsncat_s(parm.log, PARM_MAX_SIZE, PARM_LOG_DEFAULT_EXT, wcslen(PARM_LOG_DEFAULT_EXT));
            }
        }

        return parm;
    }

}

