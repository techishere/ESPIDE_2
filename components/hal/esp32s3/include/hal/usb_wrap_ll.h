/*
 * SPDX-FileCopyrightText: 2015-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdbool.h>
#include "esp_attr.h"
#include "soc/soc.h"
#include "soc/system_struct.h"
#include "soc/usb_wrap_struct.h"
#include "soc/rtc_cntl_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------- USB PHY Control  ---------------------------- */

/**
 * @brief Enables and sets the override value for the session end signal
 *
 * @param hw Start address of the USB Wrap registers
 * @param sessend Session end override value. True means VBus < 0.2V, false means VBus > 0.8V
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_enable_srp_sessend_override(usb_wrap_dev_t *hw, bool sessend)
{
    hw->otg_conf.srp_sessend_value = sessend;
    hw->otg_conf.srp_sessend_override = 1;
}

/**
 * @brief Disable session end override
 *
 * @param hw Start address of the USB Wrap registers
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_disable_srp_sessend_override(usb_wrap_dev_t *hw)
{
    hw->otg_conf.srp_sessend_override = 0;
}

/**
 * @brief Sets whether the USB Wrap's FSLS PHY interface routes to an internal or external PHY
 *
 * @param hw Start address of the USB Wrap registers
 * @param enable Enables external PHY, internal otherwise
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_enable_external(usb_wrap_dev_t *hw, bool enable)
{
    hw->otg_conf.phy_sel = enable;
    // Enable SW control of muxing USB OTG vs USJ to the internal USB FSLS PHY
    RTCCNTL.usb_conf.sw_hw_usb_phy_sel = 1;
    /*
    For 'sw_usb_phy_sel':
    0 - Internal USB FSLS PHY is mapped to the USJ. USB Wrap mapped to external PHY
    1 - Internal USB FSLS PHY is mapped to the USB Wrap. USJ mapped to external PHY
    */
    RTCCNTL.usb_conf.sw_usb_phy_sel = !enable;
}

/**
 * @brief Enables/disables exchanging of the D+/D- pins USB PHY
 *
 * @param hw Start address of the USB Wrap registers
 * @param enable Enables pin exchange, disabled otherwise
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_enable_pin_exchg(usb_wrap_dev_t *hw, bool enable)
{
    if (enable) {
        hw->otg_conf.exchg_pins = 1;
        hw->otg_conf.exchg_pins_override = 1;
    } else {
        hw->otg_conf.exchg_pins_override = 0;
        hw->otg_conf.exchg_pins = 0;
    }
}

/**
 * @brief Enables and sets voltage threshold overrides for USB FSLS PHY single-ended inputs
 *
 * @param hw Start address of the USB Wrap registers
 * @param vrefh_step High voltage threshold. 0 to 3 indicating 80mV steps from 1.76V to 2V.
 * @param vrefl_step Low voltage threshold. 0 to 3 indicating 80mV steps from 0.8V to 1.04V.
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_enable_vref_override(usb_wrap_dev_t *hw, unsigned int vrefh_step, unsigned int vrefl_step)
{
    hw->otg_conf.vrefh = vrefh_step;
    hw->otg_conf.vrefl = vrefl_step;
    hw->otg_conf.vref_override = 1;
}

/**
 * @brief Disables voltage threshold overrides for USB FSLS PHY single-ended inputs
 *
 * @param hw Start address of the USB Wrap registers
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_disable_vref_override(usb_wrap_dev_t *hw)
{
    hw->otg_conf.vref_override = 0;
}

/**
 * @brief Enable override of USB FSLS PHY's pull up/down resistors
 *
 * @param hw Start address of the USB Wrap registers
 * @param dp_pu Enable D+ pullup
 * @param dm_pu Enable D- pullup
 * @param dp_pd Enable D+ pulldown
 * @param dm_pd Enable D- pulldown
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_enable_pull_override(usb_wrap_dev_t *hw, bool dp_pu, bool dm_pu, bool dp_pd, bool dm_pd)
{
    hw->otg_conf.dp_pullup = dp_pu;
    hw->otg_conf.dp_pulldown = dp_pd;
    hw->otg_conf.dm_pullup = dm_pu;
    hw->otg_conf.dm_pulldown = dm_pd;
    hw->otg_conf.pad_pull_override = 1;
}

/**
 * @brief Disable override of USB FSLS PHY pull up/down resistors
 *
 * @param hw Start address of the USB Wrap registers
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_disable_pull_override(usb_wrap_dev_t *hw)
{
    hw->otg_conf.pad_pull_override = 0;
}

/**
 * @brief Sets the strength of the pullup resistor
 *
 * @param hw Start address of the USB Wrap registers
 * @param strong True is a ~1.4K pullup, false is a ~2.4K pullup
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_set_pullup_strength(usb_wrap_dev_t *hw, bool strong)
{
    hw->otg_conf.pullup_value = strong;
}

/**
 * @brief Check if USB FSLS PHY pads are enabled
 *
 * @param hw Start address of the USB Wrap registers
 * @return True if enabled, false otherwise
 */
FORCE_INLINE_ATTR bool usb_wrap_ll_phy_is_pad_enabled(usb_wrap_dev_t *hw)
{
    return hw->otg_conf.pad_enable;
}

/**
 * @brief Enable the USB FSLS PHY pads
 *
 * @param hw Start address of the USB Wrap registers
 * @param enable Whether to enable the USB FSLS PHY pads
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_enable_pad(usb_wrap_dev_t *hw, bool enable)
{
    hw->otg_conf.pad_enable = enable;
}

/**
 * @brief Set USB FSLS PHY TX output clock edge
 *
 * @param hw Start address of the USB Wrap registers
 * @param clk_neg_edge True if TX output at negedge, posedge otherwise
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_set_tx_edge(usb_wrap_dev_t *hw, bool clk_neg_edge)
{
    hw->otg_conf.phy_tx_edge_sel = clk_neg_edge;
}

/* ------------------------------ USB PHY Test ------------------------------ */

/**
 * @brief Enable the USB FSLS PHY's test mode
 *
 * @param hw Start address of the USB Wrap registers
 * @param enable Whether to enable the USB FSLS PHY's test mode
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_enable_test_mode(usb_wrap_dev_t *hw, bool enable)
{
    hw->test_conf.test_enable = enable;
}

/**
 * @brief Set the USB FSLS PHY's signal test values
 *
 * @param hw Start address of the USB Wrap registers
 * @param oen Output Enable (active low) signal
 * @param tx_dp TX D+
 * @param tx_dm TX D-
 * @param rx_dp RX D+
 * @param rx_dm RX D-
 * @param rx_rcv RX RCV
 */
FORCE_INLINE_ATTR void usb_wrap_ll_phy_test_mode_set_signals(usb_wrap_dev_t *hw,
                                                             bool oen,
                                                             bool tx_dp,
                                                             bool tx_dm,
                                                             bool rx_dp,
                                                             bool rx_dm,
                                                             bool rx_rcv)
{
    usb_wrap_test_conf_reg_t test_conf;
    test_conf.val = hw->test_conf.val;

    test_conf.test_usb_wrap_oe = oen;
    test_conf.test_tx_dp = tx_dp;
    test_conf.test_tx_dm = tx_dm;
    test_conf.test_rx_rcv = rx_rcv;
    test_conf.test_rx_dp = rx_dp;
    test_conf.test_rx_dm = rx_dm;

    hw->test_conf.val = test_conf.val;
}

/* ----------------------------- RCC Functions  ----------------------------- */

/**
 * Enable the bus clock for USB Wrap module
 * @param clk_en True if enable the clock of USB Wrap module
 */
FORCE_INLINE_ATTR void usb_wrap_ll_enable_bus_clock(bool clk_en)
{
    SYSTEM.perip_clk_en0.usb_clk_en = clk_en;
}

/**
 * @brief Reset the USB Wrap module
 */
FORCE_INLINE_ATTR void usb_wrap_ll_reset_register(void)
{
    SYSTEM.perip_rst_en0.usb_rst = 1;
    SYSTEM.perip_rst_en0.usb_rst = 0;
}

#ifdef __cplusplus
}
#endif
