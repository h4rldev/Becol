//// becol config

// profiles

// NOTE: These things can be defined at build time through -DDEBUG_BUILD and
// -DRELEASE_BUILD
//
// #define DEBUG_BUILD
// #define RELEASE_BUILD

#if defined(DEBUG_BUILD) && defined(RELEASE_BUILD)
#error Cannot specify two build profiles at the same time
#endif

// Module search paths

// NOTE: I feel these should be searched by default.

#define MODULE_SEARCH_RUNTIME_DIR
#define MODULE_SEARCH_ENV_VAR
#define MODULE_SEARCH_LAUNCH_ARG

// #if !(defined(MODULE_SEARCH_ENV_VAR) || defined(MODULE_SEARCH_RUNTIME_DIR) ||
// defined(MODULE_SEARCH_LAUNCH_ARG))
//     #error Must enable atleast one module search path
// #endif

// Built in modules
// NOTE: No need to define this in config if its built-in
#define MODULE_CONSOLE

// misc
#ifdef DEBUG_BUILD
#define INCLUDE_ASTTREE_TOKEN_NAMES
#endif
