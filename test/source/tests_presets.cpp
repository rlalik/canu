#include <gtest/gtest.h>

#include "canu/canu.hpp"

TEST(TestsPresets, EPJA) { auto canu = cu::make_epja_paper(); }

TEST(TestsPresets, NIMA) { auto canu = cu::make_nima_paper(); }

TEST(TestsPresets, PRC) { auto canu = cu::make_prc_paper(); }

TEST(TestsPresets, SN_JNL) { auto canu = cu::make_sn_jnl_paper(); }
