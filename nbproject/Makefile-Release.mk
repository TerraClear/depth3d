#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=None-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/e1dda48/appsettings.o \
	${OBJECTDIR}/_ext/e1dda48/basicserial.o \
	${OBJECTDIR}/_ext/e1dda48/camera_async.o \
	${OBJECTDIR}/_ext/e1dda48/camera_base.o \
	${OBJECTDIR}/_ext/e1dda48/camera_depth.o \
	${OBJECTDIR}/_ext/e1dda48/camera_depth_realsense.o \
	${OBJECTDIR}/_ext/e1dda48/camera_depth_zed.o \
	${OBJECTDIR}/_ext/e1dda48/camera_file.o \
	${OBJECTDIR}/_ext/e1dda48/camera_usb.o \
	${OBJECTDIR}/_ext/e1dda48/detector_base.o \
	${OBJECTDIR}/_ext/e1dda48/detector_hsv.o \
	${OBJECTDIR}/_ext/e1dda48/error_base.o \
	${OBJECTDIR}/_ext/e1dda48/filetools.o \
	${OBJECTDIR}/_ext/e1dda48/hsvcalibration.o \
	${OBJECTDIR}/_ext/e1dda48/msgserver.o \
	${OBJECTDIR}/_ext/e1dda48/navmath.o \
	${OBJECTDIR}/_ext/e1dda48/stopwatch.o \
	${OBJECTDIR}/_ext/e1dda48/thread_base.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/depth3d

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/depth3d: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/depth3d ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/e1dda48/appsettings.o: ../libterraclear/src/appsettings.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/appsettings.o ../libterraclear/src/appsettings.cpp

${OBJECTDIR}/_ext/e1dda48/basicserial.o: ../libterraclear/src/basicserial.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/basicserial.o ../libterraclear/src/basicserial.cpp

${OBJECTDIR}/_ext/e1dda48/camera_async.o: ../libterraclear/src/camera_async.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_async.o ../libterraclear/src/camera_async.cpp

${OBJECTDIR}/_ext/e1dda48/camera_base.o: ../libterraclear/src/camera_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_base.o ../libterraclear/src/camera_base.cpp

${OBJECTDIR}/_ext/e1dda48/camera_depth.o: ../libterraclear/src/camera_depth.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_depth.o ../libterraclear/src/camera_depth.cpp

${OBJECTDIR}/_ext/e1dda48/camera_depth_realsense.o: ../libterraclear/src/camera_depth_realsense.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_depth_realsense.o ../libterraclear/src/camera_depth_realsense.cpp

${OBJECTDIR}/_ext/e1dda48/camera_depth_zed.o: ../libterraclear/src/camera_depth_zed.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_depth_zed.o ../libterraclear/src/camera_depth_zed.cpp

${OBJECTDIR}/_ext/e1dda48/camera_file.o: ../libterraclear/src/camera_file.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_file.o ../libterraclear/src/camera_file.cpp

${OBJECTDIR}/_ext/e1dda48/camera_usb.o: ../libterraclear/src/camera_usb.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_usb.o ../libterraclear/src/camera_usb.cpp

${OBJECTDIR}/_ext/e1dda48/detector_base.o: ../libterraclear/src/detector_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/detector_base.o ../libterraclear/src/detector_base.cpp

${OBJECTDIR}/_ext/e1dda48/detector_hsv.o: ../libterraclear/src/detector_hsv.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/detector_hsv.o ../libterraclear/src/detector_hsv.cpp

${OBJECTDIR}/_ext/e1dda48/error_base.o: ../libterraclear/src/error_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/error_base.o ../libterraclear/src/error_base.cpp

${OBJECTDIR}/_ext/e1dda48/filetools.o: ../libterraclear/src/filetools.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/filetools.o ../libterraclear/src/filetools.cpp

${OBJECTDIR}/_ext/e1dda48/hsvcalibration.o: ../libterraclear/src/hsvcalibration.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/hsvcalibration.o ../libterraclear/src/hsvcalibration.cpp

${OBJECTDIR}/_ext/e1dda48/msgserver.o: ../libterraclear/src/msgserver.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/msgserver.o ../libterraclear/src/msgserver.cpp

${OBJECTDIR}/_ext/e1dda48/navmath.o: ../libterraclear/src/navmath.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/navmath.o ../libterraclear/src/navmath.cpp

${OBJECTDIR}/_ext/e1dda48/stopwatch.o: ../libterraclear/src/stopwatch.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/stopwatch.o ../libterraclear/src/stopwatch.cpp

${OBJECTDIR}/_ext/e1dda48/thread_base.o: ../libterraclear/src/thread_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/thread_base.o ../libterraclear/src/thread_base.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
