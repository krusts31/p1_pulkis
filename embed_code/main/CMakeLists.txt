cmake_minimum_required(VERSION 3.5)

# This is the project name
project(p1_C)

# Include the components required to run the application
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)

# Register the components needed for the project
#idf_component_register(SRCS "wifi.c" "ft_strlen.c" "ft_strlcpy.c" "tcp_server.c" "main.c" "info.c" "send_data.c" "read_data.c" "ft_strcrh.c" "ft_strrchr.c" "ft_memset.c" "ft_strnstr.c" "ft_calloc.c" "ft_memset.c" "mqtt.c"
#                    INCLUDE_DIRS "include"
#                    REQUIRES "tcpip_adapter")

#idf_component_register(SRCS "wifi.c" "ft_strlen.c" "ft_strlcpy.c" "tcp_server.c" "main.c" "ft_strcrh.c" "ft_strrchr.c" "ft_memset.c" "ft_strnstr.c" "ft_calloc.c" "ft_memset.c" "mqtt.c"
#                    INCLUDE_DIRS "include"
#                    REQUIRES "tcpip_adapter"
#                    EMBED_FILES "main/ca.crt" "main/client.crt" "main/client.key")

# Embed the binary data for certificates
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/ca.crt" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/client.crt" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/client.key" TEXT)

