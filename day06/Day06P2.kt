data class Point(val row: Int, val col: Int) {
  fun isInBounds(board: List<List<Tile>>) =
    row < board.size && row >= 0 && col < board[0].size && col >= 0

}

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

  fun turn(): Direction = when(this) {
    UP -> RIGHT
    RIGHT -> DOWN
    DOWN -> LEFT
    LEFT -> UP
  }
}

sealed class Tile {
  abstract fun copy(): Tile
}
object Block : Tile() {
  override fun copy() = this
}
class Free(val dirsThrough: MutableSet<Direction> =mutableSetOf()) : Tile() {
  override fun copy() = Free(dirsThrough.toMutableSet())
  val isVisited: Boolean get() = !dirsThrough.isEmpty()
}

fun List<List<Tile>>.copy(withBlock: Point? = null) =
  this.withIndex().map { (rowIndex, row) ->
    row.withIndex().map { (colIndex, tile) ->
      if (withBlock == Point(rowIndex, colIndex)) {
        Block
      } else {
        tile.copy()
      }
    }
}

enum class ExitType { LOOP, COMPLETE }

fun traverse(start: Point, board: List<List<Tile>>): ExitType {
  var pos = start
  var dir = Direction.UP
  while (true) {
    val next = dir.adjust(pos)
    if (!next.isInBounds(board)) {
      return ExitType.COMPLETE
    }
    val nextTile: Tile = board[next.row][next.col]
    if (nextTile is Block) {
      dir = dir.turn()
    } else {
      check(nextTile is Free)
      if (dir in nextTile.dirsThrough) {
        return ExitType.LOOP
      }
      pos = next
      nextTile.dirsThrough.add(dir)
    }
  }
}

fun findVisited(start: Point, clearBoard: List<List<Tile>>): List<Point> {
  val board = clearBoard.copy()
  traverse(start, board)
  return board.withIndex().map {
    (rowIndex, row) ->
        row.withIndex().filter { (_, tile) ->
          tile is Free && tile.isVisited
        }.map { (colIndex, _) ->
          Point(rowIndex, colIndex)
        }
      }.flatten()
}

fun main() {
  lateinit var start: Point
  val clearBoard: List<List<Tile>> = generateSequence(::readLine).withIndex().map { (row, line) ->
    line.withIndex().map { (col, c) ->
      if (c == '.') {
        Free()
      } else if (c == '^') {
          start = Point(row, col)
          Free()
      } else {
        require(c == '#')
        Block
      }
    }
  }.toList()

  val visited = findVisited(start, clearBoard)
  println(visited.count { traverse(start, clearBoard.copy(withBlock=it)) == ExitType.LOOP })
}
