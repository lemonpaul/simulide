A Electronic Circuit Simulator.

It's developed in C++ using Qt4 framework.


______________________________________________________________________

COMPILING:

compiling simavr:
There is a pre-compiled simavr shared library libsimavr.so (Ubuntu 10.04) 
in: SimulIDE/lib/libsimavr.so

If you need you can compile simavr library,
there is a folder containing simavr sources: include/simavr

cd include/simavr
make

if you compiled simavr  You must copy:
include/simavr/simavr/libsimavr.so to: SimulIDE/lib/libsimavr.so


Compiling SimulIDE:
There is a pre-compiled SimulIDE executable (Ubuntu 10.04).

If you need to compile it:
There is a Qt project file in the root folder, you can use it or:

qmake
make

________________________________________________________________________________

RUNNING SimulIDE:

The folder "SimulIDE" contains all necessary to run the program.

By now no installing needed. you can copy/paste this folder 
to whatever location you want and run the executable: SimulIDE

________________________________________________________________________________

USING SimulIDE:

Circuit Simulator:

    Right-click:      context menu: open, save, etc.

    Drag-drop components from left-pane->Components
    to the circuit and create connectors by clicking on pins.
    
    Click+move components.
    Right-click on components: component context menu.
    Select single components and see/edit properties in left-pane->Properties tab
    
    Shift+click_on_empty: scroll circuit.
    Mouse_wheel:          zoom in/out.
    
    click_on_wire:        create node and init new wire.
    ctrl+click_on_wire:   move wire 
    
    Power button:         start/stop simulation.
    
    Microcontrollers:
        Right-click to context menu:
            Load firmware:
                load the firmware. click on power button to start simulation.
                You can watch registers as in debuggin session.
                
            Re-load firmware:
                Re-loads a loaded firmware ( for example new version ).
                
            Load from editor:
                Loads the firmware asociated to actual document in editor
                and start a debugging session in editor.
                Then step, step-over, etc.
                click on power button to have the circuit working.
                You can watch registers.
                


