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
}

sealed interface Tile
object Wall : Tile
data class Space(var distance: Int = -1) : Tile

fun tileFromChar(c: Char): Tile = when(c) {
  '#' -> Wall
  'S' -> Space()
  'E' -> Space(0)
  '.' -> Space()
  else -> error("Unsupported: $c")
}

fun List<List<Tile>>.findTile(t: Tile): Point {
  for ((rowIndex, row) in this.withIndex()) {
    for ((colIndex, c) in row.withIndex()) {
      if (c == t) {
        return Point(rowIndex, colIndex)
      }
    }
  }
  error("Not Found")
}

fun markDistances(grid: List<List<Tile>>, endPoint: Point){
  var current = endPoint
  var distance = 1
  while (true) {
    var keepGoing = false
    for (d in Direction.values()) {
      val nextPoint = d.adjust(current)
      val next = grid[nextPoint.row][nextPoint.col]
      if (next is Space && next.distance == -1) {
        next.distance = distance
        ++distance
        current = nextPoint
        keepGoing = true
        break
      }
    }
    if (!keepGoing) {
      break
    }
  }
}

// +2 for the steps you have to actually take 
const val MIN_SAVE = 102

fun main() {
  var grid = generateSequence(::readLine).map { line ->
    "#$line#".map(::tileFromChar)
  }.toList()
  val padding = listOf(List(grid[0].size) { Wall })
  grid = padding + grid + padding
  val endPoint = grid.findTile(Space(0))
  markDistances(grid, endPoint)

  var total = 0
  for ((rowIndex, row) in grid.withIndex()) {
    for ((colIndex, t) in row.withIndex()) {
      if (t !is Space) { continue }
      val current = Point(rowIndex, colIndex)
      for (d in Direction.values()) {
        val nextPoint = d.adjust(d.adjust(current))
        val nextTile = grid[nextPoint.row][nextPoint.col]
        if (nextTile is Space && t.distance - nextTile.distance >= MIN_SAVE) {
          ++total
        }
      }
    }
  }
  println(total)
}

