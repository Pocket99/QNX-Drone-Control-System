//========================================================================
// cjoystick.hpp : XFX Game Controller HID Joystick Interface
// Version 1.0
//
// Author : Vilas Kumar Chitrakaran <cvilas@ces.clemson.edu>
// Author : Vilas Kumar Chitrakaran <cvilas@ces.clemson.edu>
// Modified by : Iman Saboori
// Date : June 2011
//========================================================================
#ifndef _CJOYSTICK_HPP_INCLUDED
#define _CJOYSTICK_HPP_INCLUDED
#include <sys/hiddi.h>
//========================================================================
/*! \struct joystick_data
 \brief A structure for holding joystick data */
//========================================================================
typedef struct joystick_data
{
	bool status_flag; //!< 'true' if device status is OK.
	char status_msg_buff[80]; //!< Char buffer to hold device status message.
	int x;
	int y;
	int z;
	int rx;
	int ry;
	int rz;
}joystick_data_t;
//========================================================================
/*! \struct joystick_report
 \brief A structure for device reports (for driver internal use) */
//========================================================================
typedef struct joystick_report
{
	struct hidd_report_instance *creport_instance;
	struct hidd_report *creport;
	_uint16 *cbtnbuf; // current button buffer
}joystick_report_t;

//========================================================================
/*! \struct joystick_device
 \brief A structure for joystick devices (for driver internal use) */
//========================================================================
typedef struct joystick_device
{
	joystick_data_t data; //!< device current data.
	joystick_report_t *report;
	hidd_device_instance_t *device_instance;
}joystick_device_t;


//========================================================================
class cjoystick
{
public:
	 cjoystick();
	 // The default constructor. Establishes connection with the HID driver.
	 // devNum The address of the joystick device to connect to (default = 0).
	
	 ~cjoystick();
	 // The default destructor disconnects from the HID driver and cleans up.
	 double getX() const;
	 // return The X position
	
	 double getY() const;
	 // return The Y position
	 double getZ() const;
	 // return The Y position
	 double getRX() const;
	 // return The Y position
	 double getRY() const;
	 // return The Y position
	 double getRZ() const;
	 // return The Y position
	 bool is_status_ok() const;
	 // return true if no error, else false.
	
	 char *get_status_msg() const;
	 // return A string carrying current status of the device. This string
	 // also carries error messages when is_status_ok() returns false.
	
	 void print_device_info(int verbosity = 1) const;
	 // Print information about the device to stdout. Higher values of
	 // \a verbosity mean more detailed information.

protected:
	 joystick_data_t get_joystick_data() const;
	 // return The whole data structure for device
	 // with current joystick data
private:
	 struct hidd_connection *d_connection;
	 // connection handle to HID server
	 // ----- HID server connection functions ------
	 static joystick_device_t s_joystick;
	 static void on_insertion(struct hidd_connection *connection, hidd_device_instance_t *instance);
	 static void on_removal(struct hidd_connection *connection, hidd_device_instance_t *instance);
	 static void on_hid_report(struct hidd_connection *connection, struct hidd_report *handle,
	 void *report_data, _uint32 report_len, _uint32 flags, void *user);
};
#endif // #ifndef _CJOYSTICK_HPP_INCLUDED