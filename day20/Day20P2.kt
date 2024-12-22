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
data class Space(var distance: Int = -1): Tile

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

fun markDistances(grid: List<List<Tile>>, endPoint: Point) {
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

const val MIN_SAVE = 100
const val ALLOWED_STEPS = 20
val HORIZONTAL_PADDING = "#".repeat(ALLOWED_STEPS)

fun numSaves(grid: List<List<Tile>>, p: Point): Int {
  val tile = grid[p.row][p.col]
  if (tile !is Space) {
    return 0
  }
  return (ALLOWED_STEPS downTo -ALLOWED_STEPS).map { rowOffset ->
    val diff = ALLOWED_STEPS - Math.abs(rowOffset)
    (-diff .. +diff).count { colOffset ->
      val toCheck = grid[p.row+rowOffset][p.col+colOffset]
      if (toCheck !is Space) {
        false
      } else {
        val saved = tile.distance - toCheck.distance - (Math.abs(rowOffset) + Math.abs(colOffset))
        saved >= MIN_SAVE
      }
    }
  }.sum()
}

fun main() {
  var grid = generateSequence(::readLine).map { line ->
    "$HORIZONTAL_PADDING$line$HORIZONTAL_PADDING".map(::tileFromChar)
  }.toList()
  val verticalPadding = List(ALLOWED_STEPS) { List(grid[0].size) { Wall } }
  grid = verticalPadding + grid + verticalPadding
  val endPoint = grid.findTile(Space(0))
  markDistances(grid, endPoint)

  // offset the start and end to skip all the padding.
  val total = (ALLOWED_STEPS .. grid.size - ALLOWED_STEPS).map { rowIndex->
    (ALLOWED_STEPS .. grid[rowIndex].size - ALLOWED_STEPS).map { colIndex ->
      numSaves(grid, Point(rowIndex, colIndex))
    }.sum()
  }.sum()
  println(total)
}
