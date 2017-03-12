#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <time.h>
#include <syslog.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <mosquitto.h>

#define I2C_ADDR_CONTROLLER 0x21
#define I2C_ADDR_MANUAL     0x22

const char* MQTT_HOST   = "platon";
const int   MQTT_PORT   = 1883;
const char* MQTT_TOPIC  = "Netz39/Things/Shuttercontrol/Button/Events";

#define MQTT_MSG_MAXLEN           16
const char* MQTT_MSG_BTNPRESS   = "button pressed";
const char* MQTT_MSG_NONE       = "none";

/**
 * Get the milliseconds since epoch.
 */
long current_millis() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  
  return tv.tv_sec*1000L + tv.tv_usec/1000L;
}

///// I2C stuff /////

/**
  * This record contains the I2C file descriptors we use in this program
  */
struct I2C_descriptors {
  int controller;
  int manual;
} I2C_fd;

#define I2C_FD_CONTROLLER (I2C_fd.controller)
#define I2C_FD_MANUAL     (I2C_fd.manual)

/**
  * Initialize an I2C channel to the specified address. Exits with an error
  * message if the initialization fails,
  *
  * @param addr The target address.
  * @return File Descriptor for the I2C channel
  */
int I2C_setup_fd(const int addr) {
  const int fd = wiringPiI2CSetup(addr);
  if (!fd) {
    syslog(LOG_EMERG, "Error %d on I2C initialization!", errno);
    exit(-1);
  }
  return fd;
}

/**
  * Initialize all I2C channels and store the file descriptors.
  */
void I2C_init(void) {
  I2C_fd.controller = I2C_setup_fd(I2C_ADDR_CONTROLLER);
  I2C_fd.manual     = I2C_setup_fd(I2C_ADDR_MANUAL);
}

#define I2C_ERR_INVALIDARGUMENT -2

  union I2C_result {
    unsigned char c[2];
    unsigned short r;
  };


int I2C_command(const int fd, const char command, const char data) {
  // check parameter range
  if ((command < 0) || (command > 0x07))
    return I2C_ERR_INVALIDARGUMENT;
  if ((data < 0) || (data > 0x0f))
    return I2C_ERR_INVALIDARGUMENT;
  
  // TODO check fd
  
    
  // build the I2C data byte
  // arguments have been checked, 
  // this cannot be negative or more than 8 bits
  unsigned char send = (command << 4) + data; 
  
  // calculate the parity
  char v = send;
  char c;
  for (c = 0; v; c++) 
    v &= v-1;
  c &= 1;

  // set parity bit  
  send += (c << 7);
  
  union I2C_result result;
  result.r = 0;

  // maximal number of tries
  int hops=20;

  // try for hops times until the result is not zero
  while (!result.c[0] && --hops) {
    // send command
    result.r = wiringPiI2CReadReg16(fd, send);

    // check for transmission errors: 2nd byte is inverted 1st byte
    const unsigned char c = ~result.c[0];
    if (result.c[1] != c) 
      // if no match, reset the result
      result.r = 0;
  }
  
  if (!hops)
    syslog(LOG_DEBUG, "Giving up transmission!\n");
  
  return result.c[0];
}

///// I3C stuff /////

void I3C_reset_manual() {
  I2C_command(I2C_FD_MANUAL, 0x4, 0x0);
}

///// Manual Controll unit /////

#define SWITCH_UP      1
#define SWITCH_NEUTRAL 3
#define SWITCH_DOWN    2
#define SWITCH_LOCKED  0

#define SWITCH_ERR             -1
#define SWITCH_ERR_OUTOFBOUNDS -2

/**
  * Read the state of the specified switch.
  * @return 
  *	Switch state according to SWITCH_XXX, SWITCH_ERR if an error
  *	occurred.
  */
char read_switch_state(const char idx) {
  // check parameter range
  if ((idx < 1) || (idx > 4))
    return SWITCH_ERR_OUTOFBOUNDS;

  // send the command    
  const char state = I2C_command(I2C_FD_MANUAL, 0x3, idx);
  
  // return result
  return state;  
}

/**
  * Beep in the provided pattern.
  * @param The beep pattern. Only the last 4 Bits are evaluated!
  */
void beep(const char pattern) {
  I2C_command(I2C_FD_MANUAL, 0x1, pattern&0xf);
}

#define LED_PATTERN_OFF  0x00
#define LED_PATTERN_SLOW 0x01
#define LED_PATTERN_FAST 0x02
#define LED_PATTERN_ON   0x03

/**
  * Set manual mode LED to the specified blink pattern.
  * @param pattern The blink pattern; one of LED_PATTERN_XXX.
  */
void set_manual_mode_led(const char pattern) {
  I2C_command(I2C_FD_MANUAL, 0x2, pattern);
}

char get_manual_mode() {
  return I2C_command(I2C_FD_MANUAL, 0x5, 0);
}

#define MANUAL_MODE_ON  1
#define MANUAL_MODE_OFF 2

void set_manual_mode(const char mode) {
  I2C_command(I2C_FD_MANUAL, 0x5, mode);
}

///// Shutter Control unit /////

#define SHUTTER_ERR             -1
#define SHUTTER_ERR_OUTOFBOUNDS -2

#define SHUTTER_OFF  0
#define SHUTTER_UP   1
#define SHUTTER_DOWN 2

/**
  * Set the shutter control state.
  * @param idx Number of the shutter, between 1 and 4
  * @param state One of SHUTTER_XXX.
  * @return 0 if everything is okay, otherwise one of SHUTTER_ERR_XXX
  */
char set_shutter_state(const char idx, const char state) {
  // check parameter range
  if ((idx < 1) || (idx > 4))
    return SHUTTER_ERR_OUTOFBOUNDS;
  if ((state < 0) || (state > 2))
    return SHUTTER_ERR_OUTOFBOUNDS;  

  // determine the command
  char command = 0x0;
  switch (state) {
    case SHUTTER_OFF:  command = 0x1; break;
    case SHUTTER_UP:   command = 0x2; break;
    case SHUTTER_DOWN: command = 0x3; break;
    default: {
      syslog(LOG_EMERG, "set_shutter_state: Unknown shutter state: %d.");
      syslog(LOG_EMERG, "This cannot happen! Aborting with assertion error.");
      exit(-1);
    }
  }

  // send the command    
  I2C_command(I2C_FD_CONTROLLER, command, idx-1);

  // return OK
  return 0;
}

/**
  * Stop all the shutters!
  */
void stop_all_shutters() {
  I2C_command(I2C_FD_CONTROLLER, 0x0, 0x0);
}


char switch_state[4];
long switch_lastchange[4];

/**
  * Set stored switch states to NEUTRAL.
  */
void clear_stored_switch_state() {
  const long t = current_millis();
  int i;
  for (i=0; i < 4; i++) {
    switch_state[i] = SWITCH_NEUTRAL;
    switch_lastchange[i] = t;
  }
}

/**
  * Store a new switch state.
  * Return old state if there was a change.
  * @return the old state or 0 if there was no change
  */
char store_switch_state(const char idx, const char state) {
  const char old_state = switch_state[idx-1];
  
  if (old_state == state)
    return 0;

  switch_lastchange[idx-1] = current_millis();  
    
  switch_state[idx-1] = state;
  return old_state;
}

/**
  * Adjust the switch state in state storage and controller,
  * but only if there was a change.
  */
void adjust_switch_state(const char idx, const char state) {
  // time since last change
  const long delay = 2*1000;
  const long rundelay = 60*1000;
  const long lastchange = current_millis() - switch_lastchange[idx-1];

  // only if rundelay not exceeded; 
  // after a while the shutter will be unlocked no matter what
  if (lastchange < rundelay) {
  
    // ignore neutral position for locked switches
    if ((state == SWITCH_NEUTRAL) &&
        (lastchange > delay))
        return;

    // lock switch if it is hold for longer than 2 secs
    if ((state != SWITCH_NEUTRAL) &&
        (switch_state[idx-1] == state) &&
        (lastchange > delay)) {
        syslog(LOG_NOTICE, "Locking %d.\n", idx);
        beep(0x1);
        return;
    }

  }
            
  // store new state and check if a change occured
  const char st = store_switch_state(idx, state);

  if (st) {
    // if locked, just turn off
    if (lastchange > delay) {
      syslog(LOG_NOTICE, "Shutting %d off.\n", idx);
      set_shutter_state(idx, SHUTTER_OFF);
      store_switch_state(idx, SWITCH_NEUTRAL);
    } else {
      syslog(LOG_NOTICE, "Changing switch state for %d to %d.\n", idx, state);
    
      // commit the action only if the state has changed.
      switch (state) {
        case SWITCH_NEUTRAL: set_shutter_state(idx, SHUTTER_OFF); break;
        case SWITCH_UP: set_shutter_state(idx, SHUTTER_UP); break;
        case SWITCH_DOWN: set_shutter_state(idx, SHUTTER_DOWN); break;
      }
    } // if-else  
  }
}


int main(int argc, char *argv[]) {
  // initialize the system logging
  openlog("shuttercontrol", LOG_CONS | LOG_PID, LOG_USER);
  syslog(LOG_INFO, "Starting shuttercontrol.");

  I2C_init();
  stop_all_shutters();
  clear_stored_switch_state();
  beep(0x05);
  set_manual_mode_led(LED_PATTERN_FAST);
  sleep(1);
  set_manual_mode_led(LED_PATTERN_OFF);
 
  // initialize MQTT   
  mosquitto_lib_init();
  
  void *mqtt_obj;
  struct mosquitto *mosq;
  mosq = mosquitto_new("shuttercontrol", true, mqtt_obj);
  if ((int)mosq == ENOMEM) {
    syslog(LOG_ERR, "Not enough memory to create a new mosquitto session.");
    mosq = NULL;
  }
  if ((int)mosq == EINVAL) {
    syslog(LOG_ERR, "Invalid values for creating mosquitto session.");
    return -1;
  }

  if (mosq) {
    int ret; 
    
    ret = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 30);
    if (ret == MOSQ_ERR_SUCCESS)
      syslog(LOG_INFO, "MQTT connection to %s established.", MQTT_HOST);
      
    // TODO error handling
  }
   
  char mqtt_payload[MQTT_MSG_MAXLEN];


  // Store manual mode; start with read-out
  char old_manual = get_manual_mode();
  
  char run=1;
  int i=0;
  while(run) {
    printf("****** %u\n", i++);

    const char manual = get_manual_mode();
    printf("Manual mode: %s\n", (manual==MANUAL_MODE_ON)?"on":"off");
    
    if (manual == MANUAL_MODE_ON)
      set_manual_mode_led(LED_PATTERN_ON);
    else
      set_manual_mode_led(LED_PATTERN_OFF);
   
    // reset MQTT payload
    mqtt_payload[0] = 0;

    // if manual mode changed, send MQTT event
    if (manual != old_manual) {
      // prepare MQTT payload
      strcpy(mqtt_payload, MQTT_MSG_BTNPRESS);
    
      // store
      old_manual = manual;
    }
 
    // send MQTT message if there is payload
    if (mqtt_payload[0] && mosq) {
      int ret;
      int mid;
      ret = mosquitto_publish(
                        mosq, 
                        &mid, 
                        MQTT_TOPIC,
                        strlen(mqtt_payload), mqtt_payload,
                        2, /* qos */
                        false /* do not retain the event */
                       );
      if (ret != MOSQ_ERR_SUCCESS)
        syslog(LOG_ERR, "MQTT error on message \"%s\": %d (%s)",
                        mqtt_payload, 
                        ret,
                        mosquitto_strerror(ret));
      else
        syslog(LOG_INFO, "MQTT message \"%s\" sent with id %d.",
                         mqtt_payload, mid);
    }

    
    int idx;
    for (idx=1; idx<5; idx++) {
      const char sw = read_switch_state(idx);
      printf("Switch %d status: %d\n", idx, sw);

      adjust_switch_state(idx, sw);      
    }

    I3C_reset_manual();

    // call the mosquitto loop to process messages
    if (mosq) {
      int ret; 
      ret = mosquitto_loop(mosq, 100, 1);
      // if failed, try to reconnect
      if (ret)
        mosquitto_reconnect(mosq);
    }

    if (sleep(1)) 
      break;
  }

  stop_all_shutters();

  // clean-up MQTT
  if (mosq) {
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);   
  }
  mosquitto_lib_cleanup();


  syslog(LOG_INFO, "Shuttercontrol finished.");
  closelog();
    
  return 0;
}
