data class Point(val row: Int, val col: Int)

enum class Direction {
  UP {
    override fun adjust(point: Point) = point.copy(row=point.row-1)
  },
  DOWN {
    override fun adjust(point: Point) = point.copy(row=point.row+1)
  },
  LEFT {
    override fun adjust(point: Point) = point.copy(col=point.col-1)
  },
  RIGHT {
    override fun adjust(point: Point) = point.copy(col=point.col+1)
  };
  abstract fun adjust(point: Point): Point

  companion object {
    fun fromChar(c: Char): Direction = when(c) {
      '^' -> UP
      '<' -> LEFT
      'v' -> DOWN
      '>' -> RIGHT
      else -> error("Unsupported: $c")
    }
  }
}

sealed interface Tile
object Wall : Tile {
  override fun toString() = "#"
}
data class Space(var hasLeftBox: Boolean, var hasRightBox: Boolean) : Tile {
  val hasBox get() = hasLeftBox || hasRightBox
  override fun toString() = if (hasLeftBox) "["
    else if (hasRightBox) "]"
    else "."
}

fun canMove(board: List<List<Tile>>, startPos: Point, dir: Direction, matchChecked: Boolean = false): Boolean {
  when (dir) {
    Direction.LEFT, Direction.RIGHT -> {
      var pos = startPos
      while (true) {
        pos = dir.adjust(pos)
        val tile = board[pos.row][pos.col]
        if (tile is Wall) {
          return false
        }
        check(tile is Space)
        if (!tile.hasBox) {
          return true
        }
      }
    }
    Direction.UP, Direction.DOWN -> {
      val curTile = board[startPos.row][startPos.col]
      val nextPos = dir.adjust(startPos)
      val nextTile = board[nextPos.row][nextPos.col]
      if (nextTile is Wall) {
        return false
      }
      check(nextTile is Space)
      check(curTile is Space)
      val matchCanMove = if (curTile.hasLeftBox) {
          matchChecked || canMove(board, Direction.RIGHT.adjust(startPos), dir, matchChecked=true)
        } else if (curTile.hasRightBox) {
          matchChecked || canMove(board, Direction.LEFT.adjust(startPos), dir, matchChecked=true)
        } else {
          true
        }
      if (!nextTile.hasBox) {
        return matchCanMove
      }
      return matchCanMove && canMove(board, nextPos, dir)
    }
  }
}

fun doLeftRightMove(board: List<MutableList<Tile>>, startPos: Point, dir: Direction) {
  var pos = dir.adjust(startPos)
  var prevTile = board[pos.row][pos.col]
  while (prevTile is Space && prevTile.hasBox) {
    pos = dir.adjust(pos)
    val temp = board[pos.row][pos.col]
    board[pos.row][pos.col] = prevTile
    prevTile = temp
    check(prevTile is Space)
  }
  val posToClear= dir.adjust(startPos)
  board[posToClear.row][posToClear.col] = Space(false, false)
}

fun doUpDownMove(board: List<MutableList<Tile>>, startPos: Point, dir: Direction, matchMoved: Boolean) {
      val curTile = board[startPos.row][startPos.col]
      val nextPos = dir.adjust(startPos)
      val nextTile = board[nextPos.row][nextPos.col]
      check(nextTile is Space)
      check(curTile is Space)
      if (nextTile.hasBox) {
        doUpDownMove(board, nextPos, dir, matchMoved=false)
      }
      if (curTile.hasLeftBox && !matchMoved) {
        doUpDownMove(board, Direction.RIGHT.adjust(startPos), dir, matchMoved=true)
      }
      if (curTile.hasRightBox && !matchMoved) {
        doUpDownMove(board, Direction.LEFT.adjust(startPos), dir, matchMoved=true)
      }
      board[nextPos.row][nextPos.col] = curTile
      board[startPos.row][startPos.col] = Space(false, false)
    }


fun doMove(board: List<MutableList<Tile>>, startPos: Point, dir: Direction) {
  when(dir) {
    Direction.LEFT, Direction.RIGHT -> doLeftRightMove(board, startPos, dir)
    Direction.UP, Direction.DOWN -> doUpDownMove(board, startPos, dir, false)
  }
}


fun main() {
  lateinit var pos: Point
  val board = generateSequence(::readLine).map {
    it.replace("#", "##")
    .replace(".", "..")
    .replace("O", "[]")
    .replace("@", "@.")
  }.takeWhile { !it.isEmpty() }.withIndex().map {
    (row, line) ->
    line.withIndex().map { (col, c) ->
      when(c) {
        '#' -> Wall
        '[' -> Space(hasLeftBox=true, hasRightBox=false)
        ']' -> Space(hasLeftBox=false, hasRightBox=true)
        '.' -> Space(false, false)
        '@' -> {
          pos = Point(row, col)
          Space(false, false)
        }
        else -> error("Unsupported: $c")
      }
    }.toMutableList()
  }.toList()
  val moves = generateSequence(::readLine).joinToString("").map(Direction::fromChar)

  for (m in moves) {
    if (canMove(board, pos, m)) {
      doMove(board, pos, m)
      pos = m.adjust(pos)
    }
  }

  val result = board.withIndex().map { (rowIndex, row) ->
    row.withIndex().map { (colIndex, t) ->
      if (t is Space && t.hasLeftBox) {
        rowIndex * 100 + colIndex
      } else {
        0
      }
    }.sum()
  }.sum()
  println(result)

}
