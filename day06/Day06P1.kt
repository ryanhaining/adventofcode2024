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

sealed class Tile
object Block : Tile()
class Free(var visited: Boolean=false) : Tile()

fun main() {
  lateinit var pos: Point
  val board: List<List<Tile>> = generateSequence(::readLine).withIndex().map { (row, line) ->
    line.withIndex().map { (col, c) ->
      if (c == '.') {
        Free()
      } else if (c == '^') {
          pos = Point(row, col)
          Free(visited=true)
      } else {
        require(c == '#')
        Block
      }
    }
  }.toList()

  var dir = Direction.UP
  while (true) {
    val next = dir.adjust(pos)
    if (!next.isInBounds(board)) {
      break
    }
    val nextTile: Tile = board[next.row][next.col]
    if (nextTile is Block) {
      dir = dir.turn()
    } else {
      check(nextTile is Free)
      pos = next
      nextTile.visited = true
    }
  }

  println(
      board.map { row -> row.count { it is Free && it.visited } }.sum()
  )
}
