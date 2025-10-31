

# DIY GHOST TRAP, WITH ARDUINO LED CONTROL


## In summary
Control of various LEDs which accompany the cardboard box that has become a Ghostbusters style trap prop.
Originally it was a look-a-like only, for Halloween.
Then it seemed like a good idea to add some LEDs
Then it would be a waste of effort to NOT make the LEDs put on a show!

## Hardware design
Begun originally on an Arduino UNO, for rapid development, due to already having one and it having easy pin connections.
Moved to an Arduino Nano Every, as a low-cost small PCB for including in the trap box.

LEDs were from a "maker set", and crudely connected directly to the Arduino pins without in-line resistors.
Arduino IO are current limited, so we're banking on them doing their job!

Wide input voltage of the Arduino allows it to be run from a 9V PP3 battery for combination of long usage, availability and portability.

## Software design
Intended as least-effort-possible code, and therefore based on Fade example.
As the code began, the number of LEDs grew, and the code extended.

Original example code snippets (of Analogue Fader and For-Loop LED stepper) needed rewriting to allow them to operate via sub-routine functions.

Where possible, references to IO pins etc are pre-set in variables at the top of the code.

