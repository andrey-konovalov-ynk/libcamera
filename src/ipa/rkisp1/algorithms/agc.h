/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2021-2022, Ideas On Board
 *
 * agc.h - RkISP1 AGC/AEC mean-based control algorithm
 */

#pragma once

#include <linux/rkisp1-config.h>

#include <libcamera/base/span.h>
#include <libcamera/base/utils.h>

#include <libcamera/geometry.h>

#include "algorithm.h"

namespace libcamera {

namespace ipa::rkisp1::algorithms {

class Agc : public Algorithm
{
public:
	Agc();
	~Agc() = default;

	int configure(IPAContext &context, const IPACameraSensorInfo &configInfo) override;
	void queueRequest(IPAContext &context,
			  const uint32_t frame,
			  IPAFrameContext &frameContext,
			  const ControlList &controls) override;
	void prepare(IPAContext &context, const uint32_t frame,
		     IPAFrameContext &frameContext,
		     rkisp1_params_cfg *params) override;
	void process(IPAContext &context, const uint32_t frame,
		     IPAFrameContext &frameContext,
		     const rkisp1_stat_buffer *stats,
		     ControlList &metadata) override;

private:
	void computeExposure(IPAContext &Context, IPAFrameContext &frameContext,
			     double yGain, double iqMeanGain);
	utils::Duration filterExposure(utils::Duration exposureValue);
	double estimateLuminance(Span<const uint8_t> expMeans, double gain);
	double measureBrightness(Span<const uint32_t> hist) const;
	void fillMetadata(IPAContext &context, IPAFrameContext &frameContext,
			  ControlList &metadata);

	uint64_t frameCount_;

	utils::Duration filteredExposure_;
};

} /* namespace ipa::rkisp1::algorithms */
} /* namespace libcamera */
