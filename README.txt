SimulIDE
Electronic Circuit Simulator

SimulIDE is a simple real time electronic circuit simulator.
It's intended for general purpose electronics and microcontroller simulation, supporting PIC and AVR.
PIC simulation is provided by gpsim and avr simulation by simavr.

This is not an accurate simulator for circuit analisis, it aims to be the fast, simple and easy to use, so this means simple and not very accurate electronic models and limited features.

Intended for hobbist or students to learn and experiment with simple circuits.
RUNNING SimulIDE:

The folder "SimulIDE" contains all necessary to run the program.
(For PIC simulation in Linux gpsim must be installed on your system)

By now no installing needed. you can copy/paste this folder
to whatever location you want and run the executable: SimulIDE
All folders and archives must be in same place from SimulIDE folder.
USING SimulIDE:
Circuit:

Click on any empy part of cicuit and see/edit 
Circuit properties in left-panel->Properties tab

Right-click on any empy part of cicuit for context menu:
    Save Circuit as image.

Drag Components from left-panel->Components tab, drop
to the circuit and create connectors by clicking on pins.

Shift+click_on_empty: scroll circuit.
Mouse_wheel:          zoom in/out.

click_on_wire:        create node and init new wire.
ctrl+click_on_wire:   move wire

Power button:         start/stop simulation.

Click on components and move then.

Right-click on components for context menu:
    Remove.
    Rotate CW.
    Rotate CCW.
    Rotate 180.

Select single components and see/edit properties in left-panel->Properties tab:
    id: you can edit id of the component.

    Show id: show or hide id label (hidden by default).

    Some Components have a value label, like resistance or capacitance
    you can edit value, unit and show/hide value label (shown by deafult).

Microcontrollers:

Right-click for context menu:
    Load firmware:
        load the firmware.

    Re-load firmware:
        Re-loads a loaded firmware (for example new version).

    Open Serial Term:
        Open serial monitor in bottom panel
        Any data sent by first usart of microcontroller
        Will de printed here.

    Close Serial Term:
        Close serial monitor in bottom panel

Afther loading firmware you can watch registers 
by name or address in left-panel->Ram Table tab.

Select microcontroller and see/edit properties in left-panel->Properties tab:
    Set clock speed Mhz.

Probe:

Hover probe over wires or pins to see voltages at any point of the circuit.

Right-click for context menu:
    Add to Plotter:
        Open Plotter in bottom panel and plots probe voltage.

    Remove from Plotter
        Remove probe from plotter.

    Add to oscope:
        Open Oscilloscope in bottom panel 
        and display probe voltage wave.

    Remove from oscope:
        Remove Oscilloscope.

