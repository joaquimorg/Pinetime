cmake_minimum_required(VERSION 3.10)

project(pinetime-lite C CXX ASM)

# define some variables just for this example to determine file locations
set(NRF_PROJECT_NAME pinetime-lite)
set(NRF_BOARD pca10040)

# check if all the necessary tools paths have been provided.
if (NOT NRF5_SDK_PATH)
    message(FATAL_ERROR "The path to the nRF5 SDK (NRF5_SDK_PATH) must be set.")
endif ()
if (DEFINED ARM_NONE_EABI_TOOLCHAIN_PATH)
    set(ARM_NONE_EABI_TOOLCHAIN_BIN_PATH ${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin)
endif ()

if (NOT NRF_TARGET MATCHES "nrf52")
    message(FATAL_ERROR "Only rRF52 boards are supported right now")
endif ()

# Setup toolchain
include(${CMAKE_SOURCE_DIR}/cmake-nRF5x/arm-gcc-toolchain.cmake)

if (NOT DEFINED ARM_GCC_TOOLCHAIN)
    message(FATAL_ERROR "The toolchain must be set up before calling this macro")
endif ()
set(CMAKE_OSX_SYSROOT "/")
set(CMAKE_OSX_DEPLOYMENT_TARGET "")


set(SDK_SOURCE_FILES
        # Startup
        ${NRF5_SDK_PATH}/modules/nrfx/mdk/system_nrf52.c
        ${NRF5_SDK_PATH}/modules/nrfx/mdk/gcc_startup_nrf52.S

        # Base SDK
        ${NRF5_SDK_PATH}/components/boards/boards.c
        ${NRF5_SDK_PATH}/integration/nrfx/legacy/nrf_drv_clock.c
        ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_clock.c
        ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_gpiote.c
        ${NRF5_SDK_PATH}/modules/nrfx/soc/nrfx_atomic.c
        ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_saadc.c

        # FreeRTOS
        ${NRF5_SDK_PATH}/external/freertos/source/croutine.c
        ${NRF5_SDK_PATH}/external/freertos/source/event_groups.c
        ${NRF5_SDK_PATH}/external/freertos/source/portable/MemMang/heap_4.c
        ${NRF5_SDK_PATH}/external/freertos/source/list.c
        ${NRF5_SDK_PATH}/external/freertos/source/queue.c
        ${NRF5_SDK_PATH}/external/freertos/source/stream_buffer.c
        ${NRF5_SDK_PATH}/external/freertos/source/tasks.c
        ${NRF5_SDK_PATH}/external/freertos/source/timers.c
        ${NRF5_SDK_PATH}/components/libraries/timer/app_timer_freertos.c

        # Libs
        ${NRF5_SDK_PATH}/components/libraries/atomic/nrf_atomic.c
        ${NRF5_SDK_PATH}/components/libraries/balloc/nrf_balloc.c
        ${NRF5_SDK_PATH}/components/libraries/util/nrf_assert.c
        ${NRF5_SDK_PATH}/components/libraries/util/app_error.c
        ${NRF5_SDK_PATH}/components/libraries/util/app_error_weak.c
        ${NRF5_SDK_PATH}/components/libraries/util/app_error_handler_gcc.c
        ${NRF5_SDK_PATH}/components/libraries/util/app_util_platform.c
        ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_backend_rtt.c
        ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_backend_serial.c
        ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_default_backends.c
        ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_frontend.c
        ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_str_formatter.c
        ${NRF5_SDK_PATH}/components/libraries/memobj/nrf_memobj.c
        ${NRF5_SDK_PATH}/components/libraries/ringbuf/nrf_ringbuf.c
        ${NRF5_SDK_PATH}/components/libraries/strerror/nrf_strerror.c

        # Segger RTT
        ${NRF5_SDK_PATH}/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c
        ${NRF5_SDK_PATH}/external/segger_rtt/SEGGER_RTT.c
        ${NRF5_SDK_PATH}/external/segger_rtt/SEGGER_RTT_printf.c

        # Other
        ${NRF5_SDK_PATH}/external/utf_converter/utf.c
        ${NRF5_SDK_PATH}/external/fprintf/nrf_fprintf.c
        ${NRF5_SDK_PATH}/external/fprintf/nrf_fprintf_format.c

        # TWI
        ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_twim.c
        ${NRF5_SDK_PATH}/integration/nrfx/legacy/nrf_drv_twi.c
        ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_twi.c

        # GPIOTE
        ${NRF5_SDK_PATH}/components/libraries/gpiote/app_gpiote.c
        )

set(TINYCRYPT_SRC
        libs/mynewt-nimble/ext/tinycrypt/src/aes_encrypt.c
        libs/mynewt-nimble/ext/tinycrypt/src/utils.c
        )

set(NIMBLE_SRC
        libs/mynewt-nimble/porting/npl/freertos/src/nimble_port_freertos.c
        libs/mynewt-nimble/porting/npl/freertos/src/npl_os_freertos.c
        libs/mynewt-nimble/nimble/host/src/ble_hs.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_hci_evt.c
        libs/mynewt-nimble/nimble/host/src/ble_l2cap_sig_cmd.c
        libs/mynewt-nimble/nimble/host/src/ble_l2cap_sig.c
        libs/mynewt-nimble/nimble/host/src/ble_l2cap.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_mbuf.c
        libs/mynewt-nimble/nimble/host/src/ble_sm.c
        libs/mynewt-nimble/nimble/host/src/ble_gap.c
        libs/mynewt-nimble/nimble/host/src/ble_gatts.c
        libs/mynewt-nimble/nimble/host/src/ble_gattc.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_conn.c
        libs/mynewt-nimble/nimble/host/src/ble_att_svr.c
        libs/mynewt-nimble/nimble/host/src/ble_store.c
        libs/mynewt-nimble/nimble/host/src/ble_store_util.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_pvcy.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_hci.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_log.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_hci_util.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_hci_cmd.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_cfg.c
        libs/mynewt-nimble/nimble/host/src/ble_uuid.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_id.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_misc.c
        libs/mynewt-nimble/nimble/host/src/ble_att.c
        libs/mynewt-nimble/nimble/host/src/ble_att_clt.c
        libs/mynewt-nimble/nimble/host/src/ble_att_svr.c
        libs/mynewt-nimble/nimble/host/src/ble_att_cmd.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_atomic.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_adv.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_flow.c
        libs/mynewt-nimble/nimble/host/src/ble_sm.c
        libs/mynewt-nimble/nimble/host/src/ble_sm_cmd.c
        libs/mynewt-nimble/nimble/host/src/ble_sm_lgcy.c
        libs/mynewt-nimble/nimble/host/src/ble_sm_alg.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_mqueue.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_stop.c
        libs/mynewt-nimble/nimble/host/src/ble_hs_startup.c
        libs/mynewt-nimble/nimble/host/store/ram/src/ble_store_ram.c
        libs/mynewt-nimble/nimble/host/src/ble_monitor.c
        libs/mynewt-nimble/nimble/transport/ram/src/ble_hci_ram.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_rand.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_conn.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_ctrl.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_hci.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_conn_hci.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_utils.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_scan.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_whitelist.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_adv.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_sched.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_supp_cmd.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_hci_ev.c
        libs/mynewt-nimble/nimble/controller/src/ble_ll_rfmgmt.c
        libs/mynewt-nimble/porting/nimble/src/os_cputime.c
        libs/mynewt-nimble/porting/nimble/src/os_cputime_pwr2.c
        libs/mynewt-nimble/porting/nimble/src/os_mbuf.c
        libs/mynewt-nimble/porting/nimble/src/os_mempool.c
        libs/mynewt-nimble/porting/nimble/src/hal_timer.c
        libs/mynewt-nimble/porting/nimble/src/mem.c
        libs/mynewt-nimble/porting/nimble/src/endian.c
        libs/mynewt-nimble/porting/nimble/src/os_msys_init.c
        libs/mynewt-nimble/nimble/drivers/nrf52/src/ble_hw.c
        libs/mynewt-nimble/nimble/drivers/nrf52/src/ble_phy.c
        libs/mynewt-nimble/nimble/host/services/gap/src/ble_svc_gap.c
        libs/mynewt-nimble/nimble/host/services/gatt/src/ble_svc_gatt.c
        libs/mynewt-nimble/nimble/host/util/src/addr.c
        )

        set(LVGL_SRC
        libs/lv_conf.h
        libs/lvgl/lvgl.h
        
        libs/lvgl/src/lvgl.h
        libs/lvgl/src/lv_api_map.h
        libs/lvgl/src/lv_conf_internal.h

        libs/lvgl/src/core/lv_disp.h
        libs/lvgl/src/core/lv_group.h
        libs/lvgl/src/core/lv_indev.h
        libs/lvgl/src/core/lv_indev_scroll.h
        libs/lvgl/src/core/lv_obj.h
        libs/lvgl/src/core/lv_obj_class.h
        libs/lvgl/src/core/lv_obj_draw.h
        libs/lvgl/src/core/lv_obj_pos.h
        libs/lvgl/src/core/lv_obj_scroll.h
        libs/lvgl/src/core/lv_obj_style.h
        libs/lvgl/src/core/lv_obj_tree.h
        libs/lvgl/src/core/lv_refr.h
        libs/lvgl/src/core/lv_theme.h
        libs/lvgl/src/core/lv_disp.c
        libs/lvgl/src/core/lv_group.c
        libs/lvgl/src/core/lv_indev.c
        libs/lvgl/src/core/lv_indev_scroll.c
        libs/lvgl/src/core/lv_obj.c
        libs/lvgl/src/core/lv_obj_class.c
        libs/lvgl/src/core/lv_obj_draw.c
        libs/lvgl/src/core/lv_obj_pos.c
        libs/lvgl/src/core/lv_obj_scroll.c
        libs/lvgl/src/core/lv_obj_style.c
        libs/lvgl/src/core/lv_obj_tree.c
        libs/lvgl/src/core/lv_refr.c
        libs/lvgl/src/core/lv_theme.c

        libs/lvgl/src/draw/lv_draw.h
        libs/lvgl/src/draw/lv_draw_arc.h
        libs/lvgl/src/draw/lv_draw_blend.h
        libs/lvgl/src/draw/lv_draw_img.h
        libs/lvgl/src/draw/lv_draw_label.h
        libs/lvgl/src/draw/lv_draw_line.h
        libs/lvgl/src/draw/lv_draw_mask.h
        libs/lvgl/src/draw/lv_draw_rect.h
        libs/lvgl/src/draw/lv_draw_triangle.h
        libs/lvgl/src/draw/lv_img_buf.h
        libs/lvgl/src/draw/lv_img_cache.h
        libs/lvgl/src/draw/lv_img_decoder.h
        libs/lvgl/src/draw/lv_draw_arc.c
        libs/lvgl/src/draw/lv_draw_blend.c
        libs/lvgl/src/draw/lv_draw_img.c
        libs/lvgl/src/draw/lv_draw_label.c
        libs/lvgl/src/draw/lv_draw_line.c
        libs/lvgl/src/draw/lv_draw_mask.c
        libs/lvgl/src/draw/lv_draw_rect.c
        libs/lvgl/src/draw/lv_draw_triangle.c
        libs/lvgl/src/draw/lv_img_buf.c
        libs/lvgl/src/draw/lv_img_cache.c
        libs/lvgl/src/draw/lv_img_decoder.c

        libs/lvgl/src/font/lv_font.h
        libs/lvgl/src/font/lv_font_fmt_txt.h
        libs/lvgl/src/font/lv_font_loader.h
        libs/lvgl/src/font/lv_font.c
        libs/lvgl/src/font/lv_font_fmt_txt.c
        libs/lvgl/src/font/lv_font_loader.c

        libs/lvgl/src/hal/lv_hal.h
        libs/lvgl/src/hal/lv_hal_disp.h
        libs/lvgl/src/hal/lv_hal_indev.h
        libs/lvgl/src/hal/lv_hal_tick.h        
        libs/lvgl/src/hal/lv_hal_disp.c
        libs/lvgl/src/hal/lv_hal_indev.c
        libs/lvgl/src/hal/lv_hal_tick.c

        libs/lvgl/src/misc/lv_anim.h
        libs/lvgl/src/misc/lv_area.h
        libs/lvgl/src/misc/lv_async.h
        libs/lvgl/src/misc/lv_bidi.h
        libs/lvgl/src/misc/lv_color.h
        libs/lvgl/src/misc/lv_fs.h
        libs/lvgl/src/misc/lv_gc.h
        libs/lvgl/src/misc/lv_ll.h
        libs/lvgl/src/misc/lv_log.h
        libs/lvgl/src/misc/lv_math.h
        libs/lvgl/src/misc/lv_mem.h
        libs/lvgl/src/misc/lv_printf.h
        libs/lvgl/src/misc/lv_style.h
        libs/lvgl/src/misc/lv_timer.h
        libs/lvgl/src/misc/lv_tlsf.h
        libs/lvgl/src/misc/lv_txt.h
        libs/lvgl/src/misc/lv_txt_ap.h
        libs/lvgl/src/misc/lv_utils.h
        libs/lvgl/src/misc/lv_anim.c
        libs/lvgl/src/misc/lv_area.c
        libs/lvgl/src/misc/lv_async.c
        libs/lvgl/src/misc/lv_bidi.c
        libs/lvgl/src/misc/lv_color.c
        libs/lvgl/src/misc/lv_fs.c
        libs/lvgl/src/misc/lv_gc.c
        libs/lvgl/src/misc/lv_ll.c
        libs/lvgl/src/misc/lv_log.c
        libs/lvgl/src/misc/lv_math.c
        libs/lvgl/src/misc/lv_mem.c
        libs/lvgl/src/misc/lv_printf.c
        libs/lvgl/src/misc/lv_style.c
        libs/lvgl/src/misc/lv_timer.c
        libs/lvgl/src/misc/lv_tlsf.c
        libs/lvgl/src/misc/lv_txt.c
        libs/lvgl/src/misc/lv_txt_ap.c
        libs/lvgl/src/misc/lv_utils.c
        
        libs/lvgl/src/extra/themes/lv_themes.h
        libs/lvgl/src/extra/themes/default/lv_theme_default.h
        libs/lvgl/src/extra/themes/default/lv_theme_default.c

        libs/lvgl/src/widgets/lv_arc.h
        libs/lvgl/src/widgets/lv_bar.h
        libs/lvgl/src/widgets/lv_btn.h
        libs/lvgl/src/widgets/lv_btnmatrix.h
        libs/lvgl/src/widgets/lv_canvas.h
        libs/lvgl/src/widgets/lv_chart.h
        libs/lvgl/src/widgets/lv_checkbox.h
        libs/lvgl/src/widgets/lv_dropdown.h
        libs/lvgl/src/widgets/lv_img.h
        libs/lvgl/src/widgets/lv_label.h
        libs/lvgl/src/widgets/lv_line.h
        libs/lvgl/src/widgets/lv_meter.h
        libs/lvgl/src/widgets/lv_roller.h
        libs/lvgl/src/widgets/lv_slider.h
        libs/lvgl/src/widgets/lv_switch.h
        libs/lvgl/src/widgets/lv_table.h
        libs/lvgl/src/widgets/lv_textarea.h
        libs/lvgl/src/widgets/lv_arc.c
        libs/lvgl/src/widgets/lv_bar.c
        libs/lvgl/src/widgets/lv_btn.c
        libs/lvgl/src/widgets/lv_btnmatrix.c
        libs/lvgl/src/widgets/lv_canvas.c
        libs/lvgl/src/widgets/lv_chart.c
        libs/lvgl/src/widgets/lv_checkbox.c
        libs/lvgl/src/widgets/lv_dropdown.c
        libs/lvgl/src/widgets/lv_img.c
        libs/lvgl/src/widgets/lv_label.c
        libs/lvgl/src/widgets/lv_line.c
        libs/lvgl/src/widgets/lv_meter.c
        libs/lvgl/src/widgets/lv_roller.c
        libs/lvgl/src/widgets/lv_slider.c
        libs/lvgl/src/widgets/lv_switch.c
        libs/lvgl/src/widgets/lv_table.c
        libs/lvgl/src/widgets/lv_textarea.c

        libs/lvgl/src/extra/layouts/lv_layouts.h
        libs/lvgl/src/extra/layouts/flex/lv_flex.h
        libs/lvgl/src/extra/layouts/flex/lv_flex.c
        libs/lvgl/src/extra/layouts/grid/lv_grid.h
        libs/lvgl/src/extra/layouts/grid/lv_grid.c

        libs/lvgl/src/extra/widgets/lv_widgets.h

        libs/lvgl/src/extra/widgets/calendar/lv_calendar.h
        libs/lvgl/src/extra/widgets/calendar/lv_calendar_header_arrow.h
        libs/lvgl/src/extra/widgets/calendar/lv_calendar_header_dropdown.h
        libs/lvgl/src/extra/widgets/calendar/lv_calendar.c
        libs/lvgl/src/extra/widgets/calendar/lv_calendar_header_arrow.c
        libs/lvgl/src/extra/widgets/calendar/lv_calendar_header_dropdown.c

        libs/lvgl/src/extra/widgets/keyboard/lv_keyboard.h
        libs/lvgl/src/extra/widgets/keyboard/lv_keyboard.c

        libs/lvgl/src/extra/widgets/list/lv_list.h
        libs/lvgl/src/extra/widgets/list/lv_list.c

        libs/lvgl/src/extra/widgets/msgbox/lv_msgbox.h
        libs/lvgl/src/extra/widgets/msgbox/lv_msgbox.c

        libs/lvgl/src/extra/widgets/spinbox/lv_spinbox.h
        libs/lvgl/src/extra/widgets/spinbox/lv_spinbox.c

        libs/lvgl/src/extra/widgets/spinner/lv_spinner.h
        libs/lvgl/src/extra/widgets/spinner/lv_spinner.c

        libs/lvgl/src/extra/widgets/tabview/lv_tabview.h
        libs/lvgl/src/extra/widgets/tabview/lv_tabview.c

        libs/lvgl/src/extra/widgets/tileview/lv_tileview.h
        libs/lvgl/src/extra/widgets/tileview/lv_tileview.c

        libs/lvgl/src/extra/widgets/win/lv_win.h
        libs/lvgl/src/extra/widgets/win/lv_win.c

        libs/lvgl/src/extra/widgets/colorwheel/lv_colorwheel.h
        libs/lvgl/src/extra/widgets/colorwheel/lv_colorwheel.c

        libs/lvgl/src/extra/widgets/led/lv_led.h
        libs/lvgl/src/extra/widgets/led/lv_led.c

        libs/lvgl/src/extra/widgets/imgbtn/lv_imgbtn.h
        libs/lvgl/src/extra/widgets/imgbtn/lv_imgbtn.c
        
        )

set(BMA421_SRC
        libs/bma421/bma4.c
        libs/bma421/bma421.c
        )

        
#set(HRS3300_SRC
#        libs/hrs3300/hrs3300_reg_init.h
#        libs/hrs3300/hrs3300.h
#        libs/hrs3300/hrs3300.c
        #libs/hrs3300/HRS3300lib.h
        #libs/hrs3300/HRS3300lib.cpp
#        )
        

list(APPEND SOURCE_FILES
        BootloaderVersion.cpp
        logging/NrfLogger.cpp

        
        drivers/St7789.cpp
        drivers/SpiNorFlash.cpp
        drivers/SpiMaster.cpp
        drivers/Spi.cpp
        drivers/Watchdog.cpp
        drivers/DebugPins.cpp
        drivers/InternalFlash.cpp
        drivers/Cst816s.cpp
        drivers/TwiMaster.cpp

        components/motor/Motor.cpp
        components/settings/Settings.cpp
        components/battery/BatteryController.cpp
        components/datetime/DateTimeController.cpp
        components/brightness/BrightnessController.cpp
        components/firmwarevalidator/FirmwareValidator.cpp
        components/accelerometer/Accelerometer.cpp
        components/fs/FS.cpp

        components/ble/BleController.cpp
        components/ble/NotificationManager.cpp
        components/ble/CallNotificationManager.cpp
        components/ble/NimbleController.cpp
        components/ble/DeviceInformationService.cpp
        components/ble/CurrentTimeClient.cpp
        components/ble/AlertNotificationClient.cpp
        components/ble/DfuService.cpp
        components/ble/CurrentTimeService.cpp
        components/ble/AlertNotificationService.cpp
        components/ble/MusicService.cpp
        components/ble/BatteryInformationService.cpp
        components/ble/ImmediateAlertService.cpp
        components/ble/ServiceDiscovery.cpp
        components/ble/FileService.cpp
        
        FreeRTOS/port.c
        FreeRTOS/port_cmsis_systick.c
        FreeRTOS/port_cmsis.c

        systemtask/SystemTask.cpp

        displayapp/LittleVgl.cpp
        displayapp/lv_pinetime_theme.c

        displayapp/DisplayApp.cpp
        displayapp/screens/ShowMessage.cpp
        displayapp/screens/Screen.cpp
        displayapp/screens/Charging.cpp
        displayapp/screens/LowBatt.cpp
        displayapp/screens/Clock.cpp
        displayapp/screens/Tile.cpp
        displayapp/screens/List.cpp
        displayapp/screens/Steps.cpp
        displayapp/screens/BatteryIcon.cpp
        displayapp/screens/BleIcon.cpp
        displayapp/screens/NotificationIcon.cpp
        displayapp/screens/IncomingCall.cpp
        displayapp/screens/About.cpp
        displayapp/screens/Label.cpp
        displayapp/screens/FirmwareUpdate.cpp
        displayapp/screens/FirmwareValidation.cpp
        displayapp/screens/ApplicationList.cpp
        displayapp/screens/Notifications.cpp
        displayapp/screens/FlashLight.cpp
        displayapp/screens/WeatherToday.cpp
        displayapp/screens/WeatherForecast.cpp
        displayapp/screens/Motion.cpp

        displayapp/screens/settings/QuickSettings.cpp   
        displayapp/screens/settings/Settings.cpp
        displayapp/screens/settings/SettingDisplay.cpp
        displayapp/screens/settings/SettingTimeFormat.cpp
        displayapp/screens/settings/SettingWatchFace.cpp
        displayapp/screens/settings/SettingSteps.cpp
        displayapp/screens/settings/SettingWakeUp.cpp

        #Watch Faces
        displayapp/screens/WatchFaceDigital.cpp
        displayapp/screens/WatchFaceAnalog.cpp
        displayapp/screens/WatchFaceMinimal.cpp
        displayapp/screens/WatchFaceCustom.cpp
        displayapp/screens/WatchFacePong.cpp
        displayapp/screens/WatchFaceWeather.cpp

        #Fonts
        displayapp/fonts/lv_font_roboto_20.c
        displayapp/fonts/lv_font_sys_20.c
        displayapp/fonts/lv_font_sys_48.c
        displayapp/fonts/clock_font/lv_font_clock_90.c
        displayapp/fonts/clock_font/lv_font_clock_76.c
        displayapp/fonts/clock_font/lv_font_clock_42.c

        main.cpp
        
        )


set(INCLUDE_FILES
        BootloaderVersion.h
        logging/Logger.h
        logging/NrfLogger.h

        drivers/TwiMaster.h
        drivers/St7789.h        
        drivers/SpiNorFlash.h
        drivers/SpiMaster.h
        drivers/Spi.h
        drivers/Watchdog.h
        drivers/DebugPins.h
        drivers/InternalFlash.h
        drivers/Cst816s.h
        components/settings/Settings.h
        components/battery/BatteryController.h
        components/ble/BleController.h
        components/ble/NotificationManager.h
        components/datetime/DateTimeController.h
        components/brightness/BrightnessController.h
        components/ble/NimbleController.h
        components/ble/DeviceInformationService.h
        components/ble/CurrentTimeClient.h
        components/ble/AlertNotificationClient.h
        components/ble/DfuService.h
        components/firmwarevalidator/FirmwareValidator.h
        components/ble/BatteryInformationService.h
        components/ble/ImmediateAlertService.h
        components/ble/ServiceDiscovery.h
        components/ble/BleClient.h

        FreeRTOS/portmacro.h
        FreeRTOS/portmacro_cmsis.h
        libs/date/includes/date/tz.h
        libs/date/includes/date/chrono_io.h
        libs/date/includes/date/date.h
        libs/date/includes/date/islamic.h
        libs/date/includes/date/iso_week.h
        libs/date/includes/date/julian.h
        libs/date/includes/date/ptz.h
        libs/date/includes/date/tz_private.h
        displayapp/LittleVgl.h        
        displayapp/screens/Symbols.h        
        displayapp/lv_pinetime_theme.h

        systemtask/SystemTask.h
        systemtask/SystemMonitor.h

        displayapp/DisplayApp.h
        displayapp/TouchEvents.h
        displayapp/screens/Screen.h
        displayapp/screens/Clock.h
        displayapp/screens/Tile.h
        displayapp/screens/BatteryIcon.h
        displayapp/screens/BleIcon.h
        displayapp/screens/NotificationIcon.h
        displayapp/screens/About.h
        displayapp/screens/ScreenList.h
        displayapp/screens/Label.h
        displayapp/screens/FirmwareUpdate.h
        displayapp/screens/FirmwareValidation.h
        displayapp/screens/ApplicationList.h
        displayapp/Apps.h
        displayapp/screens/Notifications.h
        
        #Watch Faces
        displayapp/screens/WatchFaceDigital.h
        displayapp/screens/WatchFaceAnalog.h
        displayapp/screens/WatchFaceMinimal.h
        displayapp/screens/WatchFaceCustom.h
        displayapp/screens/WatchFacePong.h        
        )

include_directories(
        .
        ../
        FreeRTOS/
        libs/
        libs/date/includes
        libs/mynewt-nimble/porting/npl/freertos/include
        libs/mynewt-nimble/nimble/include
        libs/mynewt-nimble/porting/nimble/include
        libs/mynewt-nimble/nimble/host/include
        libs/mynewt-nimble/nimble/controller/include
        libs/mynewt-nimble/nimble/transport/ram/include
        libs/mynewt-nimble/nimble/drivers/nrf52/include
        libs/mynewt-nimble/ext/tinycrypt/include
        libs/mynewt-nimble/nimble/host/services/gap/include
        libs/mynewt-nimble/nimble/host/services/gatt/include
        libs/mynewt-nimble/nimble/host/util/include
        libs/mynewt-nimble/nimble/host/store/ram/include

        ${NRF5_SDK_PATH}/components/drivers_nrf/nrf_soc_nosd
        ${NRF5_SDK_PATH}/components
        ${NRF5_SDK_PATH}/components/boards
        ${NRF5_SDK_PATH}/components/softdevice/common
        ${NRF5_SDK_PATH}/integration/nrfx
        ${NRF5_SDK_PATH}/integration/nrfx/legacy
        ${NRF5_SDK_PATH}/modules/nrfx
        ${NRF5_SDK_PATH}/modules/nrfx/drivers/include
        ${NRF5_SDK_PATH}/modules/nrfx/hal
        ${NRF5_SDK_PATH}/modules/nrfx/mdk
        ${NRF5_SDK_PATH}/external/freertos/source/include
        ${NRF5_SDK_PATH}/components/toolchain/cmsis/include
        ${NRF5_SDK_PATH}/components/libraries/atomic
        ${NRF5_SDK_PATH}/components/libraries/atomic_fifo
        ${NRF5_SDK_PATH}/components/libraries/atomic_flags
        ${NRF5_SDK_PATH}/components/libraries/balloc
        ${NRF5_SDK_PATH}/components/libraries/bootloader/ble_dfu
        ${NRF5_SDK_PATH}/components/libraries/cli
        ${NRF5_SDK_PATH}/components/libraries/crc16
        ${NRF5_SDK_PATH}/components/libraries/crc32
        ${NRF5_SDK_PATH}/components/libraries/crypto
        ${NRF5_SDK_PATH}/components/libraries/csense
        ${NRF5_SDK_PATH}/components/libraries/csense_drv
        ${NRF5_SDK_PATH}/components/libraries/delay
        ${NRF5_SDK_PATH}/components/libraries/ecc
        ${NRF5_SDK_PATH}/components/libraries/experimental_section_vars
        ${NRF5_SDK_PATH}/components/libraries/experimental_task_manager
        ${NRF5_SDK_PATH}/components/libraries/fds
        ${NRF5_SDK_PATH}/components/libraries/fstorage
        ${NRF5_SDK_PATH}/components/libraries/gfx
        ${NRF5_SDK_PATH}/components/libraries/gpiote
        ${NRF5_SDK_PATH}/components/libraries/hardfault
        ${NRF5_SDK_PATH}/components/libraries/hci
        ${NRF5_SDK_PATH}/components/libraries/led_softblink
        ${NRF5_SDK_PATH}/components/libraries/log
        ${NRF5_SDK_PATH}/components/libraries/log/src
        ${NRF5_SDK_PATH}/components/libraries/low_power_pwm
        ${NRF5_SDK_PATH}/components/libraries/mem_manager
        ${NRF5_SDK_PATH}/components/libraries/memobj
        ${NRF5_SDK_PATH}/components/libraries/mpu
        ${NRF5_SDK_PATH}/components/libraries/mutex
        ${NRF5_SDK_PATH}/components/libraries/pwm
        ${NRF5_SDK_PATH}/components/libraries/pwr_mgmt
        ${NRF5_SDK_PATH}/components/libraries/queue
        ${NRF5_SDK_PATH}/components/libraries/ringbuf
        ${NRF5_SDK_PATH}/components/libraries/scheduler
        ${NRF5_SDK_PATH}/components/libraries/sdcard
        ${NRF5_SDK_PATH}/components/libraries/slip
        ${NRF5_SDK_PATH}/components/libraries/sortlist
        ${NRF5_SDK_PATH}/components/libraries/spi_mngr
        ${NRF5_SDK_PATH}/components/libraries/stack_guard
        ${NRF5_SDK_PATH}/components/libraries/strerror
        ${NRF5_SDK_PATH}/components/libraries/svc
        ${NRF5_SDK_PATH}/components/libraries/timer
        ${NRF5_SDK_PATH}/components/libraries/twi_mngr
        ${NRF5_SDK_PATH}/components/libraries/usbd
        ${NRF5_SDK_PATH}/components/libraries/usbd/class/audio
        ${NRF5_SDK_PATH}/components/libraries/usbd/class/cdc
        ${NRF5_SDK_PATH}/components/libraries/usbd/class/cdc/acm
        ${NRF5_SDK_PATH}/components/libraries/usbd/class/hid
        ${NRF5_SDK_PATH}/components/libraries/usbd/class/hid/generic
        ${NRF5_SDK_PATH}/components/libraries/usbd/class/hid/kbd
        ${NRF5_SDK_PATH}/components/libraries/usbd/class/hid/mouse
        ${NRF5_SDK_PATH}/components/libraries/usbd/class/msc
        ${NRF5_SDK_PATH}/components/libraries/util
        ${NRF5_SDK_PATH}/external/segger_rtt/
        ${NRF5_SDK_PATH}/external/fprintf/
        ${NRF5_SDK_PATH}/external/thedotfactory_fonts
        ${NRF5_SDK_PATH}/components/libraries/gpiote

)

link_directories(
        ../
)

set(COMMON_FLAGS -MP -MD -mthumb -mabi=aapcs -Wall -Wno-unknown-pragmas -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wreturn-type -Werror=return-type)
#add_definitions(-DCONFIG_GPIO_AS_PINRESET)
#add_definitions(-DDEBUG)
add_definitions(-DNIMBLE_CFG_CONTROLLER)
add_definitions(-DOS_CPUTIME_FREQ)
add_definitions(-DNRF52 -DNRF52832 -DNRF52832_XXAA -DNRF52_PAN_74 -DNRF52_PAN_64 -DNRF52_PAN_12 -DNRF52_PAN_58 -DNRF52_PAN_54 -DNRF52_PAN_31 -DNRF52_PAN_51 -DNRF52_PAN_36 -DNRF52_PAN_15 -DNRF52_PAN_20 -DNRF52_PAN_55 -DBOARD_PCA10040)
add_definitions(-DFREERTOS)
#add_definitions(-DDEBUG_NRF_USER)
add_definitions(-D__STACK_SIZE=0x2000)
add_definitions(-D__HEAP_SIZE=0x2000)

# Release
#set(COMMON_FLAGS_OPT -Os)

# Debug
set(COMMON_FLAGS_OPT -g3)

if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
endif ()

# NRF SDK
add_library(nrf-sdk STATIC ${SDK_SOURCE_FILES})
target_include_directories(nrf-sdk SYSTEM PUBLIC . ../)
target_include_directories(nrf-sdk SYSTEM PUBLIC ${INCLUDES_FROM_LIBS})
target_compile_options(nrf-sdk PRIVATE
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )

# NimBLE
add_library(nimble STATIC ${NIMBLE_SRC} ${TINYCRYPT_SRC})
target_include_directories(nimble SYSTEM PUBLIC . ../)
target_include_directories(nimble SYSTEM PUBLIC ${INCLUDES_FROM_LIBS})
target_compile_options(nimble PRIVATE
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3 -Wno-unused-but-set-variable -Wno-maybe-uninitialized>
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 -Wno-unused-but-set-variable -Wno-maybe-uninitialized ${COMMON_FLAGS_OPT}>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3 -Wno-unused-but-set-variable -Wno-maybe-uninitialized>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 -Wno-unused-but-set-variable -Wno-maybe-uninitialized ${COMMON_FLAGS_OPT}>
        $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )

# lvgl
add_library(lvgl STATIC ${LVGL_SRC})
target_include_directories(lvgl SYSTEM PUBLIC . ../)
target_include_directories(lvgl SYSTEM PUBLIC ${INCLUDES_FROM_LIBS})
target_compile_options(lvgl PRIVATE
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )

  
# BMA421_SRC
add_library(bma421 STATIC ${BMA421_SRC})
target_include_directories(bma421 SYSTEM PUBLIC . ../)
target_include_directories(bma421 SYSTEM PUBLIC ${INCLUDES_FROM_LIBS})
target_compile_options(bma421 PRIVATE
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )

# HRS3300
#add_library(hrs3300lib STATIC IMPORTED)
#set_property(TARGET hrs3300lib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/libs/hrs3300/libheart.a)

#add_library(hrs3300 STATIC ${HRS3300_SRC})
#set_property(TARGET hrs3300 PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/libs/hrs3300/libheart.a)
#target_include_directories(hrs3300 SYSTEM PUBLIC . ../)
#target_include_directories(hrs3300 SYSTEM PUBLIC ${INCLUDES_FROM_LIBS})
#target_compile_options(hrs3300 PRIVATE
#        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
#        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3>
#        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
#        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3>
#        $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
#        )


# Build autonomous binary (without support for bootloader)
set(EXECUTABLE_NAME "pinetime-lite")
set(EXECUTABLE_FILE_NAME ${EXECUTABLE_NAME}-${pinetimelite_VERSION_MAJOR}.${pinetimelite_VERSION_MINOR}.${pinetimelite_VERSION_PATCH})
set(NRF5_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/gcc_nrf52.ld")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME ${EXECUTABLE_FILE_NAME})
target_link_libraries(${EXECUTABLE_NAME} nimble nrf-sdk lvgl bma421 )
target_compile_options(${EXECUTABLE_NAME} PUBLIC
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        SUFFIX ".out"
        LINK_FLAGS "-mthumb -mabi=aapcs -L ${NRF5_SDK_PATH}/modules/nrfx/mdk -T${NRF5_LINKER_SCRIPT} -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,--gc-sections --specs=nano.specs -lc -lnosys -lm -Wl,-Map=${EXECUTABLE_FILE_NAME}.map"
        )

add_custom_command(TARGET ${EXECUTABLE_NAME}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE_FILE_NAME}.out
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE_FILE_NAME}.out "${EXECUTABLE_FILE_NAME}.bin"
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE_FILE_NAME}.out "${EXECUTABLE_FILE_NAME}.hex"
        COMMENT "post build steps for ${EXECUTABLE_FILE_NAME}")


# Build binary intended to be used by bootloader
set(EXECUTABLE_MCUBOOT_NAME "pinetime-mcuboot-lite")
set(EXECUTABLE_MCUBOOT_FILE_NAME ${EXECUTABLE_MCUBOOT_NAME}-${pinetimelite_VERSION_MAJOR}.${pinetimelite_VERSION_MINOR}.${pinetimelite_VERSION_PATCH})
set(IMAGE_MCUBOOT_FILE_NAME image-${pinetimelite_VERSION_MAJOR}.${pinetimelite_VERSION_MINOR}.${pinetimelite_VERSION_PATCH}.bin)
set(DFU_FILE_NAME dfu-${pinetimelite_VERSION_MAJOR}.${pinetimelite_VERSION_MINOR}.${pinetimelite_VERSION_PATCH}.zip)
set(NRF5_LINKER_SCRIPT_MCUBOOT "${CMAKE_SOURCE_DIR}/gcc_nrf52-mcuboot.ld")
add_executable(${EXECUTABLE_MCUBOOT_NAME} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE_MCUBOOT_NAME} nimble nrf-sdk lvgl bma421 )
set_target_properties(${EXECUTABLE_MCUBOOT_NAME} PROPERTIES OUTPUT_NAME ${EXECUTABLE_MCUBOOT_FILE_NAME})
target_compile_options(${EXECUTABLE_MCUBOOT_NAME} PUBLIC
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:DEBUG>>: ${COMMON_FLAGS} -O0 -g3>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:RELEASE>>: ${COMMON_FLAGS} -O3 ${COMMON_FLAGS_OPT}>
        $<$<COMPILE_LANGUAGE:ASM>: -MP -MD -x assembler-with-cpp>
        )

set_target_properties(${EXECUTABLE_MCUBOOT_NAME} PROPERTIES
        SUFFIX ".out"
        LINK_FLAGS "-mthumb -mabi=aapcs -L ${NRF5_SDK_PATH}/modules/nrfx/mdk -T${NRF5_LINKER_SCRIPT_MCUBOOT} -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,--gc-sections --specs=nano.specs -lc -lnosys -lm -Wl,-Map=${EXECUTABLE_MCUBOOT_FILE_NAME}.map"
        )

add_custom_command(TARGET ${EXECUTABLE_MCUBOOT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE_MCUBOOT_FILE_NAME}.out
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE_MCUBOOT_FILE_NAME}.out "${EXECUTABLE_MCUBOOT_FILE_NAME}.bin"
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE_MCUBOOT_FILE_NAME}.out "${EXECUTABLE_MCUBOOT_FILE_NAME}.hex"
        COMMENT "post build steps for ${EXECUTABLE_MCUBOOT_FILE_NAME}"
        )


# FLASH
if (USE_JLINK)
    add_custom_target(FLASH_ERASE
            COMMAND ${NRFJPROG} --eraseall -f ${NRF_TARGET}
            COMMENT "erasing flashing"
            )
    add_custom_target("FLASH_${EXECUTABLE_NAME}"
            DEPENDS ${EXECUTABLE_NAME}
            COMMAND ${NRFJPROG} --program ${EXECUTABLE_FILE_NAME}.hex -f ${NRF_TARGET} --sectorerase
            COMMAND sleep 0.5s
            COMMAND ${NRFJPROG} --reset -f ${NRF_TARGET}
            COMMENT "flashing ${EXECUTABLE_NAME}.hex"
            )

elseif (USE_GDB_CLIENT)
    add_custom_target(FLASH_ERASE
            COMMAND ${GDB_CLIENT_BIN_PATH} -nx --batch -ex 'target extended-remote ${GDB_CLIENT_TARGET_REMOTE}' -ex 'monitor swdp_scan' -ex 'attach 1' -ex 'mon erase_mass'
            COMMENT "erasing flashing"
            )
    add_custom_target("FLASH_${EXECUTABLE_NAME}"
            DEPENDS ${EXECUTABLE_NAME}
            COMMAND ${GDB_CLIENT_BIN_PATH} -nx --batch -ex 'target extended-remote ${GDB_CLIENT_TARGET_REMOTE}' -ex 'monitor swdp_scan' -ex 'attach 1' -ex 'load' -ex 'kill' ${EXECUTABLE_NAME}.hex
            COMMENT "flashing ${EXECUTABLE_NAME}.hex"
            )
elseif (USE_OPENOCD)
    if (USE_CMSIS_DAP)
        add_custom_target(FLASH_ERASE
                COMMAND ${OPENOCD_BIN_PATH} -c 'source [find interface/cmsis-dap.cfg]' -c 'transport select swd'
                -c 'source [find target/nrf52.cfg]'
                -c 'init'
                -c 'halt'
                -c 'nrf5 mass_erase'
                -c 'halt'
                -c 'reset'
                -c 'exit'
                COMMENT "erasing flashing"
                )
        add_custom_target("FLASH_${EXECUTABLE_NAME}"
                DEPENDS ${EXECUTABLE_NAME}
                COMMAND ${OPENOCD_BIN_PATH}
                -c 'tcl_port disabled'
                -c 'gdb_port 3333'
                -c 'telnet_port 4444'
                -c 'source [find interface/cmsis-dap.cfg]'
                -c 'transport select swd'
                -c 'source [find target/nrf52.cfg]'
                -c 'halt'
                -c "program \"${EXECUTABLE_NAME}.hex\""
                -c 'reset'
                -c 'shutdown'
                COMMENT "flashing ${EXECUTABLE_NAME}.hex"
                )
    else ()
        add_custom_target(FLASH_ERASE
                COMMAND ${OPENOCD_BIN_PATH} -f interface/stlink.cfg -c 'transport select hla_swd' -f target/nrf52.cfg -c init -c halt -c 'nrf5 mass_erase' -c reset -c shutdown
                COMMENT "erasing flashing"
                )
        add_custom_target("FLASH_${EXECUTABLE_NAME}"
                DEPENDS ${EXECUTABLE_NAME}
                COMMAND ${OPENOCD_BIN_PATH} -c "tcl_port disabled" -c "gdb_port 3333" -c "telnet_port 4444" -f interface/stlink.cfg -c 'transport select hla_swd' -f target/nrf52.cfg -c "program \"${EXECUTABLE_NAME}.hex\"" -c reset -c shutdown
                COMMENT "flashing ${EXECUTABLE_NAME}.hex"
                )
    endif ()
endif ()
