include_directories(
  ${MAPLE_ROOT}
  ${MOZJS_INSTALL_DIR}/include
  ${MOZJS_INSTALL_DIR}/system_wrappers
  ${MOZJS_SOURCE_DIR}
  ${MOZJS_SOURCE_DIR}/js/src
  ${MOZJS_SOURCE_DIR}/mfbt/double-conversion
  ${MOZJS_SOURCE_DIR}/intl/icu/source/common
  ${MOZJS_SOURCE_DIR}/intl/icu/source/i18n
  ${MOZJS_SOURCE_DIR}/js/public
)

link_directories(${MOZJS_INSTALL_DIR}/lib)

add_definitions(-DMIR_FEATURE_FULL=1)

add_executable(js2mpl
  closure.cpp
  compiler.cpp
  eh.cpp
  js2mpl.cpp
  jsfunction.cpp
  jsmirbuilder.cpp
  operandstack.cpp
  scope.cpp
  util.cpp
)

get_target_property(TEMP js2mpl COMPILE_FLAGS)
if(TEMP STREQUAL "TEMP-NOTFOUND")
  SET(TEMP "") # set to empty string
else()
  SET(TEMP "${TEMP} ") # a space to cleanly separate from existing content
endif()
# append our values
SET(TEMP "${TEMP}-include ${MOZJS_SOURCE_DIR}/config/gcc_hidden.h -include ${MOZJS_BUILD_DIR}/js/src/js-confdefs.h")
set_target_properties(js2mpl PROPERTIES COMPILE_FLAGS ${TEMP} )

target_link_libraries(js2mpl
  libmplir
  libmempool
  general js_static
  general z
  general pthread
  general dl
)
