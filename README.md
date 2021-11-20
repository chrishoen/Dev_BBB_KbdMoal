## Dev_BBB_KbdModal
beagle modal keyboard program, c++, vstudio, open folder, cmake
## Description
This implements a modal keyboard filter on a beaglebone black.
The bbb usb host port is connected to a keyboard.
The bbb usb device port is connected to a host computer usb host port.
Keyboard inputs are read from the bbb usb host port, filtered, and then 
written to the bbb usb device port.

``` markdown
Host Computer <- BBB <- Keyboard
```

The keyboard is then presented to the host computer where keyboard inputs
are transformed according to a mode that is determined by the capslock
key before they are sent to the host computer.

If the capslock key is not pressed then the mode is NORMAL and
keycodes are sent to the host computer unchanged. If the capslock
key is pressed then the mode is SPECIAL and the keycodes are transformed
according to the following keymap before they are sent to the host computer.

## KeyMap
``` markdown
   A: Ctrl Gui LeftArrow
   S: Ctrl Gui RightArrow
   X: CapsLock

   D: Ctrl
   Space: Shift

   W: Ctrl X
   E: Ctrl C
   R: Ctrl V

   Y: Esc
   U: UpArrow
   I: Delete
   O: Insert
   P: Dash

   H: Home
   J: LeftArrow
   K: RightArrow
   L: End

   N: Backspace
   M: DownArrow
   Comma: PageUp;
   Period: PageDown
```

## kbdmodal
kbdmodal is a c++ program that interfaces to the keyboard via the hidraw0 device and
interfaces to the host computer via the linux kernel usb gadget interface. It implements
the modal keycode transformation.


