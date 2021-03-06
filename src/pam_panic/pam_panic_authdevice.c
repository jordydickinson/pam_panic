/*
FILENAME :     pam_panic_authdevice.c
DESCRIPTION :  Authenticates against the removable media
AUTHOR :       Bandie
DATE :         2018-03-27T02:34:08+02:00
LICENSE :      GNU-GPLv3
*/

#include <stdint.h>
#include <unistd.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <syslog.h>
#include "pam_panic_reject.h"

#include "pam_panic_authdevice.h"

int authDevice(pam_handle_t *pamh, char *allowed, char *rejected, char *serious_dev, int8_t bSerious, int8_t bReboot, int8_t bPoweroff){

  int8_t counter = 0;
  while(access(allowed, F_OK) == -1 && access(rejected, F_OK) == -1){
    pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, NULL, ASK);
    if(++counter >= 3){
      pam_syslog(pamh, LOG_NOTICE, "Couldn't identify any keys. 3 tries.");
      return (PAM_MAXTRIES);
    }
  }

  if(access(allowed, F_OK) != -1)
    return (PAM_SUCCESS);
  if(access(rejected, F_OK) != -1)
    return reject(serious_dev, bSerious, bReboot, bPoweroff);

  return (PAM_MAXTRIES);

}
