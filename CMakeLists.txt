cmake_minimum_required(VERSION 3.10)

# set the project name
project(marvel3)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

execute_process (
    COMMAND "git rev-parse --short HEAD"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_SHA_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_SHA
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

add_subdirectory(Library/Middleware/threadx-6.1.10_rel)

set(COMMON_INCLUDE
    Core/Inc/
    Drivers/
    Drivers/STM32L4xx_HAL_Driver/Inc/
    Drivers/STM32L4xx_HAL_Driver/Inc/Legacy/
    Drivers/CMSIS/Device/ST/STM32L4xx/Include/
    Drivers/CMSIS/Include/
    Library/Memory/


    Library/Utility/can-tp-module-threadx
    Library/Utility/can-tp-module-threadx/isotp-c/src
    Library/Utility/can-tp-module-threadx/isotp-c/deps/bitfield-c/src


    Library/MATLAB/MARVEL_MBD_SUBMODULE/Common/
    Library/MATLAB/MARVEL_MBD_SUBMODULE/CellBalancing/
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Contactors/
    Library/MATLAB/MARVEL_MBD_SUBMODULE/DataPipeline/
    Library/MATLAB/MARVEL_MBD_SUBMODULE/CoulombCounting/
    Library/MATLAB/MARVEL_MBD_SUBMODULE/EKF/
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Protection/
    Library/MATLAB/MARVEL_MBD_SUBMODULE/StateMachine/
    Library/MATLAB/MARVEL_MBD_SUBMODULE/ChargingAlgorithm/

    Library/Filter/
    Library/Math/
    Library/Interface/
    Library/Threads/
    Library/Functional/
    Library/Utility/
    Library/Utility/ADBMS6830/lib/
    Library/Utility/ADBMS2950/lib/
    Library/Utility/ADBMS6830/programADBMS6830/
    Library/Utility/ADBMS2950/programADBMS2950/
    Library/Peripheral/
    Library/Bootloader
    )

set(THREADX_INCLUDE 
    Library/Middleware/threadx-6.1.10_rel/common/inc/
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/inc/)

set(COMMON_FILES
    Library/Utility/onBoardFlash.c
    Library/Peripheral/flashControls.c

    Core/Src/adc.c
    Core/Src/can.c
    Core/Src/gpio.c
    Core/Src/i2c.c
    Core/Src/spi.c
    Core/Src/tim.c
    # Core/Src/usart.c
    Core/Src/dma.c
    Core/Src/iwdg.c
    # Core/Src/wwdg.c


    Core/Src/stm32l4xx_it.c
    Core/Src/stm32l4xx_hal_msp.c
    Core/Src/system_stm32l4xx.c
    Core/Src/stm32l4xx_hal_timebase_tim.c

    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_can.c
    # Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
    # Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c
    Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c
    # Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_wwdg.c

    startup_stm32l476xx.s)

set(EXECUTABLE_BOOTLDR ${PROJECT_NAME}-bootLoader.elf)
set(EXECUTABLE_APP_PRI ${PROJECT_NAME}-appPrimary.elf)
set(EXECUTABLE_APP_SEC ${PROJECT_NAME}-appSecondary.elf)

set(BOOTLOADER_MAIN
   Library/Bootloader/bootloaderMain.c)

set(BOOTLODER_FILES
    Library/Bootloader/bootloaderFunctions.c)

set(APP_MAIN
    Core/Src/main.c)

set(APP_FILES
    Library/Interface/bmsCANMatrix.c

    # Library/Interface/batteryCooling.c
    Library/Interface/ota.c


    
    Library/Filter/FIRFilter.c

    Library/Math/crc.c
    Library/Math/conversion.c
    Library/Math/lookupTable.c

    Library/MATLAB/MARVEL_MBD_SUBMODULE/Common/rt_nonfinite.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Common/rtGetInf.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Common/rtGetNaN.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Common/rt_zcfcn.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Common/AverageFunction.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Common/MaximumFunction.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Common/MinimumFunction.c

    Library/MATLAB/MARVEL_MBD_SUBMODULE/CellBalancing/CellBalancing.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/CellBalancing/WriteMosfetsData.c

    Library/MATLAB/MARVEL_MBD_SUBMODULE/Contactors/Contactors.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/Contactors/Contactors_data.c

    Library/MATLAB/MARVEL_MBD_SUBMODULE/CoulombCounting/SOCEstimation.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/CoulombCounting/SOC_ReadFromFlash.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/CoulombCounting/SOC_WriteToFlash.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/CoulombCounting/VoltagemV_Vs_CapacitymAh.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/CoulombCounting/UsableCapacity_mAh.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/CoulombCounting/SOCEstimation_data.c

    Library/MATLAB/MARVEL_MBD_SUBMODULE/DataPipeline/DataPipeline_data.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/DataPipeline/DataPipeline.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/DataPipeline/DynamicCurrentLimits_perParallelCell.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/DataPipeline/I2t_perParallelCell.c


    # Library/MATLAB/MARVEL_MBD_SUBMODULE/EKF/EKF.c
    # Library/MATLAB/MARVEL_MBD_SUBMODULE/EKF/EKF_data.c

    Library/MATLAB/MARVEL_MBD_SUBMODULE/Protection/Protection.c

    Library/MATLAB/MARVEL_MBD_SUBMODULE/StateMachine/HighLevelStatemachine.c

    Library/MATLAB/MARVEL_MBD_SUBMODULE/ChargingAlgorithm/ChargingAlgorithm.c
    Library/MATLAB/MARVEL_MBD_SUBMODULE/ChargingAlgorithm/ChargingAlgorithm_data.c
    

    Library/Functional/initializeOperations.c
    Library/Functional/monitorOperations.c
    Library/Functional/controlOperations.c
    Library/Functional/estimateParameters.c
    Library/Functional/dataProcessing.c
    Library/Functional/functionalTests.c
    Library/Functional/firmwareUpgradeApp.c
    Library/Functional/marvelStateMachine.c

    Library/Utility/onBoardIOs.c
    Library/Utility/onBoardUARTs.c
    # Library/Utility/onBoardCMU6815.c

    Library/Utility/onBoardCCM2949.c
    Library/Utility/onBoardEEPROM.c
    Library/Utility/onBoardMemory.c
    Library/Utility/onBoardCANv2b.c

    Library/Utility/ADBMS6830/lib/adBms6830GenericType.c
    Library/Utility/ADBMS6830/lib/adBms6830ParseCreate.c
    Library/Utility/ADBMS6830/programADBMS6830/adBms_Application.c
    Library/Utility/ADBMS6830/programADBMS6830/mcuWrapper.c
    Library/Utility/ADBMS6830/programADBMS6830/serialPrintResult.c

    Library/Utility/ADBMS2950/lib/adi_bms_2950data.c
    Library/Utility/ADBMS2950/programADBMS2950/adbms_2950.c
    Library/Utility/ADBMS2950/programADBMS2950/application_ADBMS2950.c


    Library/Utility/can-tp-module-threadx/isotp-c/deps/bitfield-c/src/bitfield/8byte.c
    Library/Utility/can-tp-module-threadx/isotp-c/deps/bitfield-c/src/bitfield/bitarray.c
    Library/Utility/can-tp-module-threadx/isotp-c/deps/bitfield-c/src/bitfield/bitfield.c
    Library/Utility/can-tp-module-threadx/isotp-c/src/isotp/isotp.c
    Library/Utility/can-tp-module-threadx/isotp-c/src/isotp/receive.c
    Library/Utility/can-tp-module-threadx/isotp-c/src/isotp/send.c
    Library/Utility/can-tp-module-threadx/can_tp_app.c



    Library/Peripheral/gpioControls.c
    Library/Peripheral/analogControls.c
    # Library/Peripheral/uartControls.c
    Library/Peripheral/spiControls.c
    Library/Peripheral/i2cControls.c
    Library/Peripheral/timerControls.c
    Library/Peripheral/canControls.c
    Library/Peripheral/pvd.c
    Library/Peripheral/watchDogControls.c

    )

set(THREADX_MAIN
    Library/Threads/threadxMain.c)
        
set(THREADX_FILES
    Library/Threads/tx_initialize_low_level.c
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_thread_context_restore.S
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_thread_context_save.S
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_thread_interrupt_control.S
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_thread_interrupt_disable.S
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_thread_interrupt_restore.S
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_thread_schedule.S
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_thread_stack_build.S
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_thread_system_return.S
    Library/Middleware/threadx-6.1.10_rel/ports/cortex_m4/gnu/src/tx_timer_interrupt.S)

add_executable(${EXECUTABLE_BOOTLDR} 
                ${COMMON_FILES} 
                ${BOOTLOADER_MAIN} ${BOOTLODER_FILES})

add_executable(${EXECUTABLE_APP_PRI} 
                ${COMMON_FILES} 
                ${APP_MAIN} ${APP_FILES} 
                ${THREADX_MAIN} ${THREADX_FILES})
                
add_executable(${EXECUTABLE_APP_SEC} 
                ${COMMON_FILES} 
                ${APP_MAIN} ${APP_FILES} 
                ${THREADX_MAIN} ${THREADX_FILES})

target_link_libraries(${EXECUTABLE_APP_PRI} PRIVATE threadx)
target_link_libraries(${EXECUTABLE_APP_SEC} PRIVATE threadx)
                
target_compile_definitions(${EXECUTABLE_BOOTLDR} PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32L476xx
    -DGIT_SHA=0x${GIT_SHA}U
    -DNDEBUG=${NDEBUG}
    -DCANDEBUG=${CANDEBUG}
    )

target_compile_definitions(${EXECUTABLE_APP_PRI} PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32L476xx
    -DGIT_SHA=0x${GIT_SHA}U
    -DAPP1
    -DNDEBUG=${NDEBUG}
    -DCANDEBUG=${CANDEBUG}

    )

target_compile_definitions(${EXECUTABLE_APP_SEC} PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32L476xx
    -DGIT_SHA=0x${GIT_SHA}U
    -DAPP2
    -DNDEBUG=${NDEBUG}
    -DCANDEBUG=${CANDEBUG}

    )

target_include_directories(${EXECUTABLE_BOOTLDR} PRIVATE
    ${COMMON_INCLUDE})

target_include_directories(${EXECUTABLE_APP_PRI} PUBLIC
    ${COMMON_INCLUDE}
    ${THREADX_INCLUDE})
    

target_include_directories(${EXECUTABLE_APP_SEC} PUBLIC
    ${COMMON_INCLUDE}
    ${THREADX_INCLUDE})

target_compile_options(${EXECUTABLE_BOOTLDR} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -fdata-sections
    -ffunction-sections

    -std=gnu99
    
    -Wall
    -Wno-comment
    
    $<$<CONFIG:Debug>:-Og>
    )

        
target_compile_options(${EXECUTABLE_APP_PRI} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -fdata-sections
    -ffunction-sections

    -std=gnu99

    -Wall
    -Wno-comment

    $<$<CONFIG:Debug>:-Og>
    )

target_compile_options(${EXECUTABLE_APP_SEC} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -fdata-sections
    -ffunction-sections

    -std=gnu99

    -Wall
    -Wno-comment

    $<$<CONFIG:Debug>:-Og>
    )

target_link_options(${EXECUTABLE_BOOTLDR} PRIVATE
    "SHELL:-T ${CMAKE_SOURCE_DIR}/linkerScripts/memoryMap.ld"
    "SHELL:-T ${CMAKE_SOURCE_DIR}/linkerScripts/bootLdrLinker.ld"
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -fno-math-errno

    -specs=nano.specs

    -u _printf_float
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}-bootLoader.map,--cref
    -Wl,--gc-sections
    )

target_link_options(${EXECUTABLE_APP_PRI} PRIVATE
    "SHELL:-T ${CMAKE_SOURCE_DIR}/linkerScripts/memoryMap.ld"
    "SHELL:-T ${CMAKE_SOURCE_DIR}/linkerScripts/appPriLinker.ld"
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    
    -fno-math-errno

    -specs=nano.specs
    
    -u _printf_float
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}-appPrimary.map,--cref
    -Wl,--gc-sections
    )


target_link_options(${EXECUTABLE_APP_SEC} PRIVATE
    "SHELL:-T ${CMAKE_SOURCE_DIR}/linkerScripts/memoryMap.ld"
    "SHELL:-T ${CMAKE_SOURCE_DIR}/linkerScripts/appSecLinker.ld"
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    
    -fno-math-errno

    -specs=nano.specs

    -u _printf_float
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}-appSecondary.map,--cref
    -Wl,--gc-sections
    )

add_custom_command(TARGET ${EXECUTABLE_BOOTLDR}
    POST_BUILD
    COMMAND arm-none-eabi-size ${EXECUTABLE_BOOTLDR})

    
add_custom_command(TARGET ${EXECUTABLE_APP_PRI}
    POST_BUILD
    COMMAND arm-none-eabi-size ${EXECUTABLE_APP_PRI})


add_custom_command(TARGET ${EXECUTABLE_APP_SEC}
    POST_BUILD
    COMMAND arm-none-eabi-size ${EXECUTABLE_APP_SEC})

add_custom_command(TARGET ${EXECUTABLE_BOOTLDR}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE_BOOTLDR} ${PROJECT_NAME}-bootLoader.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE_BOOTLDR} ${PROJECT_NAME}-bootLoader.bin)

add_custom_command(TARGET ${EXECUTABLE_APP_PRI}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE_APP_PRI} ${PROJECT_NAME}-appPrimary.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE_APP_PRI} ${PROJECT_NAME}-appPrimary.bin)

add_custom_command(TARGET ${EXECUTABLE_APP_SEC}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE_APP_SEC} ${PROJECT_NAME}-appSecondary.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE_APP_SEC} ${PROJECT_NAME}-appSecondary.bin)