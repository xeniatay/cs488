// Xenia Tay
// 20396769
// xzytay

//---------------------------------------------------------------------------
//
// CS488 -- Introduction to Computer Graphics
//
// game.hpp/game.cpp
//
// An engine that implements a falling blocks game.  You won't need to
// modify these files unless you decide to enhance the underlying game
// logic.
//
// University of Waterloo Computer Graphics Lab / 2003
//
//---------------------------------------------------------------------------

#ifndef CS488_GAME_HPP
#define CS488_GAME_HPP

class Piece {
public:
  Piece();
  Piece(const char *desc, int cindex,
         int left, int top, int right, int bottom);

  Piece& operator =(const Piece& other);

  int getLeftMargin() const;
  int getTopMargin() const;
  int getRightMargin() const;
  int getBottomMargin() const;
  int getColourIndex() const;

  Piece rotateCW() const;
  Piece rotateCCW() const;

  bool isOn(int row, int col) const;
  int cindex_;

private:
  void getColumn(int col, char *buf) const;
  void getColumnRev(int col, char *buf) const;

  char desc_[16];
  int margins_[4];
};

class Game {
  public:
    // Create a new game instance with a well of the given dimensions.
    // Note that internally, the board has four extra rows, to hold a
    // piece that has just begun to fall.
    Game(int width, int height);

    ~Game();

    // Set the game to an initial state -- empty well, one piece waiting
    // on top.
    void reset();

    // Advance the game by one tick.  This usually just pushes the
    // currently falling piece down by one row.  It can sometimes cause
    // one or more rows to be filled and removed.  This method returns
    // three kinds of values:
    // 			<0: game over!
    //   		 0: business as usual.
    // 	 {1,2,3,4}: the most recent piece removed 1,2,3 or 4 rows, and
    // 				a new piece has started to fall.
    int tick();

    // Move the currently falling piece left or right by one unit.
    // Returns whether the move was successful.
    bool moveLeft();
    bool moveRight();

    // Drop the current piece to the lowest position it can legally
    // occupy.  Returns whether anything happened.
    bool drop();

    // Rotate the piece clockwise or counter-clockwise.  Returns whether
    // the rotation was successful.
    bool rotateCW();
    bool rotateCCW();

    int getWidth() const
    {
      return board_width_;
    }
    int getHeight() const
    {
      return board_height_;
    }

    // Get the contents of the cell at row r and column c.  Returns
    // the following values:
    // 				 -1: Cell is empty.
    // 	{0,1,2,3,4,5,6}: Cell contains a piece with the given ID.  Use
    //				     this ID to choose a colour when drawing this cell.
    // NOTE!  You can (and should) actually call this method with values
    // for r in [0,board_height_+4), not [0,board_height_].  The top four
    // rows are added on to accommodate new pieces that are falling into
    // the well.
    int get(int r, int c) const;
    int& get(int r, int c);

    int px_;
    int py_;
    Piece piece_;
    bool is_new_piece;
    int current_piece();
    int px();
    int py();
    int hacky_collapse_py;

  private:
    bool doesPieceFit(const Piece& p, int x, int y);

    void removeRow(int y);
    int collapse();

    void removePiece(const Piece& p, int x, int y);
    void placePiece(const Piece& p, int x, int y);

    void generateNewPiece();

  private:
    int board_width_;
    int board_height_;

    bool stopped_;


    int* board_;
};

#endif // CS488_GAME_HPP
