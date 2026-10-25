execute_process(COMMAND "${PROGRAM}" "${READER}" "${CASE}"
  RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(CASE STREQUAL "valid" OR CASE STREQUAL "weighted")
  if(NOT "${result}" STREQUAL "0")
    message(FATAL_ERROR "Valid input failed: ${result}\n${output}\n${error}")
  endif()
else()
  if("${result}" STREQUAL "0" OR
     NOT error MATCHES "Invalid reader input:" OR
     error MATCHES "AddressSanitizer|runtime error:|UndefinedBehaviorSanitizer")
    message(FATAL_ERROR "Expected clean input rejection: ${result}\n${output}\n${error}")
  endif()
endif()
