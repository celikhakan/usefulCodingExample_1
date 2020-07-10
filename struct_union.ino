/* This simple (ish) sketch shows you how to organize your data into a struct
 * and then access the whole thing as a byte array.
 *
 * Useful if you want to concatenate several variables into a single byte array
 * to send over bluetooth, i2c, lora or any other protocol that works with arrays. 
 * In other words you have a fixed byte array, and we squeeze in variables of different
 * data types and lengths into it, while still using it as a byte array afterwards.
 * 
 * The way we use a union here (in plain terms) is: the union is a fixed region of memory that we
 * can write to in one format (as a structure) and then read from in another format (a byte array).
 * You can do it in reverse as well, write to it as a byte array, and read it as a structure.
 * 
 * Thanks to this thread https://forum.arduino.cc/index.php?topic=271048.0 for helping me figure it out.
 * NOTE: be careful (ab)using structures for cross-platform code, variable sizes in bytes might vary causing havoc !!!!
 */

//first we define our structure data type
//customize this to whatever variable types and names you need
typedef struct settings_t{
  byte stat;
  byte sensorId;
  byte sensortype;
  byte isWet;
  uint16_t temp;
  float volts;
  byte signal;
};

//the packet size will be the number of bytes (1byte = 8bits) of all the variables we defined in the structure above
//in our case it's: 4 bytes (the first 4 variables in the struct) + 2 bytes (the uint16_t is 2*8 bytes) + 4bytes (the float) + 1 byte (the last variable)
#define PACKET_SIZE 11 
//NOTE: the actual sizes of the variables MIGHT differ based on your platform, so be very careful when using this
//for cross-platform code (having it run the same on an ESP32 and an Arduino Nano for example)

/* Now we define a union, basically the ways we want to write or read this data
 * in our case we want one way to be the structure above
 * and another way to be a byte array of appropriate size.
 * I named this 'btPacket_t' because I use it for bluetooth, name it whatever you want.
 * You can define as many types inside as you need, just make sure the types you define are all the same size in bytes
 */
typedef union btPacket_t{
 settings_t structure;
 byte byteArray[sizeof(sensorData_t)]; /* you can use other variable types if you want. Like: a 32bit integer if you have 4 8bit variables in your struct */
};

//now we define it
btPacket_t settings;  

void setup()
{
  Serial.begin(115200);
}


void loop()
{
  //store some data in our union.
  //we're treating it as a structure in this case for easy writing
  //we could also do something like "settings.byteArray[1] = 22;" 
  //you can treat this memory region as any data type we define in our union
  settings.structure.stat = 0; 
  settings.structure.sensorId = 22; 
  settings.structure.sensortype = 0; 
  settings.structure.isWet = 0; 
  settings.structure.temp = 75; 
  settings.structure.volts = 3141 / 1000.0; 
  settings.structure.signal = 88;
  
  //let's see what we stored in it
  //this time we're treating it as a byte array
  //instead of printing it to serial you could send the whole thing over Bluetooth instead for example
  for(int i=0; i<PACKET_SIZE; i++)
  {
    Serial.print(settings.byteArray[i]);
    Serial.print("\t");
  }
  Serial.println();
  
  //you can do whatever you need with the memory location by treating it as a byte array (settings.byteArray)
  //or as a structure (settings.structure)
  
  delay(1000);
}


