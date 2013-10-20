LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../Classes/*.cpp)

LOCAL_SRC_FILES :=piktomir/main.cpp

LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
					 $(LOCAL_PATH)/../../../libs/cocos2dx/platform/third_party/android/prebuilt/libxml2/include \
					 $(LOCAL_PATH)/../../../libs/extensions \
					 $(LOCAL_PATH)/../../../libs/CocosDenshion

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static
LOCAL_STATIC_LIBRARIES := png_static_prebuilt
LOCAL_STATIC_LIBRARIES += xml2_static_prebuilt
LOCAL_STATIC_LIBRARIES += jpeg_static_prebuilt
LOCAL_STATIC_LIBRARIES += curl_static_prebuilt
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libxml2)