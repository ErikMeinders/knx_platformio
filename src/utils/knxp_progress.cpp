#include "utils/knxp_progress.h"

void progress(int nr, const char *text)
{
    Log.trace(">> Startup step [%d]: %s\n", nr, text);
}
