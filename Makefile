PROJECT_NAME := esp8266-rtos-ota

# Set directory path
# Searched for *.c, *.cpp, *.S
COMPONENT_SRCDIRS := main

# Include ESP-IDF build settings
include $(IDF_PATH)/make/project.mk


# Debug the make process, with $ make [arg]
#  V=1 		 : echo all commands executed, and also each directory as it is entered for a sub-make
#  -w  		 : echo each directory as it is entered for a sub-make
#  --trace : print out every target as its built, and the dependency which caused it to be built
#  -p			 : prints a (very verbose) summary of every generated target in each makefile
