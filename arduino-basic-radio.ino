#include "Radio.h"
#include "codes.h"
#include <Wire.h>

const ushort ADDR = 0x63;
const ushort RESET = 12;
const ushort TX_PWR = 115;
const uint FREQ = 10150;
const uint BAUD = 9600;
const ushort REV = 13;

const uint THRESHOLD = 0xFFEB;
const uint GAIN = 0x005;
const uint LIMITER_RELEASE = 0x0033;
const uint ATTACK = 0x0003;

Radio radio = Radio(ADDR, RESET);
bool initialized = false;

bool print_status(bool cond) {
  Serial.println(cond ? "Pass" : "Fail");
  return cond;
}

void reset() {
  Serial.println();
  Serial.println("Resetting Radio");
  radio.reset();
  print_status(true);
}

bool power_up() {
  Serial.println();
  Serial.println("Powering Radio Up");
  return print_status(radio.power_up());
}

bool get_rev() {
  Serial.println();
  Serial.println("Checking Rev");
  ushort rev = radio.get_rev();
  Serial.print("\tRev: ");
  Serial.println(rev);
  return print_status(rev == REV);
}

bool enable_acomp() {
  Serial.println();
  Serial.println("Enabling ACOMP");
  return print_status(
    radio.set_property(PROPS::TX_ACOMP_ENABLE, ARGS::TX_ACOMP_ENABLE::LIMITEN_ACEN));
}

bool set_threshold() {
  Serial.println();
  Serial.println("Setting ACOMP Threadhold");
  return print_status(radio.set_property(PROPS::TX_ACOMP_THRESHOLD, THRESHOLD));
}

bool set_gain() {
  Serial.println();
  Serial.println("Setting ACOMP Gain");
  return print_status(radio.set_property(PROPS::TX_ACOMP_GAIN, GAIN));
}

bool set_limit_release() {
  Serial.println();
  Serial.println("Setting Limiter Release");
  return print_status(radio.set_property(PROPS::TX_LIMITER_RELEASE_TIME, LIMITER_RELEASE));
}

bool set_attack() {
  Serial.println();
  Serial.println("Setting ACOMP Attack");
  return print_status(radio.set_property(PROPS::TX_ACOMP_ATTACK_TIME, ATTACK));
}

bool set_asq_interrupt() {
  Serial.println();
  Serial.println("Setting ASQ Interrupt");
  return print_status(radio.set_property(PROPS::TX_ASQ_INTERRUPT_SELECT,
    ARGS::TX_ASQ_INTERRUPT_SELECT::ALL));
}

bool set_freq() {
  Serial.println();
  Serial.print("Setting Frequency to ");
  Serial.print(FREQ / 100);
  Serial.print(".");
  Serial.println(FREQ % 100);
  return print_status(radio.set_freq(FREQ));
}

bool stc_loop() {
  Serial.println();
  Serial.println("Waiting for STC bit");
  return print_status(radio.stc_loop());
}

bool check_tune_status() {
  Serial.println();
  Serial.println("Checking Tune Status");
  return print_status(radio.check_tx_tune());
}

bool tune_power() {
  Serial.println();
  Serial.println("Setting Tune Power");
  return print_status(radio.set_tx_power());
}

void init_radio() {
  initialized = true;

  reset();
  if (!power_up()) { return; }
  if (!get_rev()) { return; }
  if (!enable_acomp()) { return; }
  if (!set_threshold()) { return; }
  if (!set_gain()) { return; }
  if (!set_limit_release()) { return; }
  if (!set_attack()) { return; }
  if (!set_asq_interrupt()) { return; }
  if (!set_freq()) { return; }
  if (!stc_loop()) { return; }
  if (!check_tune_status()) { return; }
  if (!tune_power()) { return; }
  if (!stc_loop()) { return; }
  if (!check_tune_status()) { return; }

  Serial.println();
  Serial.println("Setup Complete");
}

void setup() {
  Serial.begin(BAUD);
  Wire.begin();
  pinMode(RESET, OUTPUT);
}

void loop() {
  if (!initialized) {
    init_radio();
  }
}
