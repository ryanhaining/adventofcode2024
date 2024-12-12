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

data class Region(val label: Char, var area: Int, var perimeter: Int) {
  val cost get(): Int = area * perimeter
}

sealed interface Tile
data class Unused(val label: Char): Tile
private val DUMMY = Unused('.')
data class Used(val region: Region): Tile


fun flood(region: Region, lines: List<MutableList<Tile>>, pos: Point) {
  val tile = lines[pos.row][pos.col] 
  if (tile is Used && tile.region === region) {
    return
  }
  if (tile is Used || (tile is Unused && tile.label != region.label)) {
    ++region.perimeter
    return
  }
  lines[pos.row][pos.col] = Used(region)
  ++region.area
  for (d in Direction.values()) {
    flood(region, lines, d.adjust(pos))
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
        val newRegion = Region(tile.label, 0, 0)
        regions.add(newRegion)
        flood(newRegion, lines, Point(rowIndex, colIndex))
      }
    }
  }
  println(regions.map { it.cost }.sum())
}
