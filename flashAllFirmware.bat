:: Deletes All files in the Current Directory With Prompts and Warnings 
::(Hidden, System, and Read-Only Files are Not Affected) 
:: 
@ECHO OFF 
python patch_image_header.py Build/marvel3-appPrimary.bin
python patch_image_header.py Build/marvel3-appSecondary.bin

xpack-openocd-0.11.0-4\bin\openocd.exe -f ./openocd.cfg -c "program build/marvel3-bootLoader.bin exit 0x8000000"
xpack-openocd-0.11.0-4\bin\openocd.exe -f ./openocd.cfg -c "program build/marvel3-appPrimary.bin exit 0x800C800"
xpack-openocd-0.11.0-4\bin\openocd.exe -f ./openocd.cfg -c "program build/marvel3-appSecondary.bin reset exit 0x8040000"


:: Check if OpenOCD ran successfully
if %errorlevel% equ 0 (
    echo MARVEL Flashed successfully! 
) else (
    echo Flashing failed! Please check the connections and try again...
)

:exit
:: Pause for user acknowledgment
echo Press any key to exit.
pause > nul