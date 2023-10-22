setup for visual studio (c++)
1. create an empty starter visual studio c++ project

2. all of the files are to be pasted where we keep our .cpp files in an empty starter visual studio c++ project

3. in order to run this program first of all you must donwload sfml-2.5.1 (use same version) on your computer and keep note of the directory path where you install it.

4. then include the .h files in header section and .cpp files in the .cpp section of your visual studio dashboard

5. top in the visual studio menu bar select properties and then select your project properties at the bottom

6. select C/C++ -> general && paste "C:\SFML-2.5.1\include" against the additional include directories (exclude double quotes), enter then apply (this is the path of the location where sfml is installed in C Drive by default)

7. select linker -> general && against additional library directories paste "C:\SFML-2.5.1\lib"  (exclude double quotes) press enter then apply

8. select linker -> input and paste the following against the additional dependecies (press enter and apply):
kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-network-d.lib;sfml-audio-d.lib;%(AdditionalDependencies)

now try to run the program ,it runs GOOD!!!

else check again in project properties -> configuration manager:
select your platform config for mine the project worked with the following configuration:
Debug x64
repeat the above steps then run again it should work
thanks!!!!!!!

controls:

player 1 left side (ryu):
z -> move forward
c -> move backwards
v -> jump
x -> roll over
d -> flying kick
s -> kick
a -> punch
e -> down spin kick
q or w or f or g -> special attacks icluding haduken (ryu most famous move from street fighter)

player 2 right side:
j -> move forward
L or l -> move backwards
i -> jump
k -> punch
p -> kick
v -> front jab claws
u -> front back flip kick
t - > panic
m or n or o or u -> special attacks



