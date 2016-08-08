#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <time.h>
#include <syslog.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#define I2C_ADDR_DOORCTRL   0x23

struct door_status_t {
  bool green_active;	// Green Button active
  bool red_active; 	// Red Button active
  bool door_closed;	// Door is closed
  bool lock_open;	// Lock is open
  bool force_close;	// Forcing the door closed (cmd)
  bool force_open;	// Forcing the door open (cmd)
};

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
  int doorctrl;
} I2C_fd;

#define I2C_FD_DOORCTRL (I2C_fd.doorctrl)

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
  I2C_fd.doorctrl = I2C_setup_fd(I2C_ADDR_DOORCTRL);
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

#define DOORCTRL_CMD_RESET	0x00
#define DOORCTRL_CMD_OPEN	0x01
#define DOORCTRL_CMD_CLOSE	0x02
#define DOORCTRL_CMD_STATE	0x03

void I3C_reset_doorctrl() {
  I2C_command(I2C_FD_DOORCTRL, DOORCTRL_CMD_RESET, 0x0);
}

///// Door Controller /////

char doorctrl_read_status() {
  // send the command    
  const char state = I2C_command(I2C_FD_DOORCTRL,
                                 DOORCTRL_CMD_STATE, 0);
  
  // return result
  return state;  
}

void decode_door_status(uint8_t status,
                        struct door_status_t *ds)
{
  ds->green_active = (status & 0x20);
  ds->red_active   = (status & 0x10);
  ds->door_closed  = (status & 0x08);
  ds->lock_open    = (status & 0x04);
  ds->force_close  = (status & 0x02);
  ds->force_open   = (status & 0x01);
}

int main(int argc, char *argv[]) {
  // initialize the system logging
  openlog("doorstate", LOG_CONS | LOG_PID, LOG_USER);
  syslog(LOG_INFO, "Starting doorstate observer.");

  // initialize I2C
  I2C_init();
  
  // TODO initialize MQTT

  // the known door status
  struct door_status_t before; 
  decode_door_status(doorctrl_read_status(), &before);
  
  char run=1;
  int i=0;
  while(run) {
    printf("****** %u\n", i++);

    uint8_t status = doorctrl_read_status();    
    struct door_status_t ds;
    decode_door_status(status, &ds);
    
    printf("Door status byte: 0x%02x\n", status);
    printf("Green active:\t%s\n", (ds.green_active ? "yes" : "no"));
    printf("Red active:\t%s\n", (ds.red_active ? "yes" : "no"));
    printf("Door closed:\t%s\n", (ds.door_closed ? "yes" : "no"));
    printf("Lock open:\t%s\n", (ds.lock_open ? "yes" : "no"));
    printf("Force closed:\t%s\n", (ds.force_close ? "yes" : "no"));
    printf("Force open:\t%s\n", (ds.force_open ? "yes" : "no"));
    printf("\n");

    // Check door status for changes and emit MQTT messages

    // door status changed
    if (before.door_closed != ds.door_closed) {
      if (ds.door_closed) {
        syslog(LOG_INFO, "Door has been closed.");
        //TODO MQTT message
      } else {
        syslog(LOG_INFO, "Door has been opened.");
        //TODO MQTT message
      }
      
      before.door_closed = ds.door_closed;
    }

    // lock status changed
    if (before.lock_open != ds.lock_open) {
      if (ds.lock_open) {
        syslog(LOG_INFO, "Door has been unlocked.");
        //TODO MQTT message
      } else {
        syslog(LOG_INFO, "Door has been locked.");
        //TODO MQTT message      
      }
      
      before.lock_open = ds.lock_open;
    }
    

    I3C_reset_doorctrl();
    
    if (sleep(1)) 
      break;
  }

  syslog(LOG_INFO, "Doorstate observer finished.");
  closelog();
    
  return 0;
}
