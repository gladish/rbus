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
#include <unistd.h>

void event_handler(rbusHandle_t rbus, rbusEvent_t const* event, rbusEventSubscription_t* sub)
{
  (void) rbus;
  (void) sub;

  fprintf(stdout, "Event\n");
  fprintf(stdout, "  name:%s\n", event->name);
  fprintf(stdout, "  type:%d\n", event->type);
  fprintf(stdout, "  data:\n");

  rbusObject_fwrite(event->data, 5, stdout);
  fprintf(stdout, "\n\n");
}


int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;
  
  rbusHandle_t rbus;
  rbus_open(&rbus, "event-consumer");

  rbusEventSubscriptionEx_t sub;
  sub.eventName = "Device.Provider1.Param1";
  sub.filter = NULL;
  sub.interval = 0;
  sub.duration = 0;
  sub.handler = &event_handler;
  sub.userData = NULL;
  sub.handle = NULL;
  sub.asyncHandler = NULL;
  sub.size = sizeof(rbusEventSubscriptionEx_t);
  sub.fetchFirst = true;

  rbusEvent_SubscribeEx2(rbus, &sub, 1, 1000);

  while (1)
    sleep(5);

  rbus_close(rbus);

  return 0;
}
