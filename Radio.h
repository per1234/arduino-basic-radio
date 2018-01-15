#define ushort unsigned short
#define uint unsigned int

class Radio {
public:
  Radio(short addr, uint reset);

  void reset();
  bool send(const ushort data[], int len);
  bool set_property(const uint prop, const uint prop_val);
  bool power_up();
  ushort get_rev();
  bool set_freq(uint freq);
  ushort get_int_status();
  bool stc_loop();
  bool check_tx_tune();
  bool set_tx_power(const ushort pwr=115);
private:
  short _addr;
  uint _reset;
};
