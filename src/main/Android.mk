LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

VBE_PATH := $(LOCAL_PATH)/../VBE

LOCAL_C_INCLUDES := \
	$(VBE_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/src/*.cpp) \
	)

LOCAL_WHOLE_STATIC_LIBRARIES := VBE_static

LOCAL_CFLAGS += -std=c++11
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -lEGL

include $(BUILD_SHARED_LIBRARY)
