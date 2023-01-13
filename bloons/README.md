# bloons tower defense in sdl

- add levels with many balloons
  : when bloons[bloons.size()].getPI > bloons[0].src.w then add next bloon
- have global level
  : have [i][j] array of levels, i is level, j is balloon in the level, an enum for type of balloon. like 0 is blue,1 is yellow, etc.
- have single point at end of dart to check collision, have dart rotate around it.
- for each monkey check if any bloons are within the diameter of the square and then target first bloon.
  : for all bloons within range, check which has the highest PI, attack that one, and use the slope/angle to rotate monkey and dart vector
- make hitbox of bloon slightly smaller.
