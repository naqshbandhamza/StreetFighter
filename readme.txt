setup for visual studio (c++)
all of these files are to be pasted where you keep your .cpp files in a visual studio c++ project
in order to run this program first of all you must donwload sfml on your computer and keep note of the directory path where you install it.
then include the .h files in header section and .cpp files in the .cpp section
top in the menu bar select properties and then select your project properties at the bottom
select C/C++ , select general paste "C:\SFML-2.5.1\include" against the additional include directories, enter then apply (this is the path of the location where sfml is installed in C by defualt)
comeout and now select linker, select general against additional library directories paste "C:\SFML-2.5.1\lib" enter then apply
select linker--select input and paste the following:
kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-network-d.lib;sfml-audio-d.lib;%(AdditionalDependencies)
against the additional dependecies enter and apply
now try to run the program ,it runs GOOD!!!
else check again in project properties select your platform for mine it was x64.
repeat the above steps then run again it should work
thanks!!!!!!!
