#include <doctest/doctest.h>

#include "Roxy.h"
#include "Core/Version.h"

TEST_CASE("Roxy runtime links and core types are usable")
{
    CHECK_NOTHROW(Roxy::Application{});
    CHECK_NOTHROW(Roxy::Version{});
}
