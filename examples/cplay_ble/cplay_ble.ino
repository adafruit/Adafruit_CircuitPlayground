/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Adafruit_CircuitPlayground.h>

// BLE Service
BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery

// Adafruit Service: ADAFxx-C332-42A8-93BD-25E905756CB8
BLEAdafruitTemperature  bleTemp;
BLEAdafruitAccel        bleAccel;
BLEAdafruitLightSensor  bleLight;
BLEAdafruitButton       bleButton;

// Each for a service
SoftwareTimer timerTemp;
SoftwareTimer timerAccel;
SoftwareTimer timerLight;
SoftwareTimer timerButton;

// meaure function
void measure_callback(TimerHandle_t xTimerID)
{
  digitalWrite(PIN_LED1, HIGH);
  if ( xTimerID == timerTemp.getHandle() )
  {
    float temp = CircuitPlayground.temperature();
    bleTemp.Temperature.notify(&temp, sizeof(temp));  
  }

  if ( xTimerID == timerAccel.getHandle() )
  {
    float accel[3];
    accel[0] = CircuitPlayground.motionX();
    accel[1] = CircuitPlayground.motionY();
    accel[2] = CircuitPlayground.motionZ();

    bleAccel.Accel.notify(accel, sizeof(accel));
  }

  if ( xTimerID == timerLight.getHandle() )
  {
    float lux = CircuitPlayground.lightSensor();

    bleLight.Lux.notify(&lux, sizeof(lux));
  }

  if ( xTimerID == timerButton.getHandle() )
  {
    uint32_t button = 0;

    button |= ( CircuitPlayground.slideSwitch() ? 0x01 : 0x00 );
    button |= ( CircuitPlayground.leftButton()  ? 0x02 : 0x00 );
    button |= ( CircuitPlayground.rightButton() ? 0x04 : 0x00 );

    bleButton.Button.notify(&button, sizeof(button));
  }

  digitalWrite(PIN_LED1, LOW);
}

void temp_period_write_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len)
{
  timerTemp.setPeriod( bleTemp.Period.read32() );
}

void temp_notify_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t value)
{
  if (value == 0x0001)
  {
    timerTemp.setPeriod( bleTemp.Period.read32() );
    timerTemp.start();
  }else
  {
    timerTemp.stop();
  }
}

void accel_period_write_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len)
{
  timerAccel.setPeriod( bleAccel.Period.read32() );
}

void accel_notify_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t value)
{
  if (value == 0x0001)
  {
    timerAccel.setPeriod( bleAccel.Period.read32() );
    timerAccel.start();
  }else
  {
    timerAccel.stop();
  }
}

void light_period_write_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len)
{
  timerLight.setPeriod( bleLight.Period.read32() );
}

void light_notify_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t value)
{
  if (value == 0x0001)
  {
    timerLight.setPeriod( bleLight.Period.read32() );
    timerLight.start();
  }else
  {
    timerLight.stop();
  }
}

void button_period_write_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len)
{
  timerButton.setPeriod( bleButton.Period.read32() );
}

void button_notify_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t value)
{
  if (value == 0x0001)
  {
    timerButton.setPeriod( bleButton.Period.read32() );
    timerButton.start();
  }else
  {
    timerButton.stop();
  }
}

void setup()
{
  Serial.begin(115200);
  //while ( !Serial ) delay(10);   // for nrf52840 with native usb
  
  Serial.println("Bluefruit52 BLEUART Example");
  Serial.println("---------------------------\n");

  CircuitPlayground.begin();

  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(false);

  // Config the peripheral connection with maximum bandwidth 
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("Bluefruit52");
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Adafruit Service
  bleTemp.begin();
  bleTemp.Temperature.setCccdWriteCallback(temp_notify_callback);
  bleTemp.Period.setWriteCallback(temp_period_write_callback);
  bleTemp.Period.write32(5000);
  timerTemp.begin(5000, measure_callback);
  
  bleAccel.begin();
  bleAccel.Accel.setCccdWriteCallback(accel_notify_callback);
  bleAccel.Period.setWriteCallback(accel_period_write_callback);
  bleAccel.Period.write32(1000);
  timerAccel.begin(1000, measure_callback);
  
  bleLight.begin();
  bleLight.Lux.setCccdWriteCallback(light_notify_callback);
  bleLight.Period.setWriteCallback(light_period_write_callback);
  bleLight.Period.write32(3000);
  timerLight.begin(3000, measure_callback);
    
  bleButton.begin();
  bleButton.Button.setCccdWriteCallback(button_notify_callback);
  bleButton.Period.setWriteCallback(button_period_write_callback);
  bleButton.Period.write32(200);
  timerButton.begin(200, measure_callback);

  // Set up and start advertising
  startAdv();

  Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");
  Serial.println("Once connected, enter character(s) that you wish to send");
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void loop()
{

}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}
