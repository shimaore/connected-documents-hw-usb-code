v 20130925 2
C 40000 40000 0 0 0 title-B.sym
C 40500 48300 1 0 0 usb.sym
C 45200 43800 1 0 0 attiny2313a.sym
C 43300 45600 1 0 0 resistor-1.sym
{
T 43600 46000 5 10 0 0 0 0 1
device=RESISTOR
T 43500 45900 5 10 1 1 0 0 1
refdes=R1
T 43900 45900 5 10 1 1 0 0 1
value=68
}
N 44200 45700 45200 45700 4
C 44300 46000 1 0 0 resistor-1.sym
{
T 44600 46400 5 10 0 0 0 0 1
device=RESISTOR
T 44500 46300 5 10 1 1 0 0 1
refdes=R2
T 44800 46300 5 10 1 1 0 0 1
value=1.5K
}
N 44300 46100 44300 45700 4
C 43600 46900 1 90 0 crystal-1.sym
{
T 43100 47100 5 10 0 0 90 0 1
device=CRYSTAL
T 43300 47100 5 10 1 1 90 0 1
refdes=U2
T 42900 47100 5 10 0 0 90 0 1
symversion=0.1
T 43800 47000 5 10 1 1 90 0 1
value=12MHz
}
C 42600 47400 1 0 0 capacitor-1.sym
{
T 42800 48100 5 10 0 0 0 0 1
device=CAPACITOR
T 42800 47900 5 10 1 1 0 0 1
refdes=C1
T 42800 48300 5 10 0 0 0 0 1
symversion=0.1
T 43200 47900 5 10 1 1 0 0 1
value=22pF
}
C 42600 46700 1 0 0 capacitor-1.sym
{
T 42800 47400 5 10 0 0 0 0 1
device=CAPACITOR
T 42800 46500 5 10 1 1 0 0 1
refdes=C2
T 42800 47600 5 10 0 0 0 0 1
symversion=0.1
T 43200 46500 5 10 1 1 0 0 1
value=22pF
}
N 42600 47600 42600 46900 4
C 42400 46600 1 0 0 ground.sym
N 43500 47600 44500 47600 4
N 44500 47600 44500 47300 4
N 44500 47300 45200 47300 4
N 43500 46900 45200 46900 4
C 43500 46200 1 0 0 resistor-1.sym
{
T 43800 46600 5 10 0 0 0 0 1
device=RESISTOR
T 43700 46500 5 10 1 1 0 0 1
refdes=R3
T 44100 46500 5 10 1 1 0 0 1
value=68
}
N 44400 46300 44400 46500 4
N 44400 46500 45200 46500 4
C 41700 49200 1 0 0 passive-1.sym
{
T 42600 49400 5 10 0 0 0 0 1
net=PASSIVE:1
T 41900 49900 5 10 0 0 0 0 1
device=none
T 42600 49300 5 10 1 1 0 1 1
value=D-
}
C 41700 48800 1 0 0 passive-1.sym
{
T 42600 49000 5 10 0 0 0 0 1
net=PASSIVE:1
T 41900 49500 5 10 0 0 0 0 1
device=none
T 42600 48900 5 10 1 1 0 1 1
value=D+
}
C 43300 45600 1 0 1 passive-1.sym
{
T 42400 45800 5 10 0 0 0 6 1
net=PASSIVE:1
T 43100 46300 5 10 0 0 0 6 1
device=none
T 42400 45700 5 10 1 1 0 7 1
value=D-
}
C 43300 46200 1 0 1 passive-1.sym
{
T 42400 46400 5 10 0 0 0 6 1
net=PASSIVE:1
T 43100 46900 5 10 0 0 0 6 1
device=none
T 42400 46300 5 10 1 1 0 7 1
value=D+
}
N 43300 46300 43500 46300 4