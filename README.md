# BATTLESHIP    


Welcome to the BATTLESHIP README.
This README will help you run BATTLESHIP and teach you how to play. 


## Objective

BATTLESHIP is a guessing game for two players. Each player has two ships which are placed and marked on the LED screen. 
The locations of their ships are concealed from their opponent (Custom screen divider). Players alternate turns calling "shots" 
at the other player's ships, and the objective of the game is to destroy the opposing player's fleet. 

## Prerequisites

To play BATTLESHIP you will need the following:

1. UCFK4 (UC Fun Kit 4)
2. Type B Mini USB cable
3. BATTLESHIP source code
4. Custom screen divider (optional)


## How to launch BATTLESHIP

1. Open terminal.
2. Navigate to the path of the BATTLESHIP source code.
2. Run command `make program`.


## How to play

### Setup phase:


**Buttons:**
- Nav switch: Directional buttons used to position ship, push down to rotate ship.
- Push button (S3): Places ship at desired location.

**Steps:**
1. Position and rotate your ship to desired location.
2. Place ship 1.
3. Ship 2 will appear, position where desired.
4. Place ship 2.
5. First player to complete the setup phase will become player 1.
6. Player 1 wil see a 'WAIT' screen while player 2 continues to set up their ships.


### Battle phase:

        
**Buttons:**
- Nav switch: Directional buttons used to position your next shot.
- Push button (S3): Shoots at desired location.

**Screens:**
- Attack screen: Attacker can position and shoot their shot.
                 Attack screen will also display previous hits as a solid light and misses as a flashing light.

- Defending screen: Defender will be shown their placed ships.
                    Ships will flash at a position if it has been hit by opponent.
                    No buttons will function, defender must wait for opponents shot.

**Steps:**
1. Player 1 will always attack first.
2. 'ATTACK' and 'DEFENCE' will show on players screens according to their current action.
2. Attacker can position shot where desired and shoot.
3. 'HIT or 'MISS' will appear on the screen.
4. Screens will swap attacker and defender.
5. Player 2 will now attack.
6. Repeat until all ships of any player have been sunk.
7. Game will declare a winner and loser.


## Contributors
* **Ryan Chen (rch141)**
* **Harrison Cook (hgc25)**
