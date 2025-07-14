function(target_set_warnings TARGET ENABLE WARNINGS_AS_ERROR)
	if (NOT ${ENABLE})
		message(STATUS "[c] Target [${TARGET}] has no warning flags.")
		return()
	endif()

	set(MSVC_FLAGS
		"/utf-8"
		"/W4"
		"/permissive-"
	)
	set(CLANG_FLAGS
		"-Wall"
		"-Wextra"
		"-Wpedantic"
	)
	set(GNU_FLAGS ${CLANG_FLAGS})

	if(${WARNINGS_AS_ERROR})
		set(MSVC_FLAGS  ${MSVC_FLAGS}	"/WX" 		)
		set(CLANG_FLAGS ${CLANG_FLAGS}	"-Werror"	)
		set(GNU_FLAGS   ${GNU_FLAGS}	"-Werror"	)
	endif()

	if(CMAKE_CXX_COMPILER_ID)
		set(XID ${CMAKE_CXX_COMPILER_ID})
	elseif (CMAKE_C_COMPILER_ID)
		set(XID ${CMAKE_C_COMPILER_ID})
	else()
		message(WARNING "Neither CMAKE_CXX_COMPILER_ID nor CMAKE_C_COMPILER_ID is set.")
		return()
	endif()

	set (XFLAGS "")
	if (XID MATCHES "MSVC")
		set(XFLAGS ${MSVC_FLAGS})
	elseif (XID MATCHES "Clang")
		set(XFLAGS ${CLANG_FLAGS})
	elseif (XID MATCHES "GNU")
		set(XFLAGS ${GNU_FLAGS})
	endif()

	target_compile_options(${TARGET} PRIVATE ${XFLAGS})
	message(STATUS "[c] Target [${TARGET}] warning flags: [${XFLAGS}]")

endfunction(target_set_warnings)
