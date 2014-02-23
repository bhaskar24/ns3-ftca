/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009,2010 Jens Mittag, Stylianos Papanastasiou
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *      Jens Mittag <jens.mittag@kit.edu>
 *      Stylianos Papanastasiou <stylianos@gmail.com>
 */

#ifndef PHYSIM_WIFI_PHY_TAG_H
#define PHYSIM_WIFI_PHY_TAG_H

#include "ns3/wifi-mode.h"
#include "ns3/wifi-preamble.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ref-count-base.h"
#include "ns3/net-device.h"
#include <itpp/itcomm.h>

namespace ns3 {

/**
 * \brief Annotate a packet sent through a PhySimWifiPhy with transmission parameters
 * and other additional information (for cross-layer information handling or evaluation purposes).
 *
 * This tag is created when a PhySimWifiPhy instance starts to transmit a packet, and it is passed
 * through the layers and the PhySimWifiChannel to the receiving PhySimWifiPhy. During that process,
 * transmission information is collected and used by PhySimWifiPhy for further calculations.
 */
class PhySimWifiPhyTag : public Tag,
                         public RefCountBase
{
public:
  PhySimWifiPhyTag ();
  /**
   * Sets the transmission parameters which have been used by the transmitter
   * \param wifiPreamble   The preamble type (for 802.11a and p, this will be WIFI_PREAMBLE_LONG)
   * \param wifiMode       The used WiFi mode
   * \param duration       The duration of this transmission
   * \param databits       The bit vector which represents the whole payload
   * \param samples        A vector of complex samples which represents the transmitted signal in time domain
   * \param frequency      The center frequency at which the complex samples are transmitted
   * \param sampleDuration The duration of each single sample
   * \param txDevice       The pointer to the Tx NetDevice
   */
  void  SetTxParameters (WifiPreamble wifiPreamble,
                         WifiMode wifiMode,
                         Time duration,
                         itpp::bvec databits,
                         itpp::cvec samples,
                         double frequency,
                         double sampleDuration,
                         Ptr<NetDevice> txDevice);
  /**
   * Sets the received samples. Is used by the PhySimWifiChannel and PhySimPropagationLossModel to set
   * the received samples.
   * \param samples The vector of complex samples which represents the received waveform
   */
  void SetRxSamples (itpp::cvec samples);
  /**
   * Sets the background noise, for instance generated by the PhySimInterferenceHelper, that was present during the reception of
   * the corresponding frame reception. This is done in order to allow later checks by individual simulation scripts and scenarios,
   * e.g. during a TraceSource callback)
   * \param samples The samples that represent the background noise during the reception process
   */
  void SetBackgroundNoise (itpp::cvec samples);
  /**
   * Sets the detected starting time of the short training symbols. Instead of using a Time representation, we
   * use the index of the sample inside the received samples vector. This will be set in PhySimWifiPhy::EndPreamble
   * \param sample The index of the sample where the long training symbols start
   */
  void SetShortSymbolStart (uint32_t sample);
  /**
   * Sets the detected starting time of the long training symbols. Instead of using a Time representation, we
   * use the index of the sample inside the received samples vector. This will be set in PhySimWifiPhy::EndPreamble
   * \param sample The index of the sample where the long training symbols start
   */
  void SetLongSymbolStart (uint32_t sample);
  /**
   * Sets the WiFi mode as determined by the signal header decoding step. This will be done in PhySimWifiPhy::EndHeader
   * \param mode The detected WiFi mode
   */
  void SetRxWifiMode (WifiMode mode);
  /**
   * Sets the detected length of the data part of the frame in bytes. This will be done in PhySimWifiPhy::EndHeader.
   * \param lenght The detected length in bytes
   */
  void SetDetectedLength (uint32_t length);
  /**
   * Sets the initial estimate obtained during the training symbols. This will be set in PhySimWifiPhy::EndPreamble
   * \param estimate The initial estimate as obtained by PhySimChannelEstimator::GetInitialChannelEstimation
   */
  void SetInitialEstimate (double estimate);
  /**
   * Sets the decoded data bits, such that a callback function can make use of it.
   * \param payload The sequence of data bits that form the actual payload of the packet
   */
  void SetRxDataBits (itpp::bvec payload);
  /**
   * Set the calculated SINR of the preamble (short plus long training symbols)
   * \param value The calculated SINR
   */
  void SetPreambleSinr (double value);
  /**
   * Set the calculated SINR of the signal header
   * \param value The calculated SINR
   */
  void SetHeaderSinr (double value);
  /**
   * Set the calculated SINR of the payload section.
   * \param value The calculated SINR
   */
  void SetPayloadSinr (double value);
  /**
   * Set the calculated SINR of the total frame/packet
   * \param value The calculated SINR
   */
  void SetOverallSinr (double value);
  /**
   * Set the pathloss that was applied by a propagation loss model. Please note that if propagation loss
   * models are chained, they have to read this value and calculate the total pathloss first before setting
   * this value again. Also, fast fading effects are/shall not captured by this call.
   * \param value The applied pathloss by an propagation loss model
   */
  void SetPathLoss (double value);
  /**
   * Set the Tx power value that was used for this transmission.
   * \param value The used transmission power value in dBm
   */
  void SetTxPower (double value);
  /**
   * Stores the Rx NetDevice object at which the reception has started.
   * \param device The pointer to the Rx NetDevice
   */
  void SetRxNetDevice (Ptr<NetDevice> device);
  /**
   * Stores the correlation values from PhySimSignalDetector::ScanForShortTrainingSymbols
   * for potential evaluation purposes
   * \param correlations The vector with the correlation values
   */
  void SetShortTrainingSymbolCorrelations (std::vector<double> correlations);
  /**
   * Stores the correlation values from PhySimSignalDetector::ScanForLongTrainingSymbols
   * for potential evaluation purposes
   * \param correlations The vector with the correlation values
   */
  void SetLongTrainingSymbolCorrelations (std::vector<double> correlations);
  /**
   * Returns the pointer to the Rx NetDevice object.
   */
  Ptr<NetDevice> GetRxNetDevice () const;
  /**
   * Returns the pointer to the Tx NetDevice object.
   */
  Ptr<NetDevice> GetTxNetDevice () const;
  /**
   * Returns whether the frame has already been transmitted or not.
   * \return True if transmitted, false otherwise
   */
  bool IsTxed () const;
  /**
   * Returns the used WiFi Preamble type (either WIFI_PREAMBLE_LONG or WIFI_PREAMBLE_SHORT)
   * \see wifi-preamble.h
   * \return The used WiFi preamble format
   */
  WifiPreamble GetWifiPreamble () const;
  /**
   * Returns the WifiMode used in this transmission.
   * \return The used Tx WiFi mode
   */
  WifiMode GetTxWifiMode () const;
  /**
   * Returns the duration of the packet transmission.
   * \return The duration of this frame
   */
  Time GetDuration () const;
  /**
   * Returns the Tx data bit vector, which can be used to determine a successful reception at the end of PhySimWifiPhy::EndRx
   * \return The transmitted data bits as an IT++ bit vector
   */
  itpp::bvec GetTxedDataBits () const;
  /**
   * Returns the Rx data bit vector, which can be used by a callback for instance to show the difference between transmitted and
   * received data bits (e.g. by a visualizer, frontend or demonstrator)
   * \return The received data bits as an IT++ bit vector
   */
  itpp::bvec GetRxDataBits () const;
  /**
   * Returns the vector of the transmitted complex time samples.
   * \return The transmitted complex time samples
   */
  itpp::cvec GetTxedSamples () const;
  /**
   * Returns the center carrier frequency that was used in this transmission
   * \return The center frequency of the carrier used
   */
  double GetFrequency () const;
  /**
   * Returns the sample duration with which the packet was transmitted
   * \param The duration of a time sample in this transmission
   */
  double GetSampleDuration () const;
  /**
   * Returns the stored value for the preamble SINR (in dBm).
   */
  double GetPreambleSinr () const;
  /**
   * Returns the stored value for the signal header SINR (in dBm). Please note that this information might be missing,
   * if the attribute 'CalculateHeaderSinr' is set to false in PhySimWifiPhy.
   */
  double GetHeaderSinr () const;
  /**
   * Returns the stored value for the payload SINR (in dBm). Please note that this information might be missing,
   * if the attribute 'CalculatePayloadSinr' is set to false in PhySimWifiPhy.
   */
  double GetPayloadSinr () const;
  /**
   * Returns the stored value for the overall/total SINR (in dBm). Please note that this information might be missing,
   * if the attribute 'CalculateOverallSinr' is set to false in PhySimWifiPhy.
   */
  double GetOverallSinr () const;
  /**
   * Returns the stored value for the applied pathloss (in dB)
   */
  double GetPathLoss () const;
  /**
   * Returns the stored value for the transmission power used by the transmitter
   */
  double GetTxPower () const;
  /**
   * Returns the Rx WiFi mode, that was previously set by PhySimWifiPhy::EndHeader in order to remember the detected WiFi mode
   * \return The detected WiFi mode for this frame
   */
  WifiMode GetRxWifiMode () const;
  /**
   * Returns the sequence of complex time samples that represent the received signal at the receiver
   * \return The received samples represented as an IT++ complex vector
   */
  itpp::cvec GetRxedSamples () const;
  /**
   * Returns the background noise that was present during the reception of this signal or frame
   */
  itpp::cvec GetBackgroundNoise () const;
  /**
   * Returns the time sample index at which the long symbol start has been detected (usually set by PhySimSignalDetector::DetectPreamble)
   */
  uint32_t GetLongSymbolStart () const;
  /**
   * Returns the time sample index at which the short symbol start has been detected (usually set by PhySimSignalDetector::DetectPreamble)
   */
  uint32_t GetShortSymbolStart () const;
  /**
   * Returns the vector with the correlation values that have been computed by PhySimSignalDetector::ScanForShortTrainingSymbols
   */
  std::vector<double> GetShortTrainingSymbolCorrelations () const;
  /**
   * Returns the vector with the correlation values that have been computed by PhySimSignalDetector::ScanForLongTrainingSymbols
   */
  std::vector<double> GetLongTrainingSymbolCorrelations () const;
  /**
   * Returns the number of bytes that have been detected during the signal header decode process within PhySimWifiPhy::EndHeader.
   */
  uint32_t GetDetectedLength () const;
  /**
   * Returns the initial channel estimate that has been set after calling PhySimChannelEstimator::GetInitialChannelEstimation
   * within PhySimWifiPhy::EndPreamble
   */
  double GetInitialEstimate () const;
  /**
   * Marks the packet as being captured, i.e. synchronized to despite the fact that the receiver was already synchronized to a different packet
   */
  void SetCaptured ();
  /**
   * Returns whether the packet has been captured or not, i.e. if the receiver synchronized to it despite the fact of already having being synchronized to a different packet
   */
  bool IsCaptured () const;

  static TypeId GetTypeId ();
  virtual TypeId GetInstanceTypeId () const;
  virtual uint32_t GetSerializedSize () const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

private:
  // *** Parameters set by the transmitting PhySimWifiPhy ***
  itpp::bvec            m_txDataBits;
  itpp::cvec            m_txSamples;
  bool                  m_txed;
  WifiPreamble          m_wifiPreamble;
  WifiMode              m_txWifiMode;
  Time                  m_duration;
  double                m_frequency;
  double                m_txPower;
  double                m_sampleDuration;
  Ptr<NetDevice>        m_txNetDevice;

  // *** Parameters set by the channel ***
  double                m_pathLoss;

  // *** Parameters set by the receiving PhySimWifiPhy ***
  itpp::cvec            m_rxSamples;
  itpp::bvec            m_rxDatabits;
  uint32_t              m_beginShortSymbols;
  uint32_t              m_beginLongSymbols;
  WifiMode              m_rxWifiMode;
  uint32_t              m_length;
  double                m_initialEstimate;
  Ptr<NetDevice>        m_rxNetDevice;

  // *** Parameters set for evaluation... e.g. SINR and BER
  double                m_preambleSinr;
  double                m_headerSinr;
  double                m_payloadSinr;
  double                m_overallSinr;
  itpp::cvec            m_bgNoise;
  std::vector<double>   m_shortTrainingSymbolCorrelations;
  std::vector<double>   m_longTrainingSymbolCorrelations;
  bool                  m_captured;

};

std::ostream& operator<< (std::ostream& os, const PhySimWifiPhyTag& tag);



} // namespace ns3


#endif /* PHYSIM_WIFI_PHY_TAG_H */