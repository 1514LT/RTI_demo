# 定义两个目标名称
set(PUB_TARGET_NAME "pub")
set(SUB_TARGET_NAME "sub")

# 设置RTI静态库路径
set(RTI_LIB_PATH "${CMAKE_SOURCE_DIR}/thirdparty/rti_dds_micro/lib")

# 公共源文件（DataType相关文件）
set(common_files
  "${CMAKE_CURRENT_SOURCE_DIR}/DataType.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/DataTypeSupport.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/DataTypePlugin.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/DataTypeApplication.c"
)

# 检查公共文件是否存在
foreach(file ${common_files})
  if(NOT EXISTS ${file})
    message(WARNING "File ${file} not found")
  endif()
endforeach()

# 发布者源文件
set(pub_files
  "${CMAKE_CURRENT_SOURCE_DIR}/pub.c"
  ${common_files}
)

# 订阅者源文件
set(sub_files
  "${CMAKE_CURRENT_SOURCE_DIR}/sub.c"
  ${common_files}
)

# 检查文件是否存在，如果不存在则从GLOB获取
foreach(file ${pub_files} ${sub_files})
  if(NOT EXISTS ${file})
    message(WARNING "File ${file} not found, will use GLOB fallback")
    set(use_glob TRUE)
  endif()
endforeach()

# 如果某些文件不存在，使用GLOB作为后备方案
if(use_glob)
  file(GLOB_RECURSE all_src_files
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
  )
  if(WIN32)
    list(FILTER all_src_files EXCLUDE REGEX ".*_unix.cc$"   )
  else()
    list(FILTER all_src_files EXCLUDE REGEX ".*_win.cc$"    )
  endif()
  
  # 分离pub和sub文件
  set(pub_files ${all_src_files})
  set(sub_files ${all_src_files})
endif()

# 创建发布者可执行文件
add_executable(${PUB_TARGET_NAME}
  ${pub_files}
)
target_set_warnings(${PUB_TARGET_NAME} true false)
target_include_directories(${PUB_TARGET_NAME} PRIVATE)
target_compile_definitions(${PUB_TARGET_NAME} PRIVATE)
target_link_directories(${PUB_TARGET_NAME} PRIVATE ${RTI_LIB_PATH})
target_link_libraries(${PUB_TARGET_NAME} PRIVATE     
    ${RTI_LIB_PATH}/librti_me_discdpdez.a
    ${RTI_LIB_PATH}/librti_me_discdpsez.a
    ${RTI_LIB_PATH}/librti_me_rhsmz.a
    ${RTI_LIB_PATH}/librti_me_whsmz.a
    ${RTI_LIB_PATH}/librti_mez.a
    pthread
)
target_compile_options(${PUB_TARGET_NAME} PRIVATE)

# 创建订阅者可执行文件
add_executable(${SUB_TARGET_NAME}
  ${sub_files}
)
target_set_warnings(${SUB_TARGET_NAME} true false)
target_include_directories(${SUB_TARGET_NAME} PRIVATE)
target_compile_definitions(${SUB_TARGET_NAME} PRIVATE)
target_link_directories(${SUB_TARGET_NAME} PRIVATE ${RTI_LIB_PATH})
target_link_libraries(${SUB_TARGET_NAME} PRIVATE     
    ${RTI_LIB_PATH}/librti_me_discdpdez.a
    ${RTI_LIB_PATH}/librti_me_discdpsez.a
    ${RTI_LIB_PATH}/librti_me_rhsmz.a
    ${RTI_LIB_PATH}/librti_me_whsmz.a
    ${RTI_LIB_PATH}/librti_mez.a
    pthread
)
target_compile_options(${SUB_TARGET_NAME} PRIVATE)

# 为两个目标都添加调试定义
if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo) 
  target_compile_definitions(${PUB_TARGET_NAME} PRIVATE
    -D_XDEBUG
  )
  target_compile_definitions(${SUB_TARGET_NAME} PRIVATE
    -D_XDEBUG
  )
endif()

# 安装文件
file(GLOB_RECURSE dll_files
  "${CMAKE_SOURCE_DIR}/thirdparty/*.dll"
  "${CMAKE_SOURCE_DIR}/thirdparty/*.pdb"
)
install(FILES    ${dll_files}          DESTINATION bin)
install(DIRECTORY  "${CMAKE_SOURCE_DIR}/doc"      DESTINATION bin)