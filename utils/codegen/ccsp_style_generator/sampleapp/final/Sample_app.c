/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * 
 * code generated with: python ../../scripts/rbus_code_generator_ccsp_style.py ../Sample_dm.xml Sample
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include "Sample_rbus.h"

int shutdown = 0;

void sigIntHandler(int sig)
{
    shutdown = 1;
    signal(SIGINT, SIG_DFL);
}

int main()
{
    rbusError_t err = RBUS_ERROR_SUCCESS;
    err = Sample_Init();
    if(err != RBUS_ERROR_SUCCESS)
        return err;
    signal(SIGINT, sigIntHandler);
    while(!shutdown)
        usleep(100000);
    err = Sample_Unload();
    return 0;
}
