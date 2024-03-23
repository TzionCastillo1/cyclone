#
# This is a project Makefile. It is assumed that the directory this Makeifle resides in is a 
# project subdirectory.
#

PROJECT_NAME := icm20608_driver

# This line has to be included into the Makefile
# to include components that are located somewhere
# but not in "component" directory

EXTRA_COMPONENT_DIRS := $(realpath ../..)
EXCLUDE_COMPONENTS := test

include $(IDF_PATH)/make/project.mk
