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
data class Space(var hasBox: Boolean) : Tile {
  override fun toString() = if (hasBox) "O" else "."
}

fun findFree(board: List<List<Tile>>, startPos: Point, dir: Direction): Point? {
  var pos = startPos
  while (true) {
    pos = dir.adjust(pos)
    val tile = board[pos.row][pos.col]
    if (tile is Wall) {
      return null
    }
    check(tile is Space)
    if (!tile.hasBox) {
      return pos
    }
  }
}

fun main() {
  lateinit var pos: Point
  val board = generateSequence(::readLine).takeWhile { !it.isEmpty() }.withIndex().map {
    (row, line) ->
    line.withIndex().map { (col, c) ->
      when(c) {
        '#' -> Wall
        '.' -> Space(false)
        'O' -> Space(true)
        '@' -> {
          pos = Point(row, col)
          Space(false)
        }
        else -> error("Unsupported: $c")
      }
    }
  }.toList()
  val moves = generateSequence(::readLine).joinToString("").map(Direction::fromChar)

  for (m in moves) {
    val nextPos = m.adjust(pos)
    val nextTile = board[nextPos.row][nextPos.col]
    if (nextTile is Wall) {
      continue
    }
    check(nextTile is Space)
    if (!nextTile.hasBox) {
      pos = nextPos
    } else {
      val freePos = findFree(board, nextPos, m)
      if (freePos != null) {
        val freeTile = board[freePos.row][freePos.col]
        check(freeTile is Space)
        nextTile.hasBox = false
        freeTile.hasBox = true
        pos = nextPos
      }
    }
  }

  val result = board.withIndex().map { (rowIndex, row) ->
    row.withIndex().map { (colIndex, t) ->
      if (t is Space && t.hasBox) {
        rowIndex * 100 + colIndex
      } else {
        0
      }
    }.sum()
  }.sum()
  println(result)

}
