#include "MuxApi.h"

#include <QLibrary>

MuxApi *MuxApi::instance()
{
    static MuxApi* ret = NULL;
    if (!ret) {
        ret = new MuxApi;
    }

    return ret;
}

bool MuxApi::init(const QString &dllPath)
{
    QLibrary lib(dllPath);
    ptr_create_mux  = (create_mux)lib.resolve("create_mux");
    ptr_start_mux   = (start_mux)lib.resolve("start_mux");
    ptr_stop_mux    = (stop_mux)lib.resolve("stop_mux");
    ptr_destroy_mux = (destroy_mux)lib.resolve("destroy_mux");

    return ptr_create_mux
            && ptr_start_mux
            && ptr_stop_mux
            && ptr_destroy_mux;
}
