#include <boost/test/unit_test.hpp>

#include "Werk/OS/CpuMask.hpp"

BOOST_AUTO_TEST_SUITE(CpuMaskTest)

BOOST_AUTO_TEST_CASE(TestGetProcessorCount) {
	Werk::CpuMask mask(Werk::getProcessorCount());
	mask.set(0);
	mask.set(1);
	mask.apply();
}

BOOST_AUTO_TEST_SUITE_END()