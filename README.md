# CS-Flowers
CS Flower is an action packed turn-based game where you the player, move across the Lawn and cut down all the flowers. However, it's not as easy as it sounds. While you snip away at each of them one by one, the flowers wake up, evolve and retaliate against you. So it is up to you to figure out the best angle of attack against these formidable foes!

Game Structure
There are two main phases to the overall game:

Setup Phase
This phase takes care of the initial setup of the map and the game. The setup is as follows:

The player is spawned onto the map
Various tiles known as foliage are placed onto the map
The flowers which act as enemies are spawned onto the map
Gameplay Phase
This phase handles the gameplay, which is split into turns. On each turn, the player, flowers, and the game will manipulate the map. A turn is comprised of the following steps:

The Player Step
The player is allowed to make an action like moving around or cutting a flower
The Upkeep Step
The upkeep is an intermediary step where you update parts of the map and its tiles
The Flower Step
The flowers are all allowed to make an action like attacking or defending against the player
The End Step
The final step at the end of a turn in which you determine if the game should end or continue
As you progress through each stage of this assignment, you will be implementing parts of each step.

