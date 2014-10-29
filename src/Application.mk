
# Enable GNU STL
APP_STL := gnustl_static

# Enable RTTI (Needed for dynamic_cast)
APP_CPPFLAGS += -frtti

APP_ABI := armeabi armeabi-v7a x86
