## History

### 11th October 2019

* Branch v1.2
   * Changing the output to the boiler switch from high to low when switched on. 
   
### 17th October 2019

* Branch v1.2 has been implemented as the boiler switch and is working quite well. 
* Real world testing for about 1 week shows that it is working well, being able to switch the boiler on and off as needed without any signal conflict. However, since the system only has the two nodes, it's possible that I'm simply missing something due to the simplicity of the system.
* Since this branch is working as expected, it will be merged into the master brach and work will start on the v2 branches.

### 17th October 2019

* Branch v2 created to start using the canbus_controller module
* All calls to mcp_driver have been removed and replaced with calls to canbus_controller
* With a few changes to the arduino send program, the canbus_controller module appears to be working as expected.
    * It is currently being tested while working as the boiler switch and I will do further updates to the module while I am waiting.

### v2.2-changing_int_pin_from_mcp2515_away_from_pgd

#### 21 November 2019

TO BE IMPLEMENTED

INT from the mcp2515 controller has been connected to pin 4 on the pic, which is also the PGD programming pin. This has been changed to pin 26 (RP15), which currently isn't conncected to anything.

    This was causing problems where the pic could not be programmed up when pin 4 was connected as it should be.

