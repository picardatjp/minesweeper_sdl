# chess in sdl

## todo

- finish movement:
  - castling:
    - king and rook haven't moved yet and king does not pass through check
    - en passant:
    - only available for one turn after pawn moves next to enemy pawn after using a two move start movement in check:
    - cant move into check or make move where the next move would result in check
    - can only make moves that remove check and don't again go into check
- implement checkmate
- add in ability to load specific game with FEN
- refactor to look nicer, more maintainable
