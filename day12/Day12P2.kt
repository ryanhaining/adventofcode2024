data class Point(val row: Int, val col: Int)

enum class Direction {
  UP {
    override fun adjust(point: Point) = point.copy(row=point.row-1)
    override val sideways get() = listOf(LEFT, RIGHT)
  },
  DOWN {
    override fun adjust(point: Point) = point.copy(row=point.row+1)
    override val sideways get() = listOf(LEFT, RIGHT)
  },
  LEFT {
    override fun adjust(point: Point) = point.copy(col=point.col-1)
    override val sideways get() = listOf(UP, DOWN)
  },
  RIGHT {
    override fun adjust(point: Point) = point.copy(col=point.col+1)
    override val sideways get() = listOf(UP, DOWN)
  };
  abstract fun adjust(point: Point): Point
  abstract val sideways: List<Direction>
}

data class Region(val label: Char, var area: Int, var sides:Int = 0) {
  val cost get(): Int = area * sides
}

sealed interface Tile
data class Unused(val label: Char): Tile
private val DUMMY = Unused('.')
data class Used(val region: Region, val markedSides: MutableSet<Direction> =mutableSetOf<Direction>()): Tile

fun isMatch(used: Used, tile: Tile): Boolean = when(tile) {
  is Used -> tile.region === used.region
  is Unused -> tile.label == used.region.label
}


fun flood(region: Region, lines: List<MutableList<Tile>>, pos: Point) {
  var tile = lines[pos.row][pos.col] 
  if (tile is Used || (tile is Unused && tile.label != region.label)) {
    return
  }
  tile = Used(region)
  lines[pos.row][pos.col] = tile
  ++region.area
  for (d in Direction.values()) {
    flood(region, lines, d.adjust(pos))
  }
}

fun markSide(lines: List<List<Tile>>, start: Point, dir: Direction) {
  val startTile = lines[start.row][start.col]
  check(startTile is Used)
  ++startTile.region.sides
  startTile.markedSides.add(dir)
  for (sd in dir.sideways) {
    var pos = start
    while (true) {
      pos = sd.adjust(pos)
      val tile = lines[pos.row][pos.col]
      if (!isMatch(startTile, tile)) {
        break
      }
      check(tile is Used)
      val nextPos = dir.adjust(pos)
      val nextTile = lines[nextPos.row][nextPos.col]
      if (isMatch(tile, nextTile)) {
        break
      }
      tile.markedSides.add(dir)
    }
  }
}

fun findSides(lines: List<List<Tile>>, pos: Point) {
  val curTile = lines[pos.row][pos.col]
  check(curTile is Used)
  for (d in Direction.values()) {
    val nextPos = d.adjust(pos)
    val nextTile = lines[nextPos.row][nextPos.col]
    if (d !in curTile.markedSides && !isMatch(curTile, nextTile)) {
      markSide(lines, pos, d)
    }
  }
}


fun main() {
  val lines: MutableList<MutableList<Tile>> = generateSequence(::readLine).map { line ->
    val row = line.map<Tile> { c -> Unused(c) } .toMutableList()
    row.add(DUMMY)
    row.add(0, DUMMY)
    row
  }.toMutableList()
  val padding = List<Tile>(lines[0].size) { DUMMY }
  lines.add(padding.toMutableList())
  lines.add(0, padding.toMutableList())

  val regions = mutableListOf<Region>()
  for ((rowIndex, row) in lines.withIndex()) {
    for ((colIndex, tile) in row.withIndex()) {
      if (tile is Unused && tile !== DUMMY) {
        val newRegion = Region(tile.label, 0)
        regions.add(newRegion)
        flood(newRegion, lines, Point(rowIndex, colIndex))
      }
    }
  }
  for ((rowIndex, row) in lines.withIndex()) {
    for ((colIndex, tile) in row.withIndex()) {
      check(tile is Used || tile === DUMMY)
      if (tile !== DUMMY) {
        findSides(lines, Point(rowIndex, colIndex))
      }
    }
  }
  println(regions.map { it.cost }.sum())
}
