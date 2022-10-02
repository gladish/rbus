/*
 * If not stated otherwise in this file or this component's Licenses.txt file
 * the following copyright and licenses apply:
 *
 * Copyright 2019 RDK Management
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

#include <rbus.h>
#include <string.h>
#include <unistd.h>


rbusError_t get_handler(rbusHandle_t rbus, rbusProperty_t prop, rbusGetHandlerOptions_t* opts)
{
  (void) rbus;

  printf("GET %s from %s\n", rbusProperty_GetName(prop), opts->requestingComponent);

  if (!strcmp(rbusProperty_GetName(prop), "Device.Provider1.Param1"))
  {
    static int n = 100;

    //
    // internally rbus is querying this parameter to check for value-change.
    // we can trigger that by making rbusValue_Compare return non-zero
    //
    // the internall polling can be overridden/turned off. Whent this is done,
    // the event provider MUST publish this event on its own. In order to override
    // the internal polling, you use the event subscritption in regDataElement
    // see below...
    //
    char buff[16];
    snprintf(buff, sizeof(buff), "number-%d", n++);

    rbusValue_t value;
    rbusValue_Init(&value);
    rbusValue_SetString(value, buff);
    rbusProperty_SetValue(prop, value);
    rbusValue_Release(value);
  }

  return RBUS_ERROR_SUCCESS;
}

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;

  rbusHandle_t rbus;
  rbus_open(&rbus, "event-provider");

  // 
  // register object. In this configuration, rbus will periodically
  // poll for the value of "Device.Provider1.Param1". If the value has changed,
  // rbus will automatically publish a value changed event with the new and old
  // value.
  // The polling is only done if a client is subscribed.
  // In order to take control of the event changed publication, the app should
  // supply a callback for the event subscription handler. This callback will get fired
  // when a client subscribes/connects. The application can then return "autoPublish"
  // as false.
  //
  // typedef rbusError_t (*rbusEventSubHandler_t)(rbusHandle_t handle, rbusEventSubAction_t action,
  //     char const* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);
  //
  //
  rbusDataElement_t dataElements[1] = {
    {
      "Device.Provider1.Param1", RBUS_ELEMENT_TYPE_PROPERTY, {
      get_handler,
      NULL, /* set handler */
      NULL, /* table add row */
      NULL, /* table remove row */
      NULL, /* event subscription handler */
      NULL  /* method invoke handler */
    }},
  };

  rbus_regDataElements(rbus, 1, dataElements);

  // XXX: wasted thread
  while (1)
    sleep(5);

  rbus_close(rbus);

  return 0;
}
