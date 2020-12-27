#ifndef ADS129X_REG_H
#define ADS129X_REG_H

#ifdef __cplusplus
extern "C" {
#endif

enum SPI_COMMAND {
  // System commands
  WAKEUP  = 0x02,
  STANDBY = 0x04,
  RST     = 0x06,
  START   = 0x08,
  STOP    = 0x0a,

  // Read commands
  RDATAC  = 0x10,
  SDATAC  = 0x11,
  RDATA   = 0x12,

  // Register commands
  RREG    = 0x20,
  WREG    = 0x40
};

enum REG {
  // Device settings
  ID      = 0x00,

  // Global settings
  CONFIG1 = 0x01,
  CONFIG2 = 0x02,
  LOFF    = 0x03,

  // Channel specific settings
  CHNSET = 0X03,
  CH1SET = CHNSET + 1,
  CH2SET = CHNSET + 2,

  // Lead off status
  RLD_SENS = 0x06,
  LOFF_SENS = 0x07,
  LOFF_STAT = 0x08,

  // Other
  GPIO = 0x0B,
  RESP1 = 0x09,
  RESP2 = 0x0A
};

enum ID_BITS {
  DEV_ID7 = 0x80,
  DEV_ID6 = 0x40,
  DEV_ID5 = 0x20,
  
  DEV_ID1 = 0x02,
  DEV_ID0 = 0x01,

  ID_CONST = 0x10,
  
  ID_ADS1X9X = (ID_CONST | DEV_ID6),
  
  ID_ADS1292R = (ID_ADS1X9X | DEV_ID5 | DEV_ID1 | DEV_ID0)
};

//CONFIG1: Configuration Register 1
//Address = 01h
// -----------------------------------------------------------------
// |BIT 7       |BIT 6 |BIT 5  |BIT 4 |BIT 3 |BIT 2 |BIT 1  |BIT 0 |
// |------------|------|-------|------|------|------|-------|------|
// |SINGLE_SHOT |   0  |   0   | 0    |  0   | OSR2 | OSR1  | OSR0 |
// -----------------------------------------------------------------
//
//Bit 7 SINGLE_SHOT: Single-shot conversion
//    This bit sets the conversion mode
//       0 = Continuous conversion mode (default)
//       1 = Single-shot mode
//    NOTE: Pulse mode is disabled when individual data rate control is selected.
//
//Bits[6:3] Must be set to '0'
//
//Bits[2:0] OSR[2:0]: Channel oversampling ratio
//These bits determine the oversampling ratio of both channel 1 and channel 2.
// ----------------------------------------------------------
//          BIT   |  OVERSAMPLING RATIO   |  SAMPLE RATE    |
//          000   |  fMOD/1024            |  125SPS         |
//          001   |  fMOD/512             |  250SPS         |
//          010   |  fMOD/256             |  500SPS         |
//          011   |  fMOD/128             |  1024SPS        |
//          100   |  fMOD/128             |  2048SPS        |
//          101   |  Do Not Use           |  Do Not Use     |
//          110   |  Do Not Use           |  Do Not Use     |
//          111   |  Do Not Use           |  Do Not Use     |
// ----------------------------------------------------------

enum CONFIG1_BITS {
  SINGLE_SHOT = 0x80,
  DR2 = 0x04,
  DR1 = 0x02,
  DR0 = 0x01,

  CONFIG1_CONST = 0x00,
  SAMP_125_SPS = CONFIG1_CONST,
  SAMP_250_SPS = (CONFIG1_CONST | DR0),
  SAMP_500_SPS = (CONFIG1_CONST | DR1),
  SAMP_1_KSPS = (CONFIG1_CONST | DR1 | DR0),
  SAMP_2_KSPS = (CONFIG1_CONST | DR2), 
  SAMP_4_KSPS = (CONFIG1_CONST | DR2 | DR0),
  SAMP_8_KSPS = (CONFIG1_CONST | DR2 | DR1)
  
};

//CONFIG2: Configuration Register 2
//Address = 02h
// ----------------------------------------------------------------------------------
// |BIT 7  |BIT 6         |BIT 5     | BIT 4   |  BIT 3 |BIT 2  |BIT 1    |BIT 0    |
// |-------|--------------|----------|---------|--------|-------|---------|---------|
// |   1   |PDB_LOFF_COMP |PDB_REFBUF| VREF_4V | CLK_EN |   0   |INT_TEST |TEST_FREQ|
// ----------------------------------------------------------------------------------
//
//Configuration Register 2 configures the test signal generation. See the Input Multiplexer section for more details.
//Bit 7 Must always be set to '1'
//
//Bit 6 PDB_LOFF_COMP : Lead-off comparator power-down
//    This bit powers down the lead-off comparators.
//       0 = Lead-off comparators disabled (default)
//       1 = Lead-off comparators enabled
//
//Bit 5 PDB_REFBUF : Reference buffer power-down
//    This bit powers down the internal reference buffer so that the external reference can be used.
//       0 = Reference buffer is powered down (default)
//       1 = Reference buffer is enabled
//
//Bit 4 VREF_4V: Enables 4-V reference
//    This bit chooses between 2.4V and 4V reference.
//       0 = 2.4-V reference (default)
//       1 = 4-V reference
//
//Bit 3 CLKOUT_EN: CLK connection
//    This bit determines if the internal oscillator signal is connected to the CLK pin when an internal oscillator is used.
//       0 = Oscillator clock output disabled (default)
//       1 = Oscillator clock output enabled
//
//Bit 2 Must be set to '0'
//
//Bit 1 TEST_AMP: Test signal amplitude
//    This bit determines the test signal amplitude.
//       0 = No test signal (default)
//       1 = ±(VREFP  VREFN)/2400
//
//Bit 0 TEST_FREQ: Test signal frequency.
//    This bit determines the test signal frequency.
//       0 = At dc (default)
//       1 = Square wave at 1 Hz

enum CONFIG2_BITS {
  PDB_LOFF_COMP = 0x40,
  PDB_REFBUF = 0X20,
  VREF_4V = 0x10,
  CLK_EN = 0X08,
  INT_TEST = 0x02,
  TEST_FREQ = 0x01,

  CONFIG2_CONST = 0x80,
  INT_TEST_1HZ = (CONFIG2_CONST | INT_TEST | TEST_FREQ), 
  INT_TEST_DC = (CONFIG2_CONST | INT_TEST)
};

//LOFF: Lead-Off Control Register
//Address = 03h
// -----------------------------------------------------------------------------------------------
// | BIT 7    |BIT 6    |BIT 5    |BIT 4     | BIT 3      |BIT 2       |BIT 1         | BIT 0    |
// |----------|-------  |-------  |----------|----------- |---------   |--------------|----------|
// |COMP_TH2  |COMP_TH1 |COMP_TH0 | 0        | ILEAD_OFF1 | ILEAD_OFF0 |0             | FLEAD_OFF|
// -----------------------------------------------------------------------------------------------
//
//The Lead-Off Control Register configures the Lead-Off detection operation.
//
//Bits[7:5] Lead-off comparator threshold
//    These bits determine the lead-off comparator threshold.
//    Comparator positive side in %
//       000 = 95 (default)
//       001 = 92.5
//       010 = 90
//       011 = 87.5
//       100 = 85
//       101 = 80
//       110 = 75
//       111 = 70
//    Comparator negative side in %
//       000 = 5 (default)
//       001 = 7.5
//       010 = 10
//       011 = 12.5
//       100 = 15
//       101 = 20
//       110 = 25
//       111 = 30
//
//Bit 4 Must be set to 1
//
//Bits[3:2] ILEAD_OFF[1:0]: Lead-off current magnitude
//    These bits determine the magnitude of current for the current lead-off mode.
//       00 = 6 nA (default)
//       01 = 24 nA
//       10 = 6 microA
//       11 = 24 microA
//
//Bit 1 Must be set to '0'
//
//Bit 0 FLEAD_OFF: Lead-off frequency
//    This bit generates the LEAD_OFF_CLK signal. It also generates AC_LEAD_OFF, which is '1' when FLEAD_OFF is '1'.
//       0 = At dc lead-off detect (default)
//       1 = At ac lead-off detect at DECICLK/4 (500 Hz for an 2-kHz output rate)

enum LOFF_BITS {
  COMP_TH2 = 0x80,
  COMP_TH1 = 0x40,
  COMP_TH0 = 0x20,
  VLEAD_OFF_EN = 0x10,
  ILEAD_OFF1 = 0x08,
  ILEAD_OFF0 = 0x04,
  FLEAD_OFF1 = 0x02,
  FLEAD_OFF0 = 0x01,

  LOFF_CONST = 0x10,

  COMP_TH_95 = 0x00,
  COMP_TH_92_5 = COMP_TH0,
  COMP_TH_90 = COMP_TH1,
  COMP_TH_87_5 = (COMP_TH1 | COMP_TH0),
  COMP_TH_85 = COMP_TH2,
  COMP_TH_80 = (COMP_TH2 | COMP_TH0),
  COMP_TH_75 = (COMP_TH2 | COMP_TH1),
  COMP_TH_70 = (COMP_TH2 | COMP_TH1 | COMP_TH0),

  ILEAD_OFF_6nA = 0x00,
  ILEAD_OFF_12nA = ILEAD_OFF0,
  ILEAD_OFF_18nA = ILEAD_OFF1,
  ILEAD_OFF_24nA = (ILEAD_OFF1 | ILEAD_OFF0),

  FLEAD_OFF_AC = FLEAD_OFF0,
  FLEAD_OFF_DC = (FLEAD_OFF1 | FLEAD_OFF0)
};

enum CHNSET_BITS {
  PD = 0x80,
  
  GAIN_1X  = 0x10,
  GAIN_2X  = 0x20,
  GAIN_3X  = 0x30,
  GAIN_4X  = 0x40,
  GAIN_6X  = 0x00,
  GAIN_8X  = 0x50,
  GAIN_12X = 0x60,
  
  MUX0 = 0x01,
  MUX1 = 0x02,
  MUX2 = 0x04,
  
  ELECTRODE_INPUT = 0x00,
  SHORTED = MUX0,
  RLD_INPUT = MUX1,
  MVDD = (MUX1 | MUX0),
  TEMP = MUX2,
  TEST_SIGNAL = (MUX2 | MUX0),
  RLD_DRP = (MUX2 | MUX1),
  RLD_DRN = (MUX2 | MUX1 | MUX0)
};

//CHnSET: Individual Channel Settings
//Address = 04h
// ------------------------------------------------------------------------------
// | BIT 7    |BIT 6   |BIT 5   |  BIT 4  | BIT 3  | BIT 2  |  BIT 1  | BIT 0   |
// |----------|--------|--------|---------|--------|--------|---------|---------|
// |   PD1    |GAIN1_1 |GAIN1_0 |  0      |  MUX1_3| MUX1_2 |  MUX1_1 | MUX1_0  |
// ------------------------------------------------------------------------------
//
//The CH1SET Control Register configures the power mode, PGA gain, and multiplexer settings channels.
//
//Bit 7 PD1: Channel 1 power-down
//    0 = Normal operation (default)
//    1 = Channel 1 power-down
//
//Bits[6:4 ] GAIN1[2:0]: Channel 1 PGA gain setting
//    These bits determine the PGA gain setting for channel 1.
//       000 = 6 (default)
//       001 = 1
//       010 = 2
//       011 = 3
//       100 = 4
//       101 = 8
//       110 = 12
//
//Bits[3:0] MUX1[3:0]: Channel 1 input selection
//    These bits determine the channel 1 input selection.
//       0000 = Normal electrode input (default)
//       0001 = Input shorted (for offset measurements)
//       0010 = RLD_MEASURE
//       0011 = VDD/2 for supply measurement
//       0100 = Temperature sensor
//       0101 = Cal signal
//       0110 = RLD_DRP (positive electrode is the driver)
//       0111 = RLD_DRM (negative electrode is the driver)
//       1000 = Reserved
//       1001 = MUX RESPP/RESPN to INP/INM
//       1010 = Reserved

enum CH1SET_BITS {
  CH1SET_CONST = 0x00
};

//CHnSET: Individual Channel Settings
//Address = 05h
// ------------------------------------------------------------------------------
// | BIT 7    |BIT 6   |BIT 5   |  BIT 4  | BIT 3  | BIT 2  |  BIT 1  | BIT 0   |
// |----------|--------|--------|---------|--------|--------|---------|---------|
// |   PD2    |GAIN2_1 |GAIN2_0 |  0      |  MUX2_3| MUX2_2 |  MUX2_1 | MUX2_0  |
// ------------------------------------------------------------------------------
//
//The CH1SET Control Register configures the power mode, PGA gain, and multiplexer settings channels.
//
//Bit 7 PD2: Channel 1 power-down
//    0 = Normal operation (default)
//    1 = Channel 1 power-down
//
//Bits[6:4 ] GAIN2[2:0]: Channel 1 PGA gain setting
//    These bits determine the PGA gain setting for channel 1.
//       000 = 6 (default)
//       001 = 1
//       010 = 2
//       011 = 3
//       100 = 4
//       101 = 8
//       110 = 12
//
//Bits[3:0] MUX2[3:0]: Channel 1 input selection
//    These bits determine the channel 1 input selection.
//       0000 = Normal electrode input (default)
//       0001 = Input shorted (for offset measurements)
//       0010 = RLD_MEASURE
//       0011 = VDD/2 for supply measurement
//       0100 = Temperature sensor
//       0101 = Cal signal
//       0110 = RLD_DRP (positive electrode is the driver)
//       0111 = RLD_DRM (negative electrode is the driver)
//       1000 = Reserved
//       1001 = MUX RESPP/RESPN to INP/INM
//       1010 = Reserved

enum CH2SET_BITS {
  CH2SET_CONST = 0x00
};

//RLD_SENSP
//Address = 06h
// -----------------------------------------------------------------------------------
// | BIT 7  | BIT 6 |BIT 5   |  BIT 4          | BIT 3  | BIT 2  |  BIT 1  | BIT 0   |
// |--------|-------|--------|-----------------|--------|--------|---------|---------|
// |  CHOP1 | CHOP0 |PD_RLD  |  RLD_LOFF_SENS  | RLDN2  | RLDP2  |  RLDN1  | RLDP1   |
// -----------------------------------------------------------------------------------
//
//This register controls the selection of the positive and negative signals from each channel for right leg drive
//derivation. See the Right Leg Drive (RLD DC Bias Circuit) subsection of the ECG-Specific Functions section for
//details.
//
//Bits[7:6] CHOP[1:0]: Chop frequency
//    These bits determine PGA chop frequency
//       00 = fMOD/16
//       01 = fMOD/32
//       10 = fMOD/2
//       11 = fMOD/4
//
//Bit 5 PDB_RLD: RLD buffer power
//    This bit determines the RLD buffer power state.
//       0 = RLD buffer is powered down (default)
//       1 = RLD buffer is enabled
//
//Bit 4 RLD_LOFF_SENSE: RLD lead-off sense function
//    This bit enables the RLD lead-off sense function.
//       0 = RLD lead-off sense is disabled (default)
//       1 = RLD lead-off sense is enabled
//
//Bit 3 RLDN2: Channel 2 RLD negative inputs
//    This bit controls the selection of negative inputs from channel 2 for right leg drive derivation.
//       0 = Not connected (default)
//       1 = RLD connected to IN2N
//
//Bit 2 RLDP2: Channel 2 RLD positive inputs
//    This bit controls the selection of positive inputs from channel 2 for right leg drive derivation.
//       0 = Not connected (default)
//       1 = RLD connected to IN2P
//
//Bit 1 RLDN1: Channel 1 RLD negative inputs
//    This bit controls the selection of negative inputs from channel 1 for right leg drive derivation.
//       0 = Not connected (default)
//       1 = RLD connected to IN1N
//
//Bit 0 RLDP1: Channel 1 RLD positive inputs
//    This bit controls the selection of positive inputs from channel 1 for right leg drive derivation.
//       0 = Not connected (default)
//       1 = RLD connected to IN1P

enum RLD_SENS_BITS {
  FMOD16 = 0x00,
  FMOD2  = 0x80,
  FMOD4  = 0xC0,

  PDB_RLD = 0x20,
  RLD_LOFF_SENSE = 0x10,

  RLD2N = 0x08,
  RLD2P = 0x04,
  RLD1N = 0x02,
  RLD1P = 0x01,

  RLD_SENS_CONST = 0x00
};

//LOFF_SENS
//Address = 07h
// -------------------------------------------------------------------------------
// | BIT 7    |BIT 6    |BIT 5   |  BIT 4  | BIT 3  | BIT 2  |  BIT 1  | BIT 0   |
// |----------|---------|--------|---------|--------|--------|---------|---------|
// | 0        |   0     |FLIP2   |  FLIP1  | LOFFN2 | LOFFP2 |  LOFFN1 | LOFFP1  |
// -------------------------------------------------------------------------------
//
//This register selects the positive and negative side from each channel for lead-off detection. See the Lead-Off
//Detection subsection of the ECG-Specific Functions section for details. Note that the LOFF_STAT register bits
//should be ignored if the corresponding LOFF_SENS bits are set to '1'.
//
//Bits[7:6] Must be set to '0'
//
//Bit 5 FLIP2: Current direction selection
//    This bit controls the direction of the current used for lead off derivation.
//       0 = Disabled (default)
//       1 = Enabled
//
//Bit 4 FLIP1: Current direction selection
//    This bit controls the direction of the current used for lead off derivation.
//       0 = Disabled (default)
//       1 = Enabled
//
//Bit 3 LOFFN2: Channel 2 lead-off detection negative inputs
//    This bit controls the selection of negative input from channel 2 for lead off detection.
//       0 = Disabled (default)
//       1 = Enabled
//
//Bit 2 LOFFP2: Channel 2 lead-off detection positive inputs
//    This bit controls the selection of positive input from channel 2 for lead off detection.
//       0 = Disabled (default)
//       1 = Enabled
//
//Bit 1 LOFFN1: Channel 1 lead-off detection negative inputs
//    This bit controls the selection of negative input from channel 1 for lead off detection.
//       0 = Disabled (default)
//       1 = Enabled
//
//Bit 0 LOFFP1: Channel 1 lead-off detection positive inputs
//    This bit controls the selection of positive input from channel 1 for lead off detection.
//       0 = Disabled (default)
//       1 = Enabled

enum LOFF_SENS_BITS {
  FLIP2  = 0x20,
  FLIP1  = 0x10,

  LOFF2N = 0x08,
  LOFF2P = 0x04,
  LOFF1N = 0x02,
  LOFF1P = 0x01,

  LOFF_SENS_CONST = 0x00
};

//LOFF_STAT
//Address = 08h
// -----------------------------------------------------------------------------------------
// | BIT 7    |BIT 6    |BIT 5   |  BIT 4     | BIT 3    | BIT 2    |  BIT 1    | BIT 0    |
// |----------|---------|--------|------------|----------|----------|-----------|----------|
// |  0       |MOD_FREQ |0       |  RLD_STAT  | IN2N_OFF | IN2P_OFF | IN1N_OFF  | IN1P_OFF |
// -----------------------------------------------------------------------------------------
//
//This register stores the status of whether the positive or negative electrode on each channel is on or off. See the
//Lead-Off Detection subsection of the ECG-Specific Functions section for details. Ignore the LOFF_STAT values
//if the corresponding LOFF_SENS bits are not set to '1'.
//'0' is lead-on (default) and '1' is lead-off. When the LOFF_SENS bits are '0', the LOFF_STAT bits should be
//ignored.
//
//Bit 7 Must be set to '0'
//
//Bit 6 MOD_FREQ: System frequency selection
//    This bit sets the modultar frequency. Two external clock values are supported: 512 kHz and 2.048 MHz. This bit must be set
//    so that MOD_FREQ = 128 kHz.
//       0 = External_CLK/4 (default)
//       1 = External_CLK/16
//
//Bit 5 Must be set to '0'
//
//Bit 4 RLD_STAT: RLD lead-off status
//    This bit determines the status of RLD.
//       0 = RLD is connected (default)
//       1 = RLD is not connected
//
//Bit 3 IN2N_OFF: Channel 2 negative electrode status
//    This bit determines if the channel 2 negative electrode is connected or not.
//       0 = Connected (default)
//       1 = Not connected
//
//Bit 2 IN2P_OFF: Channel 2 positive electrode status
//    This bit determines if the channel 2 positive electrode is connected or not.
//       0 = Connected (default)
//       1 = Not connected
//
//Bit 1 IN1N_OFF: Channel 1 negative electrode status
//    This bit determines if the channel 1 negative electrode is connected or not.
//       0 = Connected (default)
//       1 = Not connected
//
//Bit 0 IN1P_OFF: Channel 1 positive electrode status
//    This bit determines if the channel 1 positive electrode is connected or not.
//       0 = Connected (default)
//       1 = Not connected

enum LOFF_STAT_BITS {
  CLK_DIV_FCLK4   = 0x40,
  CLK_DIV_FCLK16  = 0x40,

  RLD_STAT = 0x10,

  IN2N_OFF = 0x08,
  IN2P_OFF = 0x04,
  IN1N_OFF = 0x02,
  IN1P_OFF = 0x01,

  LOFF_STAT_CONST = 0x00
};

//RESP1: Respiration Control Register 1
//Address = 09h
// --------------------------------------------------------------------------------------------
// | BIT 7       |  BIT 6       |BIT 5    |  BIT 4  | BIT 3    | BIT 2    |  BIT 1  | BIT 0   |
// |-------------|--------------|---------|---------|----------|----------|---------|---------|
// | RESP_MOD_EN |  RESP_MOD_EN |RESP_PH3 |RESP_PH2 | RESP_PH1 | RESP_PH0 |  1      | REP_CTL |
// --------------------------------------------------------------------------------------------
//
//This register controls the respiration functionality.
//
//Bit 7 RESP_DEMOD_EN1: Enables respiration demodulation circuitry
//    This bit enables/disables the demodulation circuitry on channel 1.
//       0 = RESP demodulation circuitry turned off (default)
//       1 = RESP demodulation circuitry turned on
//
//Bit 6 RESP_MOD_EN: Enables respiration modulation circuitry
//       This bit enables/disables the modulation circuitry on channel 1.
//       0 = RESP modulation circuitry turned off (default)
//       1 = RESP modulation circuitry turned on
//
//Bits[5:2] RESP_PH[3:0]: Respiration phase(1)
//    These bits control the phase of the respiration demodulation control signal.
//    RESP_PH[3:0]   RESP_CLK = 32kHz     RESP_CLK = 64kHz
//       0000        0° (default)         0° (default)
//       0001        11.25°               22.5°
//       0010        22.5°                45°
//       0011        33.75°               67.5°
//       0100        45°                  90°
//       0101        56.25°               112.5°
//       0110        67.5°                135°
//       0111        78.75°               157.5°
//       1000        90°                  Not available
//       1001        101.25°              Not available
//       1010        112.5°               Not available
//       1011        123.75°              Not available
//       1100        135°                 Not available
//       1101        146.25°              Not available
//       1110        157.5°               Not available
//       1111        168.75°              Not available
//
//    (1) The RESP_PH3 bit is ignored when RESP_CLK = 64kHz.
//
//Bit 1 Must be set to '1'
//
//Bit 0 RESP_CTRL: Respiration control
//    This bit sets the mode of the respiration circuitry.
//       0 = Internal respiration with internal clock
//       1 = Internal respiration with external clock

enum RESP1_BITS {
  RESP_DEMOD_EN = 0x80,
  RESP_MOD_EN = 0x40,

  RESP_CTRL = 0x01,

  RESP1_CONST = 0x2
};

//RESP2: Respiration Control Register 2
//Address = 0Ah
// -------------------------------------------------------------------------------------------
// | BIT 7    |BIT 6     | BIT 5    |   BIT 4  |  BIT 3   |  BIT 2   |   BIT 1    | BIT 0    |
// |----------|----------|----------|----------|----------|----------|------------|----------|
// |CALIB_ON  |0         |   0      |     0    |     0    | RESP_FREQ| RLDREF_INT |  1       |
// -------------------------------------------------------------------------------------------
//
//This register controls the respiration functionality.
//
//Bit 7 CALIB_ON: Calibration on
//    This bit is used to enable offset calibration.
//       0 = Off (default)
//       1 = On
//
//Bits[6:3] Must be '0'
//
//Bit 2 RESP_FREQ: Respiration control frequency
//    This bit controls the respiration control frequency when RESP_CTRL[1:0] = 10.
//       0 = 32 kHz (default)
//       1 = 64 kHz
//
//Bit 1 RLDREF_INT: RLDREF signal
//    This bit determines the RLDREF signal source.
//       0 = RLDREF signal fed externally
//       1 = RLDREF signal (AVDD - AVSS)/2 generated internally (default)
//
//Bit 0 Must be set to '1;

enum RESP2_BITS {
  CALIB_ON = 0x80,
  RESP_FREQ32KHZ = 0x00,
  RESP_FREQ64KHZ = 0x04,

  RLDREF_INT = 0x2,

  RESP2_CONST = 0x01
};

enum GPIO_BITS {
  GPIOC2 = 0x08,
  GPIOC1 = 0x04,

  GPIOD2 = 0x02,
  GPIOD1 = 0x01,

  GPIO_CONST = 0x00
};

#ifdef __cplusplus
}
#endif

#endif
