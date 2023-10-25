/**
 * Insound test include for configuring the environment.
 * Contains definition ENV_FILEPATH to get the path to the test-env file.
 * @example
 * `Insound::configureEnv(ENV_FILEPATH);`
 */
#pragma once
#include <insound/core/env.h>
#include <insound/tests/definitions.h>

#define ENV_FILEPATH (TEST_ROOT "/test-env")
