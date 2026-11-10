# PA2 Reflection: Fifth Leaf

For these answers, `n` is the number of scenes, `h` is the number of scenes in one playthrough, and `b` is the number of items in the bag.

## 1. My functions and their time complexities

1. **Scene constructor (TODO 1):** It makes space for exactly the number of choices that scene has. I set each new slot to `nullptr` so there is nothing random in it. An ending does not need an array. Since there can only be up to three choices, this is O(1).

2. **Scene destructor (TODO 2):** It deletes the child scenes, the choices array, and the scene's data. Deleting a child also deletes everything below that child, so deleting a whole subtree takes O(n).

3. **isEnding (TODO 3):** It checks whether the scene has zero choices. That is O(1).

4. **getChoice (TODO 4):** It returns a child at an index, or `nullptr` if the index is out of range. That is O(1).

5. **setChoice (TODO 5):** It puts a child in a valid choice slot. That is O(1).

6. **countScenes (TODO 6):** It counts the current scene as one, then calls itself on every child. Each scene is counted once, so it is O(n).

7. **countEndings (TODO 7):** It counts a scene if it has no choices. Otherwise, it adds the ending counts from its children. It visits every scene once, so it is O(n).

8. **height (TODO 8):** It finds the longest route from a scene down to an ending. It checks the heights of the children and uses the largest one. It visits all the scenes, so it is O(n).

9. **clear (TODO 9):** It deletes the root and then sets `root_` to `nullptr`. The scene destructors delete the rest of the tree, so this takes O(n).

10. **load (TODO 10):** It opens the file, reads the starting HP, and calls `readScene` to build the story. If loading works, it gives the root to the tree. Reading `n` scenes takes O(n), ignoring the lengths of the text lines.

11. **readScene (TODO 11):** It reads one scene and then calls itself to read each choice's scene. It also rejects a scene with more than three choices. If loading fails, it deletes the part it already built. Each scene is read once, so it is O(n).

12. **askChoice (TODO 12):** It reads what the player typed and keeps asking until they enter a valid choice or input ends. This depends on how much the player types and how often they retry, not on `n`. Checking one line takes O(length of the line).

13. **play (TODO 13):** It goes through one route, adds HP and loot, and puts new items in the bag. It stops when the player dies or reaches an ending. It visits at most `h` scenes, and checking the bag for an item can take O(b), so the worst case is O(hb).

14. **printStats (TODO 14):** It prints the number of scenes, endings, and choices in the longest route. Those functions go through the tree, so this is O(n).

15. **printSpoilers (TODO 15):** It starts `printEndings` at the root with an empty path. That visits the whole tree, so the scene traversal is O(n).

16. **printEndings (TODO 16):** It goes through the choices in order and prints the path when it finds an ending. Each scene is visited once, so the tree traversal is O(n). Making and printing the path strings takes extra time.

17. **greedyRun (TODO 17):** It takes the choice with the most loot in the next scene, without looking farther ahead. It only walks one route and checks at most three choices per scene, so it is O(h), or at most O(n).

18. **bestLoot (TODO 18):** It tries every branch and ignores paths where HP reaches zero. It returns the most loot from a surviving route and saves that route's path. It visits every scene once, so the tree traversal is O(n). Copying the path strings can take extra time, up to O(nh) in the worst case.

19. **optimalRun (TODO 19):** It calls `bestLoot` and prints the best surviving route, or says that none survives. The tree traversal is O(n), with the same extra path-string work as `bestLoot`.

## 2. Why use the exact number of slots?

Not every scene has three choices, so giving every scene three slots would waste space. The assignment also says the array has to be the exact size needed. If a scene is an ending, it has zero choices, so `choices_` stays `nullptr` and there is no array to allocate.

## 3. What happens in the Scene destructor?

First, I delete each child in the choices array. Each child deletes its own children too. Then I use `delete[]` on the choices array, and finally I delete the scene's data. The slots start as `nullptr`, which matters if loading stops halfway through a story. If I deleted the array first, I would lose access to the pointers to its child scenes. That could leak memory or make the program read memory that was already freed.

## 4. Greedy compared with optimal in my story

My story is an alternate Black Clover adventure with Asta, Yuno, Father Orsi, and Sister Lily. Greedy takes path `2-1`. It chooses the church cellar because that scene gives 8 loot right away, more than either of the other first choices. Then it opens the gold chest. The chest gives more loot, but the trap brings Asta's HP down to -1. Greedy dies with **28 loot**.

Optimal takes path `3-1-1-1-1-1-1-1`. Asta helps Father Orsi, trains, gets the five-leaf grimoire, and destroys the Tower. He ends with **9 HP and 32 loot**. Greedy loses because it only looks at the next reward and does not see that the chest is a trap.

## 5. Why is bestLoot O(n) if it tries every path?

Checking every path might sound exponential. But this is a tree, so the different branches do not join back together. `bestLoot` visits each scene once as it goes down the tree. It does not go back to the root and redo the same scenes for each ending. That makes the work of visiting scenes O(n). The path strings can still add extra copying time.

## 6. How I use List

I use `List<Item>` for the player's bag. `search` checks if the player already has an item so they cannot get two with the same name. `addFront` puts a new item in the bag. `size` gives the number of items at the end, and `print` shows them. I do not call `deleteFront` myself because the list destructor cleans up when `play` ends. If there are `b` items, one `search` can take O(b) because it may have to look through the whole list.