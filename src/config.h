//// becol config

// Module search paths
#define MODULE_SEARCH_RUNTIME_DIR
#define MODULE_SEARCH_ENV_VAR
#define MODULE_SEARCH_LAUNCH_ARG

#if !(defined(MODULE_SEARCH_ENV_VAR) || defined(MODULE_SEARCH_RUNTIME_DIR) ||  \
      defined(MODULE_SEARCH_LAUNCH_ARG))
#error Must enable atleast one module search path
#endif
