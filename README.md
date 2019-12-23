#Data stuctures and algorithms
Our goal was to make as effective program as possible. We were also assigned to use built-in
data stuctures and algorithms (STL) and assest the effectiveness of our implementations.

##Prg1: Beacons of RGB
Beacons in this assignment send light to each others instead of rotating. 
Receiving beacons add the incoming lightbeam to their own light. All the beacons have a 
location (x,y), color (r,g,b) and name. In this part of the assignment we were given a task to 
create a class for managing the beacons and their lightbeams. Class Datastructures is all my work. 
We also got some functionalities ready from the course staff.

##Prg2: Game of fibres
This part expands the prg1 program. Instead of sending the light in the air through the fog, it's more 
efficient to send it underground using (optical)fibres. In every crossroad you can decide where to 
send the light. If a beacon is not connected to a fibre it can't send light through them. Every fibre 
has a startingpoint and endingpoint and the light cant travel both directions in the fibre.

##How to use the program
Select the function from the dropdown menu and type in all the parameters as shown above the writing area.
For example to add beacons select add_beacon from the dropdown menu and type e.g. G1 Lime (0,0) (0,255,0).
On the right side of the window you can choose whether you want to see different kinds of information in the game.

##Other notes
There are several efficiency tests for the game. You can run them by using the read function from the dropdown 
menu and typing in the filename of the test. For example testing the beacons select read and type 
"example-beacons.txt" and hit the execute button.

