
# Enable GNU STL
APP_STL := gnustl_static

# Enable RTTI (Needed for dynamic_cast)
APP_CPPFLAGS += -frtti -std=c++11

APP_ABI := armeabi armeabi-v7a x86
NDK_TOOLCHAIN_VERSION=4.8

APP_PLATFORM := android-20
APP_OPTIM := debug
