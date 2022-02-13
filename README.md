# Keylock on arduino

1. Make simple arduino keylock

   1. Support of writing code by keypad
   2. By using keypad possible to delete wrong typed chars
   3. One button could reset all written chars
   4. Subbmit button
   5. Sleeping mode
   6. For back from sleep mode need to press any key once. On lcd nothing typed
   7. In code button wainting press by if condition
   8. LED is gonna be locker, just for tracking code works properly
   9. NFC support
   10. Add punch sensr(idk name of this sensor actually)

2. Advance Level
   1. Android app:
      1. By using nfc or localport WI-FI connecting to arduino
      2. In setting of locker possible to change:
         - password
         - time until sleep
         - Able to open locker by NFC
      3. Arduino sending messege to App if punch sensor is active
      4. App must to have same password as locker on enetering part
      5. And of course by using app you could to open locker
