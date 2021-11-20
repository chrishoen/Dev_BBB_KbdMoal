#!/bin/bash

modprobe libcomposite

pushd /sys/kernel/config/usb_gadget  # Put ourselves in the kernel configfs space

mkdir -p kbdmodal_hidkbd
cd kbdmodal_hidkbd

echo 0x0200 > bcdUSB     # Complies with USB specification 2.0.0
echo 0xFFFE > idVendor   # User specified--your company USB number here
echo 0xFFFE > idProduct  # User specified--your product number here
echo 0x0100 > bcdDevice  # v1.0.0


# String descriptors (configure these before you need them)
mkdir -p strings/0x0409  # 0x0409 is the code for English
echo "12345678-1" > strings/0x0409/serialnumber
echo "chrishoen" > strings/0x0409/manufacturer
echo "kbdmodal keyboard interface 1" > strings/0x0409/product

# Configuration descriptor 1
mkdir -p configs/c.1/strings/0x0409
echo "Conf Desc 1" > configs/c.1/strings/0x0409/configuration  # This sets iConfiguration in Configuration Descriptor
echo 500 > configs/c.1/MaxPower  # Request 500 mA

# Careful ... stuff after this line actually calls into the kernel module

mkdir -p functions/hid.usb0		   # Calls *_alloc_inst in the kernel module--we do specific to the class stuff
echo 1 > functions/hid.usb0/protocol	   # Keyboard--specified by USB
echo 1 > functions/hid.usb0/subclass	   # Boot Interface--specified by USB
echo 8 > functions/hid.usb0/report_length  # wMaxPacketSize--set to 8 to enable usage by old low-speed devices

echo -ne '\x5\x1\x9\x6\xa1\x1\x5\x8\x19\x1\x29\x3\x15\x0\x25\x1\x75\x1\x95\x3\x91\x2\x95\x5\x91\x1\x5\x7\x19\xe0\x29\xe7\x95\x8\x81\x2\x75\x8\x95\x1\x81\x1\x19\x0\x2a\xff\x0\x26\xff\x0\x95\x6\x81\x0\xc0' > functions/hid.usb0/report_desc

# Unpacking the report descriptor using the wonderful "USB Descriptor and Request Parser" at https://eleccelerator.com/usbdescreqparser/
# Descriptor shamelessly copied from: "Appendix E: Example USB Descriptors for HID Class Devices"

# 0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
# 0x09, 0x06,        // Usage (Keyboard)
# 0xA1, 0x01,        // Collection (Application)
# 0x05, 0x08,        //   Usage Page (LEDs)
# 0x19, 0x01,        //   Usage Minimum (Num Lock)
# 0x29, 0x03,        //   Usage Maximum (Scroll Lock)
# 0x15, 0x00,        //   Logical Minimum (0)
# 0x25, 0x01,        //   Logical Maximum (1)
# 0x75, 0x01,        //   Report Size (1)
# 0x95, 0x03,        //   Report Count (3)
# 0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
# 0x95, 0x05,        //   Report Count (5)
# 0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
# 0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
# 0x19, 0xE0,        //   Usage Minimum (0xE0)
# 0x29, 0xE7,        //   Usage Maximum (0xE7)
# 0x95, 0x08,        //   Report Count (8)
# 0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
# 0x75, 0x08,        //   Report Size (8)
# 0x95, 0x01,        //   Report Count (1)
# 0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
# 0x19, 0x00,        //   Usage Minimum (0x00)
# 0x2A, 0xFF, 0x00,  //   Usage Maximum (0xFF)
# 0x26, 0xFF, 0x00,  //   Logical Maximum (255)
# 0x95, 0x06,        //   Report Count (6)
# 0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
# 0xC0,              // End Collection

# // 55 bytes


ln -s functions/hid.usb0 configs/c.1/  # calls _alloc in the kernel module

ls /sys/class/udc > UDC  # calls *_bind in the kernel module

# /dev/hidg0 should now exist

# Connecting the USB cable actually calls *_set_alt in the kernel module

# At this point--device is being polled--but responding with NACK since we're not connected to /dev/hidg0

popd
